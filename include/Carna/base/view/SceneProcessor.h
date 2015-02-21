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

#ifndef SCENEPROCESSOR_H_6014714286
#define SCENEPROCESSOR_H_6014714286

#include <Carna/Carna.h>

/** \file   SceneProcessor.h
  * \brief  Defines \ref Carna::base::view::SceneProcessor.
  */

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// SceneProcessor
// ----------------------------------------------------------------------------------

class CARNA_LIB SceneProcessor
{

	Framebuffer* myOutput;

public:

	void setOutput( Framebuffer& );
	
	Framebuffer& output() const;
	
	void render( /*const Camera&*/ ) const;

}; // SceneProcessor



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // SCENEPROCESSOR_H_6014714286
