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

#include <Carna/base/view/FrameRenderer.h>
#include <Carna/base/view/RenderTask.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// FrameRenderer
// ----------------------------------------------------------------------------------

FrameRenderer::FrameRenderer( unsigned int width, unsigned int height )
	: myWidth( width )
	, myHeight( height )
	, reshaped( true )
{
}


FrameRenderer::~FrameRenderer()
{
	clearStages();
}


std::size_t FrameRenderer::stages() const
{
	return myStages.size();
}


void FrameRenderer::appendStages( RenderStage* rs )
{
	myStages.push_back( rs );
}

	
void FrameRenderer::clearStages()
{
	std::for_each( myStages.begin(), myStages.end(), std::default_delete() );
	myStages.clear();
}


RenderStage& FrameRenderer::stageAt( std::size_t position ) const
{
	return *myStages[ position ];
}


unsigned int FrameRenderer::width() const
{
	return myWidth;
}


unsigned int FrameRenderer::height() const
{
	return myHeight;
}


void FrameRenderer::reshape( unsigned int width, unsigned int height )
{
	myWidth = width;
	myHeight = height;
	reshaped = true;
}


void FrameRenderer::render( const Camera& cam, Node& root ) const
{
	// reshape render stages' buffers and update world transforms
	for( auto rsItr = myStages.begin(); rsItr != myStages.end(); ++rsItr )
	{
		RenderStage& rs = **rsItr;
		
		// ensure buffers are properly sized
		if( reshaped || !rs.isInitialized() )
		{
			rs.reshape( myWidth, myHeight );
		}
		
		// update world transforms
		rs.prepareFrame( root );
	}
	
	// mark that all buffer sizes have been established
	reshaped = false;
	
	// render frame
	RenderTask task( *this, cam );
	task.render();
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
