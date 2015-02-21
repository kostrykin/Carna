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
#include <Carna/base/noncopyable.h>

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

	NON_COPYABLE

public:

	virtual ~SceneProcessor();
	
	/** \brief
	  * Called once before each frame.
	  *
	  * If this scene processor requires a \ref RenderQueue "render queue",
	  * than this is the right place to \ref RenderQueue::build "build" it.
	  */
	virtual void prepareFrame() = 0;
	
	/** \brief
	  * Orders this scene processor to reshape it's buffers according to the specified dimensions.
	  */
	virtual void reshape( unsigned int width, unsigned int height ) = 0;
	
	/** \brief
	  * Tells whether this scene processor is ready for rendering.
	  *
	  * In particular this method returns \c true if this scene processor only requires
	  * \ref reshape to be called if the dimensions actually have changed. If this method
	  * returns \c false, \ref reshape will only be called when the dimensions change.
	  */
	virtual bool isInitialized() const = 0;
	
	virtual void render( RenderManager& ) const = 0;

}; // SceneProcessor



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // SCENEPROCESSOR_H_6014714286
