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

#ifndef RENDERSTAGE_H_6014714286
#define RENDERSTAGE_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>

/** \file   RenderStage.h
  * \brief  Defines \ref Carna::base::view::RenderStage.
  */

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// RenderStage
// ----------------------------------------------------------------------------------

class CARNA_LIB RenderStage
{

    NON_COPYABLE

public:

    virtual ~RenderStage();
    
    /** \brief
      * Orders this scene processor to reshape it's buffers according to the specified dimensions.
      */
    virtual void reshape( unsigned int width, unsigned int height ) = 0;
    
    /** \brief
      * Tells whether this scene processor is ready for rendering.
      *
      * In particular this method returns \c true if this scene processor only requires
      * \ref reshape to be called if the dimensions actually have changed. In contrary, i.e.
      * if this method returns \c false, \ref reshape will be called before every frame.
      */
    virtual bool isInitialized() const = 0;
    
    /** \brief
      * Called once before each frame.
      *
      * If this scene processor requires a \ref RenderQueue "render queue",
      * than this is the right place to \ref RenderQueue::build "build" it.
      */
    virtual void prepareFrame( const Camera& cam, Node& root ) = 0;
    
    virtual void render( RenderTask& ) const = 0;

}; // RenderStage



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // RENDERSTAGE_H_6014714286
