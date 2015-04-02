/*
 *  Copyright (C) 2010 - 2015 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#include <Carna/base/glew.h>
#include <Carna/presets/CompositionStage.h>
#include <Carna/base/FrameRenderer.h>
#include <Carna/base/Framebuffer.h>
#include <Carna/base/Texture.h>
#include <Carna/base/ShaderManager.h>
#include <Carna/base/ShaderUniform.h>
#include <Carna/base/Viewport.h>
#include <Carna/base/RenderState.h>

namespace Carna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// CompositionStage :: Details
// ----------------------------------------------------------------------------------

struct CompositionStage::Details
{
    Details();
    CompositionMode compositionMode;
    bool swap;

    std::unique_ptr< base::Texture< 2 > > intermediateRenderTexture;
    std::unique_ptr< base::Framebuffer  > intermediateBuffer;
    const base::ShaderProgram* interleaveShader;
    
    void renderInterleaved
        ( CompositionStage* self
        , const base::math::Matrix4f& viewTransform
        , base::RenderTask& rt
        , const base::Viewport& vp );

    void renderInterleavedPass
        ( CompositionStage* self
        , const base::math::Matrix4f& viewTransform
        , base::RenderTask& rt
        , const base::Viewport& vp
        , bool isFirstInvocation
        , bool isFirstSource );
    
    void renderAside
        ( CompositionStage* self
        , const base::math::Matrix4f& viewTransform
        , base::RenderTask& rt
        , const base::Viewport& vp );
};


CompositionStage::Details::Details()
    : swap( false )
    , interleaveShader( nullptr )
{
}


void CompositionStage::Details::renderInterleaved
    ( CompositionStage* self
    , const base::math::Matrix4f& viewTransform
    , base::RenderTask& rt
    , const base::Viewport& vp )
{
    renderInterleavedPass( self, viewTransform, rt, vp,  true, false ^ swap );
    renderInterleavedPass( self, viewTransform, rt, vp, false,  true ^ swap );
}


void CompositionStage::Details::renderInterleavedPass
    ( CompositionStage* self
    , const base::math::Matrix4f& viewTransform
    , base::RenderTask& rt
    , const base::Viewport& vp
    , bool isFirstInvocation
    , bool isFirstSource )
{
    CARNA_ASSERT( interleaveShader != nullptr );
    
    /* Render to intermediate buffer.
     */
    const base::Viewport framebufferViewport( *intermediateBuffer );
    CARNA_RENDER_TO_FRAMEBUFFER( *intermediateBuffer,
        
        glClearColor( 0, 0, 0, 0 );
        rt.renderer.glContext().clearBuffers( base::GLContext::COLOR_BUFFER_BIT | base::GLContext::DEPTH_BUFFER_BIT );
        self->renderPass( viewTransform, rt, framebufferViewport, isFirstInvocation, isFirstSource )
        
    );

    /* Draw back to output buffer.
     */
    base::RenderState rs;
    rs.setDepthTest( false );
    rs.setDepthWrite( false );
    const static unsigned int UNIT = base::TextureBase::SETUP_UNIT + 1;
    intermediateRenderTexture->bind( UNIT );
    rt.renderer.glContext().setShader( *interleaveShader );
    base::ShaderUniform< int >( "mod2result", isFirstInvocation ? 0 : 1 ).upload();
    base::FrameRenderer::RenderTextureParams params( UNIT );
    params.useDefaultShader = false;
    rt.renderer.renderTexture( params );
}


void CompositionStage::Details::renderAside
    ( CompositionStage* self
    , const base::math::Matrix4f& vt
    , base::RenderTask& rt
    , const base::Viewport& vp )
{
    /* Render the left screen.
     */
    base::Viewport vp2( vp, vp.marginLeft() / 2, vp.marginTop() / 2, vp.width() / 2, vp.height() );
    self->renderPass( vt, rt, vp2, true, false ^ swap );

    /* Render the right screen.
     */
    vp2.setMarginLeft( vp2.marginLeft() + vp.parentWidth() / 2 );
    self->renderPass( vt, rt, vp2, false, true ^ swap );
}



// ----------------------------------------------------------------------------------
// CompositionStage
// ----------------------------------------------------------------------------------

CompositionStage::CompositionStage( CompositionMode compositionMode )
    : pimpl( new Details() )
{
    pimpl->compositionMode = compositionMode;
}


CompositionStage::~CompositionStage()
{
    if( isInitialized() && pimpl->interleaveShader != nullptr )
    {
        renderer().glContext().makeCurrent();
        base::ShaderManager::instance().releaseShader( *pimpl->interleaveShader );
    }
}


bool CompositionStage::isCompositionSwapped() const
{
    return pimpl->swap;
}


void CompositionStage::setCompositionSwap( bool swap )
{
    pimpl->swap = swap;
}


CompositionStage::CompositionMode CompositionStage::compositionMode() const
{
    return pimpl->compositionMode;
}


void CompositionStage::setCompositionMode( CompositionMode compositionMode )
{
    pimpl->compositionMode = compositionMode;
}


void CompositionStage::reshape( const base::FrameRenderer& fr, unsigned int width, unsigned int height )
{
    base::RenderStage::reshape( fr, width, height );
    if( pimpl->intermediateBuffer.get() == nullptr )
    {
        /* Initialize buffers.
         */
        pimpl->intermediateRenderTexture.reset( base::Framebuffer::createRenderTexture() );
        pimpl->intermediateBuffer.reset( new base::Framebuffer( width, height, *pimpl->intermediateRenderTexture ) );

        /* Acquire other resources.
         */
        pimpl->interleaveShader = &base::ShaderManager::instance().acquireShader( "interleave" );
    }
    else
    {
        /* Reshape buffers.
         */
        pimpl->intermediateBuffer->resize( width, height );
    }
}


void CompositionStage::renderPass
    ( const base::math::Matrix4f& viewTransform
    , base::RenderTask& rt
    , const base::Viewport& vp )
{
    switch( pimpl->compositionMode )
    {
    
    case interleave:
        pimpl->renderInterleaved( this, viewTransform, rt, vp );
        break;
    
    case aside:
        pimpl->renderAside( this, viewTransform, rt, vp );
        break;
        
    default:
        CARNA_FAIL( "Unknown composition mode!" );
    
    }
}


void CompositionStage::prepareFrame( base::Node& root )
{
}



}  // namespace Carna :: presets

}  // namespace Carna
