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
  * The viewport specifies how normalized device coordinates are mapped to frame
  * coordinates, as described \ref WindowCoordinates "here". This makes the viewport
  * always somehow *"naturally"* related to a frame. It has proven to be convenient
  * to organize viewports hierarchically. In this terms, each `%Viewport` instance is
  * associated with another *parent* viewport that it is derived from. Viewports,
  * that do not have such a parent viewport, are named *root* viewports.
  *
  * \author Leonid Kostrykin
  * \date   22.2.15 - 20.3.15
  */
class CARNA_LIB Viewport
{

    NON_COPYABLE

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    /** \brief
      * Creates root viewport.
      */
    Viewport( unsigned int rootWidth, unsigned int rootHeight, bool fitSquare );
    
    /** \overload
      */
    explicit Viewport( const Framebuffer& );
    
    /** \brief
      * Derives viewport from \a parent.
      */
    Viewport( const Viewport& parent, unsigned int left, unsigned int top, unsigned int width, unsigned int height );

    /** \brief
      * Restores the parent viewport if this viewport is still active.
      */
    ~Viewport();

    /** Makes this viewport the active one of the
      * \ref GLContext "current OpenGL context".
      */
    void makeActive() const;
    
    bool isActive() const;

    /** \brief
      * Restores the previous viewport.
      *
      * \pre `isActive() == true`
      */
    void done() const;
    
    void setWidth ( unsigned int );
    void setHeight( unsigned int );
    
    void setMarginLeft( unsigned int );
    void setMarginTop ( unsigned int );
    
    unsigned int  width() const;
    unsigned int height() const;
    
    unsigned int parentWidth () const;
    unsigned int parentHeight() const;
    
    unsigned int marginLeft  () const;
    unsigned int marginTop   () const;
    unsigned int marginRight () const;
    unsigned int marginBottom() const;

}; // Viewport



}  // namespace Carna :: base

}  // namespace Carna

#endif // VIEWPORT_H_6014714286
