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

RenderTask::RenderTask( const FrameRenderer& renderer )
    : myOutput( nullptr )
    , nextRenderStage( 0 )
    , renderer( renderer )
{
}


RenderTask::RenderTask( const FrameRenderer& renderer, Framebuffer& output )
    : myOutput( &output )
    , nextRenderStage( 0 )
    , renderer( renderer )
{
}


RenderTask::RenderTask( const RenderTask& parent, Framebuffer& output )
    : myOutput( &output )
    , nextRenderStage( parent.nextRenderStage )
    , renderer( parent.renderer )
{
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
