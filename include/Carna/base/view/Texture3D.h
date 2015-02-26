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

#ifndef TEXTURE3D_H_6014714286
#define TEXTURE3D_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>
#include <Carna/base/math.h>

/** \file   Texture3D.h
  * \brief  Defines \ref Carna::base::view::Texture3D.
  */

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// Texture3D
// ----------------------------------------------------------------------------------

class CARNA_LIB Texture3D
{

    NON_COPYABLE

public:

    /** \brief
      * Texture unit that is fine to be used for temporal bindings, i.e. for setting
      * textures up. This unit shouldn't be used for lasting bindings.
      */
    const static unsigned int SETUP_UNIT = 0;

    /** \brief
      * Instantiates and associates with a newly created OpenGL texture object.
      */
    Texture3D();

    /** \brief
      * Deletes the associated OpenGL object.
      */
    virtual ~Texture3D();

    /** \brief
      * The ID of the associated OpenGL object.
      */
    const unsigned int id;

    /** \brief
      * Binds this texture to \a unit.
      *
      * Consider using \ref SETUP_UNIT if you're binding the texture temporarily.
      */
    void bind( unsigned int unit ) const;

    void upload( int internalFormat, const math::Vector3ui& size, int pixelFormat, int bufferType, const void* bufferData );

}; // Texture3D



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // TEXTURE3D_H_6014714286
