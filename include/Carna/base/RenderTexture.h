/*
 *  Copyright (C) 2010 - 2014 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#ifndef RENDERTEXTURE_H_6014714286
#define RENDERTEXTURE_H_6014714286

/** \file   RenderTexture.h
  * \brief  Defines \ref Carna::base::RenderTexture.
  */

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// RenderTexture
// ----------------------------------------------------------------------------------

/** \brief
  * Maintains a 2D texture that can be rendered to. Realizes the RAII-idiom.
  *
  * Instances of this class must be bound to
  * \ref Framebuffer "framebuffer objects" in order to render to them.
  *
  * \author Leonid Kostrykin
  * \date   9.3.2011 - 18.3.13
  */
class CARNA_LIB RenderTexture
{

    NON_COPYABLE

    unsigned int myWidth;
    unsigned int myHeight;

public:

    /** \brief
      * Creates new OpenGL texture object.
      *
      * \param width
      *     is the texture's width.
      *
      * \param height
      *     is the texture's height.
      *
      * \param floatingPoint
      *     sets whether `float`-based pixels shall be used instead of unsigned byte.
      */
    RenderTexture( unsigned int width, unsigned int height, bool floatingPoint = false );

    /** \brief
      * Deletes the maintained OpenGL texture object.
      */
    ~RenderTexture();


    /** \brief
      * Identifies the maintained OpenGL texture object.
      */
    const unsigned int id;

    /** \brief
      * Holds whether texture format is floating point or unsigned byte.
      */
    const bool floatingPoint;


    /** \brief
      * Invalidates the texture's contents and resizes it.
      */
    void resize( unsigned int width, unsigned int height );

    /** \brief
      * Binds the maintained OpenGL texture object to texture \a unit.
      */
    void bind( unsigned int unit ) const;


    /** \brief
      * Tells the texture's width.
      */
    inline unsigned int width() const
    {
        return myWidth;
    }

    /** \brief
      * Tells the texture's height.
      */
    inline unsigned int height() const
    {
        return myHeight;
    }

}; // RenderTexture



}  // namespace Carna :: base

}  // namespace Carna

#endif // RENDERTEXTURE_H_6014714286
