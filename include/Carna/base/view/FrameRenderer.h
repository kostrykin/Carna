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

#ifndef FRAMERENDERER_H_6014714286
#define FRAMERENDERER_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>
#include <vector>

/** \file   FrameRenderer.h
  * \brief  Defines \ref Carna::base::view::FrameRenderer.
  */

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// FrameRenderer
// ----------------------------------------------------------------------------------

class CARNA_LIB FrameRenderer
{

	NON_COPYABLE

	std::vector< RenderStage* > myStages;
	
	unsigned int myWidth, myHeight;
	
	mutable bool reshaped;

public:
	
	FrameRenderer( unsigned int width, unsigned int height );

	~FrameRenderer();

	std::size_t stages() const;
	
	void appendStage( RenderStage* );
	
	void clearStages();
	
	RenderStage& stageAt( std::size_t position ) const;
	
	unsigned int width() const;
	
	unsigned int height() const;
	
	void reshape( unsigned int width, unsigned int height );
	
	void render( const Camera& cam, Node& root ) const;

}; // FrameRenderer



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // FRAMERENDERER_H_6014714286
