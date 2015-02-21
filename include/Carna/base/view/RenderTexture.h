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
  * \brief  Defines \ref Carna::base::view::RenderTexture.
  */

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// RenderTexture
// ----------------------------------------------------------------------------------

/** \brief  Represents a texture that can be rendered to.
  *
  * Instances of this class must be bound to \ref FramebufferObject "framebuffer objects" in order to render to them.
  *
  * \author Leonid Kostrykin
  * \date   9.3.2011 - 18.3.13
  */
class CARNA_LIB RenderTexture
{

    NON_COPYABLE

public:

    /** \brief  Acquires GL texture object.
      *
      * \param  w   specifies texture's width.
      * \param  h   specifies texture's height.
      * \param  fp  uses floating point pixels instead of unsigned byte when \c true.
      */
    RenderTexture( unsigned int w, unsigned int h, bool fp = false );

    /** \brief  Releases associated GL texture object.
      */
    ~RenderTexture();


    /** \brief  Identifies the associated GL texture object.
      */
    const unsigned int id;

    /** \brief  Holds whether texture format is floating point but not unsigned byte.
      */
    const bool floatingPoint;


    /** \brief  Invalidates the texture's contents and resizes it.
      */
    void resize( unsigned int w, unsigned int h );


    /** \brief  Returns the texture's width.
      */
    inline unsigned int width() const
    {
        return w;
    }

    /** \brief  Returns the texture's height.
      */
    inline unsigned int height() const
    {
        return h;
    }


private:

    /** \property w
      * \brief  This texture's width.
      *
      * \property h
      * \brief  This texture's height.
      */
    unsigned int w, h;

}; // RenderTexture



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // RENDERTEXTURE_H_6014714286
