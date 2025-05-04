/*
 *  Copyright (C) 2010 - 2016 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 * 
 * 
 *  Copyright (C) 2021 - 2025 Leonid Kostrykin
 * 
 */

#include <LibCarna/base/glew.h>
#include <LibCarna/presets/OccludedRenderingStage.h>
#include <LibCarna/base/RenderState.h>
#include <LibCarna/base/RenderTask.h>
#include <LibCarna/base/Framebuffer.h>
#include <LibCarna/base/Viewport.h>
#include <set>

namespace Carna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// OccludedRenderingStage :: Details
// ----------------------------------------------------------------------------------

struct OccludedRenderingStage::Details
{

    Details();

    std::set< const base::RenderStage* > enabledStages;

    float occlusionTranslucency;

}; // OccludedRenderingStage :: Details


OccludedRenderingStage::Details::Details()
    : occlusionTranslucency( DEFAULT_OCCLUSION_TRANSLUCENCY )
{
}



// ----------------------------------------------------------------------------------
// OccludedRenderingStage :: VideoResources
// ----------------------------------------------------------------------------------

struct OccludedRenderingStage::VideoResources
{

    VideoResources( const base::ShaderProgram& shader, unsigned int w, unsigned int h );

    const base::ShaderProgram& shader;
    const std::unique_ptr< base::Texture< 2 > > renderTexture;
    base::Framebuffer fbo;

}; // OccludedRenderingStage::VideoResources


OccludedRenderingStage::VideoResources::VideoResources( const base::ShaderProgram& shader, unsigned int w, unsigned int h )
    : shader( shader )
    , renderTexture( base::Framebuffer::createRenderTexture() )
    , fbo( w, h, *renderTexture )
{
}



// ----------------------------------------------------------------------------------
// OccludedRenderTask
// ----------------------------------------------------------------------------------

class OccludedRenderTask : public base::RenderTask
{

    OccludedRenderingStage& ctrl;

public:

    OccludedRenderTask( OccludedRenderingStage& ctrl, const base::RenderTask& parent, base::Framebuffer& output );

protected:

    virtual void renderStage( base::RenderStage& rs, const base::Viewport& vp ) override;

}; // OccludedRenderTask


OccludedRenderTask::OccludedRenderTask( OccludedRenderingStage& ctrl, const base::RenderTask& parent, base::Framebuffer& output )
    : base::RenderTask( parent, output )
    , ctrl( ctrl )
{
}


void OccludedRenderTask::renderStage( base::RenderStage& rs, const base::Viewport& vp )
{
    if( ctrl.isStageEnabled( rs ) )
    {
        RenderTask::renderStage( rs, vp );
    }
}



// ----------------------------------------------------------------------------------
// OccludedRenderingStage
// ----------------------------------------------------------------------------------

const float OccludedRenderingStage::DEFAULT_OCCLUSION_TRANSLUCENCY = 0.33f;


OccludedRenderingStage::OccludedRenderingStage()
    : pimpl( new Details() )
{
}


OccludedRenderingStage::~OccludedRenderingStage()
{
    if( vr.get() != nullptr )
    {
        if( isInitialized() )
        {
            renderer().glContext().makeCurrent();
        }
        base::ShaderManager::instance().releaseShader( vr->shader );
    }
}


void OccludedRenderingStage::disableAllStages()
{
    pimpl->enabledStages.clear();
}


void OccludedRenderingStage::enableStage( const base::RenderStage& rs )
{
    pimpl->enabledStages.insert( &rs );
}


void OccludedRenderingStage::disableStage( const base::RenderStage& rs )
{
    pimpl->enabledStages.erase( &rs );
}


bool OccludedRenderingStage::isStageEnabled( const base::RenderStage& rs ) const
{
    return pimpl->enabledStages.find( &rs ) != pimpl->enabledStages.end();
}


void OccludedRenderingStage::renderPass
    ( const base::math::Matrix4f& viewTransform
    , base::RenderTask& rt
    , const base::Viewport& vp )
{
    using namespace base;
    if( vr.get() == nullptr )
    {
        const ShaderProgram& shader = ShaderManager::instance().acquireShader( "unshaded" );
        vr.reset( new VideoResources( shader, vp.width(), vp.height() ) );
    }

    /* Fork the render task.
     */
    OccludedRenderTask rtFork( *this, rt, vr->fbo );

    /* Provide the depth buffer to the forked task.
     */
    const Viewport forkViewport( vp, 0, 0, vr->fbo.width(), vr->fbo.height() );
    const unsigned int outputFramebufferId = base::Framebuffer::currentId();
    LIBCARNA_RENDER_TO_FRAMEBUFFER( vr->fbo,

        base::Framebuffer::copyDepthAttachment( outputFramebufferId, vr->fbo.id, vp, forkViewport );

        /* Configure render state.
         */
        base::RenderState rs;
        rs.setDepthTestFunction( GL_GEQUAL );

        /* Do the rendering.
         */
        glClearColor( 0, 0, 0, 0 );
        rtFork.render( forkViewport, base::GLContext::COLOR_BUFFER_BIT );

    );

    /* Draw results back to this stage's output framebuffer.
     */
    RenderState rs;
    rs.setBlend( true );
    rs.setDepthWrite( false );
    rs.setDepthTest( false );
    base::FrameRenderer::RenderTextureParams params( 0 );
    params.alphaFactor = pimpl->occlusionTranslucency;
    vr->renderTexture->bind( 0 );
    rt.renderer.renderTexture( params );
}


void OccludedRenderingStage::prepareFrame( base::Node& root )
{
}


void OccludedRenderingStage::reshape( base::FrameRenderer& fr, unsigned int width, unsigned int height )
{
    RenderStage::reshape( fr, width, height );
    if( vr.get() != nullptr )
    {
        vr.reset( new VideoResources( vr->shader, width, height ) );
    }
}


void OccludedRenderingStage::setOcclusionTranslucency( float occlusionTranslucency )
{
    pimpl->occlusionTranslucency = occlusionTranslucency;
}


float OccludedRenderingStage::occlusionTranslucency() const
{
    return pimpl->occlusionTranslucency;
}



}  // namespace Carna :: presets

}  // namespace Carna
