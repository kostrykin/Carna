/*
 *  Copyright (C) 2010 - 2016 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 * 
 * 
 *  Copyright (C) 2021 - 2025 Leonid Kostrykin
 * 
 */

#ifndef VIEWPORT_H_6014714286
#define VIEWPORT_H_6014714286

#include <LibCarna/LibCarna.hpp>
#include <LibCarna/base/noncopyable.hpp>

/** \file   Viewport.h
  * \brief  Defines \ref LibCarna::base::Viewport.
  */

namespace LibCarna
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
  */
class LIBCARNA Viewport
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

    /** \brief
      * Makes this viewport the active one of the
      * \ref GLContext "current OpenGL context".
      */
    void makeActive() const;
    
    /** \brief
      * Tells whether this viewport is currently active.
      */
    bool isActive() const;

    /** \brief
      * Restores the previous viewport.
      *
      * \pre `isActive() == true`
      */
    void done() const;
    
    void setWidth ( unsigned int );     ///< Sets \ref FrameCoordinates "viewport"  width. \pre `isActive() == false`
    void setHeight( unsigned int );     ///< Sets \ref FrameCoordinates "viewport" height. \pre `isActive() == false`
    
    void setMarginLeft( unsigned int ); ///< Sets \ref FrameCoordinates "viewport" left margin. \pre `isActive() == false`
    void setMarginTop ( unsigned int ); ///< Sets \ref FrameCoordinates "viewport"  top margin. \pre `isActive() == false`
    
    unsigned int  width() const;        ///< Tells \ref FrameCoordinates "viewport"  width.
    unsigned int height() const;        ///< Tells \ref FrameCoordinates "viewport" height.
    
    unsigned int parentWidth () const;  ///< Tells parent viewport's  width.
    unsigned int parentHeight() const;  ///< Tells parent viewport's height.
    
    unsigned int marginLeft  () const;  ///< Tells \ref FrameCoordinates "viewport's"   left margin.
    unsigned int marginTop   () const;  ///< Tells \ref FrameCoordinates "viewport's"    top margin.
    unsigned int marginRight () const;  ///< Tells \ref FrameCoordinates "viewport's"  right margin.
    unsigned int marginBottom() const;  ///< Tells \ref FrameCoordinates "viewport's" bottom margin.

}; // Viewport



}  // namespace LibCarna :: base

}  // namespace LibCarna

#endif // VIEWPORT_H_6014714286
