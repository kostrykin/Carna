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
#include <Carna/base/math.h>

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

    bool viewTransformFixed;

public:

    RenderStage();

    virtual ~RenderStage();

    void setViewTransformFixed( bool viewTransformFixed );

    bool isViewTransformFixed() const;
    
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
      */
    virtual void prepareFrame( const FrameRenderer& fr, Node& root );

    /** \brief
      * Called once before each pass.
      *
      * If this scene processor requires a \ref RenderQueue "render queue",
      * than this is the right place to \ref RenderQueue::build "build" it.
      * Note that the queue needs to be rebuilt only once per frame, unless
      * \ref isViewTransformFixed is \c false. If it is \c true and this
      * is not the first invocation of this method since the last time
      * \ref prepareFrame was called, \ref RenderQueue::rewind "rewinding"
      * the queue will be sufficient.
      */
    virtual void preparePass( const math::Matrix4f& viewTransform ) = 0;
    
    virtual void renderPass( RenderTask& rt, const Viewport& vp ) = 0;

}; // RenderStage



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // RENDERSTAGE_H_6014714286
