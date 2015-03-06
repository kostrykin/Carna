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

#include <Carna/presets/OccludedRenderingStage.h>
#include <Carna/base/RenderState.h>
#include <Carna/base/Framebuffer.h>
#include <Carna/base/RenderTexture.h>
#include <Carna/base/Viewport.h>

namespace Carna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// OccludedRenderingStage :: Details
// ----------------------------------------------------------------------------------

struct OccludedRenderingStage::Details
{

    base::GLContext* context;

    void activateGLContext() const;

}; // OccludedRenderingStage :: Details


void OccludedRenderingStage::Details::activateGLContext() const
{
    if( context != nullptr )
    {
        context->makeActive();
    }
}



// ----------------------------------------------------------------------------------
// OccludedRenderingStage :: VideoResources
// ----------------------------------------------------------------------------------

struct OccludedRenderingStage::VideoResources
{

    VideoResources( const base::ShaderProgram& shader, unsigned int w, unsigned int h );

    const base::ShaderProgram& shader;
    base::RenderTexture renderTexture;
    base::Framebuffer fbo;

}; // OccludedRenderingStage::VideoResources


OccludedRenderingStage::VideoResources::VideoResources( const base::ShaderProgram& shader, unsigned int w, unsigned int h )
    : shader( shader )
    , renderTexture( w, h )
    , fbo( renderTexture )
{
}



// ----------------------------------------------------------------------------------
// OccludedRenderingStage
// ----------------------------------------------------------------------------------

OccludedRenderingStage::OccludedRenderingStage()
    : pimpl( new Details() )
{
}


OccludedRenderingStage::~OccludedRenderingStage()
{
    if( vr.get() != nullptr )
    {
        pimpl->activateGLContext();
        base::ShaderManager::instance().releaseShader( vr->shader );
    }
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
        vr.reset( new VideoResources( shader, vp.width, vp.height ) );
    }

    /* Do the rendering.
     */
    Viewport fboViewport( vp, 0, 0, vr->fbo.width(), vr->fbo.height() );
    fboViewport.makeActive();
    CARNA_RENDER_TO_FRAMEBUFFER( vr->fbo,
        rt.renderer.glContext().clearBuffers( GLContext::COLOR_BUFFER_BIT );
        //OpaqueRenderingStage::renderPass( viewTransform, rt, vp );
    );
    fboViewport.done();
}


bool OccludedRenderingStage::isInitialized() const
{
    return pimpl->context != nullptr;
}


void OccludedRenderingStage::prepareFrame( base::Node& root )
{
}


void OccludedRenderingStage::reshape( const base::FrameRenderer& fr, const base::Viewport& vp )
{
    pimpl->context = &fr.glContext();
    if( vr.get() != nullptr )
    {
        vr.reset( new VideoResources( vr->shader, vp.width, vp.height ) );
    }
}



}  // namespace Carna :: presets

}  // namespace Carna
