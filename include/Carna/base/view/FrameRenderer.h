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

	std::vector< SceneProcessor* > myProcessors;

public:

	~FrameRenderer();

	std::size_t processors() const;
	
	void appendProcessor( SceneProcessor* );
	
	void clearProcessors();
	
	void render( const Camera& cam, Node& root ) const;
	
	void render( const Camera& cam, Node& root, Framebuffer& out ) const;
	
private:
	
	void render( const Camera& cam, Node& root, Framebuffer* out ) const;

}; // FrameRenderer



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // FRAMERENDERER_H_6014714286
