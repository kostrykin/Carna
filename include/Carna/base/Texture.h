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

#ifndef TEXTURE_H_6014714286
#define TEXTURE_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>
#include <Carna/base/math.h>
#include <Carna/base/CarnaException.h>

/** \file   Texture.h
  * \brief  Defines \ref Carna::base::Texture.
  */

namespace Carna
{

namespace base
{


// ----------------------------------------------------------------------------------
// bindGLTextureObject
// ----------------------------------------------------------------------------------

template< unsigned int dimension >
void bindGLTextureObject( unsigned int unit, unsigned int id )
{
    static_assert( dimension >= 1 && dimension <= 3, "Texture dimension must be 1, 2 or 3." );
}


template< >
void CARNA_LIB bindGLTextureObject< 1 >( unsigned int unit, unsigned int id );


template< >
void CARNA_LIB bindGLTextureObject< 2 >( unsigned int unit, unsigned int id );


template< >
void CARNA_LIB bindGLTextureObject< 3 >( unsigned int unit, unsigned int id );



// ----------------------------------------------------------------------------------
// Texture
// ----------------------------------------------------------------------------------

/** \brief
  * Specialization with dimension-independent definitions.
  *
  * \author Leonid Kostrykin
  * \date   24.3.15
  */
template< >
class CARNA_LIB Texture< 0 >
{

    NON_COPYABLE

public:

    /** \brief
      * Holds the ID of the represented OpenGL texture object.
      */
    const unsigned int id;

    /** \brief
      * Deletes the maintained OpenGL texture object.
      */
    virtual ~Texture();

    /** \brief
      * Texture unit that is fine to be used for temporal bindings, i.e. for setting
      * textures up. This unit shouldn't be used for lasting bindings.
      */
    const static unsigned int SETUP_UNIT = 0;
    
protected:

    /** \brief
      * Creates OpenGL texture object.
      */
    Texture();

    void uploadGLTextureData
        ( const Eigen::Matrix< unsigned int, 1, 1 >& size
        , int internalFormat
        , int pixelFormat
        , int bufferType
        , const void* bufferPtr );

    void uploadGLTextureData
        ( const math::Vector2ui& size
        , int internalFormat
        , int pixelFormat
        , int bufferType
        , const void* bufferPtr );

    void uploadGLTextureData
        ( const math::Vector3ui& size
        , int internalFormat
        , int pixelFormat
        , int bufferType
        , const void* bufferPtr );
    
}; // Texture< 0 >


/** \brief
  * Represents an OpenGL texture object. This class realizes the RAII-idiom.
  *
  * \author Leonid Kostrykin
  * \date   22.2.15 - 24.3.15
  */
template< unsigned int dimension >
class Texture : public Texture< 0 >
{

public:

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    
    /** Holds the resolution type of this texture.
      */
    typedef Eigen::Matrix< unsigned int, dimension, 1 > Resolution;

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
    Texture
        ( const Resolution& size
        , int internalFormat
        , int pixelFormat
        , int bufferType
        , const void* bufferPtr );
    
    /** \brief
      * Holds the dimension of this texture.
      */
    const static unsigned int DIMENSION = dimension;

    const Resolution size;    ///< Holds the resolution of this texture.
    const int internalFormat; ///< Holds the number of color components in the texture, e.g. `GL_RGBA8UI` or `GL_INTENSITY16`.
    const int pixelFormat;    ///< Holds the format of the pixel data, e.g. `GL_RED`, `GL_RGB` or `GL_RGBA`.

    /** \brief
     * Binds this texture to \a unit.
     *
     * Consider using \ref Texture<0>::SETUP_UNIT if you're binding the texture temporarily.
     */
    void bind( unsigned int unit ) const;

}; // Texture


template< unsigned int dimension >
Texture< dimension >::Texture
        ( const Texture< dimension >::Resolution& size
        , int internalFormat
        , int pixelFormat
        , int bufferType
        , const void* bufferPtr )
    : size( size )
    , internalFormat( internalFormat )
    , pixelFormat( pixelFormat )
{
    static_assert( dimension >= 1 && dimension <= 3, "Texture dimension must be 1, 2 or 3." );
    CARNA_ASSERT_EX( id != 0, "Texture acquisition failed!" );
    
    /* Ensure that texture size is even.
     */
    for( unsigned int i = 0; i < dimension; ++i )
    {
        CARNA_ASSERT_EX( size( i, 0 ) % 2 == 0, "Texture only supports even sizes!" );
    }
    
    /* Upload data to texture.
     */
    this->bind( SETUP_UNIT );
    uploadGLTextureData( size, internalFormat, pixelFormat, bufferType, bufferPtr );
}


template< unsigned int dimension >
void Texture< dimension >::bind( unsigned int unit ) const
{
    bindGLTextureObject< dimension >( unit, id );
}



}  // namespace Carna :: base

}  // namespace Carna

#endif // TEXTURE_H_6014714286
