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

#include <LibCarna/LibCarna.h>
#include <LibCarna/base/noncopyable.h>
#include <LibCarna/base/math.h>
#include <LibCarna/base/CarnaException.h>

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

/** \brief
  * Binds OpenGL texture object \a id to texture \a unit. For internal usage only,
  * use \ref Texture::bind instead.
  *
  * \relates TextureBase
  */
template< unsigned int dimension >
void bindGLTextureObject( unsigned int unit, unsigned int id )
{
    static_assert( dimension >= 1 && dimension <= 3, "Texture dimension must be 1, 2 or 3." );
}


/** \overload
  */
template< >
void LIBCARNA bindGLTextureObject< 1 >( unsigned int unit, unsigned int id );


/** \overload
  */
template< >
void LIBCARNA bindGLTextureObject< 2 >( unsigned int unit, unsigned int id );


/** \overload
  */
template< >
void LIBCARNA bindGLTextureObject< 3 >( unsigned int unit, unsigned int id );



// ----------------------------------------------------------------------------------
// TextureBase
// ----------------------------------------------------------------------------------

/** \brief
  * \ref Texture base class with dimension-independent definitions.
  *
  * \author Leonid Kostrykin
  * \date   24.3.15 - 30.3.15
  */
class LIBCARNA TextureBase
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
    virtual ~TextureBase();

    /** \brief
      * Texture unit that is fine to be used for temporal bindings, i.e. for setting
      * textures up. This unit shouldn't be used for lasting bindings.
      */
    const static unsigned int SETUP_UNIT = 0;

    /** \brief
      * Queries `GL_MAX_TEXTURE_SIZE` from the current GL context.
      */
    static unsigned int maxTextureSize();
    
protected:

    /** \brief
      * Creates OpenGL texture object.
      */
    TextureBase();

    /** \brief
      * Wraps `glTexImage1d`.
      */
    void uploadGLTextureData
        ( const Eigen::Matrix< unsigned int, 1, 1 >& size
        , int internalFormat
        , int pixelFormat
        , int bufferType
        , const void* bufferPtr );
    
    /** \brief
      * Wraps `glTexImage2d`.
      */
    void uploadGLTextureData
        ( const math::Vector2ui& size
        , int internalFormat
        , int pixelFormat
        , int bufferType
        , const void* bufferPtr );
    
    /** \brief
      * Wraps `glTexImage3d`.
      */
    void uploadGLTextureData
        ( const math::Vector3ui& size
        , int internalFormat
        , int pixelFormat
        , int bufferType
        , const void* bufferPtr );
    
}; // TextureBase



// ----------------------------------------------------------------------------------
// Texture
// ----------------------------------------------------------------------------------

/** \brief
  * Represents an OpenGL texture object. This class realizes the RAII-idiom.
  *
  * \author Leonid Kostrykin
  * \date   22.2.15 - 24.3.15
  */
template< unsigned int dimension >
class Texture : public TextureBase
{

public:
    
    /** Holds the resolution type of this texture.
      */
    typedef Eigen::Matrix< unsigned int, dimension, 1 > Resolution;

    /** \brief
      * Creates OpenGL texture object.
      *
      * \post `isValid() == false`
      *
      * \param internalFormat
      *     specifies the number of color components in the texture, e.g.
      *     `GL_RGBA8UI` or `GL_INTENSITY16`.
      *
      * \param pixelFormat
      *     specifies the format of the pixel data, e.g. `GL_RED`, `GL_RGB` or
      *     `GL_RGBA`.
      *
      * \see
      * Valid values for the parameters are available here:
      * https://www.opengl.org/sdk/docs/man3/xhtml/glTexImage3D.xml
      */
    Texture( int internalFormat, int pixelFormat );
    
    /** \brief
      * Holds the dimension of this texture.
      */
    const static unsigned int DIMENSION = dimension;

    /** \brief
      * Tells the resolution of this texture.
      * \pre `isValid() == true`
      */
    const Resolution& size() const;
    
    /** \brief
      * Tells whether the texture has been initialized, i.e. it has a \ref size.
      */
    bool isValid() const;
    
    const int internalFormat; ///< Holds the number of color components in the texture, e.g. `GL_RGBA8UI` or `GL_INTENSITY16`.
    const int pixelFormat;    ///< Holds the format of the pixel data, e.g. `GL_RED`, `GL_RGB` or `GL_RGBA`.

    /** \brief
      * Binds this texture to \a unit. Consider using \ref TextureBase::SETUP_UNIT if
      * you're binding the texture temporarily.
      */
    void bind( unsigned int unit ) const;
    
    /** Binds this texture to \ref TextureBase::SETUP_UNIT, updates its \a size and
      * data.
      *
      * \post `isValid() == true`
      *
      * \param size
      *     is the new resolution of this texture.
      *
      * \param bufferType
      *     specifies the data type of the pixel data pointed to by \a bufferPtr.
      *
      * \param bufferPtr
      *     points to the pixel data that will be uploaded to the texture.
      *
      * No data is uploaded to the texture if `nullptr` is given for \a bufferPtr.
      * The value of \a bufferType has no meaning than.
      */
    void update( const Resolution& size, int bufferType, const void* bufferPtr );

    /** \overload
      */    
    void update( const Resolution& size );
    
private:

    std::unique_ptr< Resolution > mySize;

}; // Texture


template< unsigned int dimension >
Texture< dimension >::Texture( int internalFormat, int pixelFormat )
    : internalFormat( internalFormat )
    , pixelFormat( pixelFormat )
{
    static_assert( dimension >= 1 && dimension <= 3, "Texture dimension must be 1, 2 or 3." );
    LIBCARNA_ASSERT_EX( id != 0, "Texture acquisition failed!" );
}


template< unsigned int dimension >
void Texture< dimension >::bind( unsigned int unit ) const
{
    bindGLTextureObject< dimension >( unit, id );
}


template< unsigned int dimension >
bool Texture< dimension >::isValid() const
{
    return mySize.get() != nullptr;
}


template< unsigned int dimension >
const Eigen::Matrix< unsigned int, dimension, 1 >& Texture< dimension >::size() const
{
    LIBCARNA_ASSERT( isValid() );
    return *mySize;
}


template< unsigned int dimension >
void Texture< dimension >::update( const Eigen::Matrix< unsigned int, dimension, 1 >& size, int bufferType, const void* bufferPtr )
{
    /* Ensure that texture size is positive.
     */
    for( unsigned int i = 0; i < dimension; ++i )
    {
        LIBCARNA_ASSERT_EX( size( i, 0 ) >= 1, "Texture only supports positive sizes!" );
    }
    
    /* Ensure that z-component of the texture size is even if this is a 3D texture.
     */
    if( dimension == 3 )
    {
        LIBCARNA_ASSERT_EX( size( 2, 0 ) % 2 == 0, "3D textures must have even depth!" );
    }
    
    /* Update the OpenGL texture object.
     */
    this->bind( SETUP_UNIT );
    uploadGLTextureData( size, internalFormat, pixelFormat, bufferType, bufferPtr );
}


template< unsigned int dimension >
void Texture< dimension >::update( const Eigen::Matrix< unsigned int, dimension, 1 >& size )
{
    update( size, 0, nullptr );
}



}  // namespace Carna :: base

}  // namespace Carna

#endif // TEXTURE_H_6014714286
