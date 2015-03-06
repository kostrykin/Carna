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

#include <Carna/base/RenderTask.h>
#include <Carna/base/RenderStage.h>
#include <Carna/base/Framebuffer.h>
#include <Carna/base/Camera.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// RenderTask
// ----------------------------------------------------------------------------------

RenderTask::RenderTask( const FrameRenderer& renderer, const math::Matrix4f& proj, const math::Matrix4f& vt )
    : myOutput( nullptr )
    , nextRenderStage( 0 )
    , myViewTransform( vt )
    , renderer( renderer )
    , projection( proj )
{
}


RenderTask::RenderTask( const FrameRenderer& renderer, const math::Matrix4f& proj, const math::Matrix4f& vt, Framebuffer& output )
    : myOutput( &output )
    , nextRenderStage( 0 )
    , myViewTransform( vt )
    , renderer( renderer )
    , projection( proj )
{
}


RenderTask::RenderTask( const RenderTask& parent, Framebuffer& output )
    : myOutput( &output )
    , nextRenderStage( parent.nextRenderStage )
    , myViewTransform( parent.myViewTransform )
    , renderer( parent.renderer )
    , projection( parent.projection )
{
}


const math::Matrix4f& RenderTask::viewTransform() const
{
    return myViewTransform;
}


void RenderTask::overrideViewTransform( const math::Matrix4f& viewTransform )
{
    this->myViewTransform = viewTransform;
    for( std::size_t rsIdx = nextRenderStage; rsIdx < renderer.stages(); ++rsIdx )
    {
        RenderStage& rs = renderer.stageAt( rsIdx );
        rs.setViewTransformFixed( false );
    }
}


void RenderTask::render( const Viewport& vp, unsigned int clearBuffersMask )
{
    const std::unique_ptr< Framebuffer::Binding > fboBinding( myOutput == nullptr ? nullptr : new Framebuffer::Binding( *myOutput ) );
    vp.makeActive();
    if( clearBuffersMask != 0 )
    {
        renderer.glContext().clearBuffers( clearBuffersMask );
    }
    while( nextRenderStage < renderer.stages() )
    {
        RenderStage& rs = renderer.stageAt( nextRenderStage );
        ++nextRenderStage;
        renderStage( rs, vp );
    }
    vp.done();
}


void RenderTask::renderStage( RenderStage& rs, const Viewport& vp )
{
    rs.renderPass( myViewTransform, *this, vp );
}


void RenderTask::finish()
{
    nextRenderStage = renderer.stages();
}



}  // namespace Carna :: base

}  // namespace Carna
