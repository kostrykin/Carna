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

#ifndef VIEWPORT_H_6014714286
#define VIEWPORT_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>

/** \file   Viewport.h
  * \brief  Defines \ref Carna::base::Viewport.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Viewport
// ----------------------------------------------------------------------------------

/** \brief
  * Defines a \ref WindowCoordinates "rendering viewport". The viewport is a property
  * of the \ref GLContext "current OpenGL context".
  *
  * \author Leonid Kostrykin
  * \date   22.2.15 - 19.3.15
  */
class CARNA_LIB Viewport
{

    NON_COPYABLE

    const Viewport* parent;

    mutable bool isDone;

public:

    /** \brief
      * Creates root viewport. For internal usage only.
      */
    Viewport( const FrameRenderer& fr, bool fitSquare );
    
    /** \brief
      * Derives viewport from \a parent.
      */
    Viewport( const Viewport& parent, unsigned int left, unsigned int top, unsigned int width, unsigned int height );

    /** \brief
      * Derives viewport from \a parent. The arguments must be given relatively.
      *
      * \todo finish this
      */
    Viewport( const Viewport& parent, float width, float height, float left, float top );

    /** \brief
      * Restores the parent viewport if this viewport is still active.
      */
    ~Viewport();

    unsigned int left;

    unsigned int top;

    unsigned int width;

    unsigned int height;

    /** Makes this viewport the active one of the
      * \ref GLContext "current OpenGL context".
      */
    void makeActive() const;

    /** \brief
      * Restores the parent viewport.
      */
    void done() const;

}; // Viewport



}  // namespace Carna :: base

}  // namespace Carna

#endif // VIEWPORT_H_6014714286
