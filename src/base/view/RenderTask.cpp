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

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// RenderTask
// ----------------------------------------------------------------------------------

RenderTask::RenderTask( const FrameRenderer& renderer, const Camera& cam, Node& root )
	: myOutput( nullptr )
	, cam( &cam )
	, renderer( renderer )
	, root( root )
{
}


RenderTask::RenderTask( const FrameRenderer& renderer, const Camera& cam, Node& root, Framebuffer& output )
	: myOutput( &output )
	, cam( &cam )
	, renderer( renderer )
	, root( root )
{
}


RenderTask::RenderTask( RenderTask& parent, Framebuffer& output )
	: myOutput( &output )
	, cam( parent.cam )
	, nextRenderStage( parent.nextRenderStage )
	, renderer( parent.renderer )
	, root( parent.root )
{
}

	
const Camera& RenderTask::camera() const
{
	return *cam;
}


void RenderTask::render()
{
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
