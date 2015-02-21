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

RenderTask::RenderTask( const FrameRenderer& renderer, const Camera& cam )
    : myOutput( nullptr )
    , cam( &cam )
    , nextRenderStage( 0 )
    , myWorldViewTransform( cam.worldTransform().inverse() )
    , renderer( renderer )
{
}


RenderTask::RenderTask( const FrameRenderer& renderer, const Camera& cam, Framebuffer& output )
    : myOutput( &output )
    , cam( &cam )
    , nextRenderStage( 0 )
    , myWorldViewTransform( cam.worldTransform().inverse() )
    , renderer( renderer )
{
}


RenderTask::RenderTask( RenderTask& parent, Framebuffer& output )
    : myOutput( &output )
    , cam( parent.cam )
    , nextRenderStage( parent.nextRenderStage )
    , myWorldViewTransform( parent.myWorldViewTransform )
    , renderer( parent.renderer )
{
}

    
const Camera& RenderTask::camera() const
{
    return *cam;
}


const Matrix4f& RenderTask::worldViewTransform() const
{
    return myWorldViewTransform;
}


void RenderTask::render()
{
    Framebuffer::Binding binding( *myOutput );
    while( nextRenderStage < renderer.stages() )
    {
        RenderStage& rs = renderer.stageAt( nextRenderStage );
        ++nextRenderStage;
        rs.render( *this );
    }
}
    
    
void RenderTask::finish()
{
    nextRenderStage = renderer.stages();
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
