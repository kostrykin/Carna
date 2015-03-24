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

#ifndef TEXTURE3DOBJECT_H_6014714286
#define TEXTURE3DOBJECT_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>
#include <Carna/base/math.h>

/** \file   Texture3DObject.h
  * \brief  Defines \ref Carna::base::Texture3DObject.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Texture3DObject
// ----------------------------------------------------------------------------------

/** \brief
  * Represents a 3D OpenGL texture object. This class realizes the RAII-idiom.
  *
  * \author Leonid Kostrykin
  * \date   22.2.15 - 24.3.15
  */
class CARNA_LIB Texture3DObject
{

    NON_COPYABLE

    void cleanup();

public:

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    /** \brief
      * Creates OpenGL texture object.
      *
      * \param size
      *     is the resolution of this texture.
      *
      * \param internalFormat
      *     specifies the number of color components in the texture, e.g.
      *     `GL_RGBA8UI` or `GL_INTENSITY16`.
      *
      * \param pixelFormat
      *     specifies the format of the pixel data, e.g. `GL_RED`, `GL_RGB` or
      *     `GL_RGBA`.
      *
      * \param bufferType
      *     specifies the data type of the pixel data pointed to by \a bufferPtr.
      *
      * \param bufferPtr
      *     points to the pixel data that will be uploaded to the texture.
      *
      * \see
      * Valid values for the parameters are available here:
      * https://www.opengl.org/sdk/docs/man3/xhtml/glTexImage3D.xml
      */
    Texture3DObject
        ( const math::Vector3ui& size
        , int internalFormat
        , int pixelFormat
        , int bufferType
        , const void* bufferPtr );

    /** \brief
      * Deletes.
      */
    virtual ~Texture3DObject();

    /** \brief
      * Texture unit that is fine to be used for temporal bindings, i.e. for setting
      * textures up. This unit shouldn't be used for lasting bindings.
      */
    const static unsigned int SETUP_UNIT = 0;

    const unsigned int id;       ///< Holds the ID of the represented OpenGL texture object.
    const math::Vector3ui size;  ///< Holds the resolution of this texture.
    const int internalFormat;    ///< Holds the number of color components in the texture, e.g. `GL_RGBA8UI` or `GL_INTENSITY16`.
    const int pixelFormat;       ///< Holds the format of the pixel data, e.g. `GL_RED`, `GL_RGB` or `GL_RGBA`.

    /** \brief
     * Binds this texture to \a unit.
     *
     * Consider using \ref SETUP_UNIT if you're binding the texture temporarily.
     */
    void bind( unsigned int unit ) const;

}; // Texture3DObject



}  // namespace Carna :: base

}  // namespace Carna

#endif // TEXTURE3DOBJECT_H_6014714286
