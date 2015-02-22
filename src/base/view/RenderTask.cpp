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

#include <Carna/base/view/RenderTask.h>
#include <Carna/base/view/RenderStage.h>
#include <Carna/base/view/Framebuffer.h>
#include <Carna/base/view/Camera.h>
#include <Carna/base/Matrix4f.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// RenderTask
// ----------------------------------------------------------------------------------

RenderTask::RenderTask( const FrameRenderer& renderer, const Matrix4f& projection, const Matrix4f& viewTransform )
    : myOutput( nullptr )
    , nextRenderStage( 0 )
    , viewTransform( viewTransform )
    , renderer( renderer )
    , projection( projection )
{
}


RenderTask::RenderTask( const FrameRenderer& renderer, const Matrix4f& projection, const Matrix4f& viewTransform, Framebuffer& output )
    : myOutput( &output )
    , nextRenderStage( 0 )
    , viewTransform( viewTransform )
    , renderer( renderer )
    , projection( projection )
{
}


RenderTask::RenderTask( const RenderTask& parent, Framebuffer& output )
    : myOutput( &output )
    , nextRenderStage( parent.nextRenderStage )
    , viewTransform( parent.viewTransform )
    , renderer( parent.renderer )
    , projection( parent.projection )
{
}


void RenderTask::overrideViewTransform( const Matrix4f& viewTransform )
{
    this->viewTransform = viewTransform;
    for( std::size_t rsIdx = nextRenderStage; rsIdx < renderer.stages(); ++rsIdx )
    {
        RenderStage& rs = renderer.stageAt( rsIdx );
        rs.setViewTransformFixed( false );
    }
}


void RenderTask::render( const Viewport& vp )
{
    Framebuffer::Binding binding( *myOutput );
    while( nextRenderStage < renderer.stages() )
    {
        RenderStage& rs = renderer.stageAt( nextRenderStage );
        ++nextRenderStage;
        rs.preparePass( viewTransform );
        rs.renderPass( *this, vp );
    }
}
    
    
void RenderTask::finish()
{
    nextRenderStage = renderer.stages();
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
