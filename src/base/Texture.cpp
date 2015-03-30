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

#include <Carna/base/glew.h>
#include <Carna/base/glError.h>
#include <Carna/base/Texture.h>
#include <Carna/base/CarnaException.h>
#include <Carna/base/text.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// createGLTextureObject
// ----------------------------------------------------------------------------------

static unsigned int createGLTextureObject()
{
    unsigned int id;
    glGenTextures( 1, &id );
    return id;
}



// ----------------------------------------------------------------------------------
// bindGLTextureObject
// ----------------------------------------------------------------------------------

template< >
void bindGLTextureObject< 1 >( unsigned int unit, unsigned int id )
{
    glActiveTexture( GL_TEXTURE0 + unit );
    glBindTexture( GL_TEXTURE_1D, id );
}


template< >
void bindGLTextureObject< 2 >( unsigned int unit, unsigned int id )
{
    glActiveTexture( GL_TEXTURE0 + unit );
    glBindTexture( GL_TEXTURE_2D, id );
}


template< >
void bindGLTextureObject< 3 >( unsigned int unit, unsigned int id )
{
    glActiveTexture( GL_TEXTURE0 + unit );
    glBindTexture( GL_TEXTURE_3D, id );
}



// ----------------------------------------------------------------------------------
// checkGLTextureDataParameters
// ----------------------------------------------------------------------------------

static void checkGLTextureDataParameters( int& internalFormat, int& pixelFormat, int& bufferType, const void*& bufferPtr )
{
    if( bufferPtr == nullptr )
    {
        bufferPtr  = NULL;
        bufferType = GL_UNSIGNED_BYTE;
    }
}



// ----------------------------------------------------------------------------------
// TextureBase
// ----------------------------------------------------------------------------------

TextureBase::TextureBase()
    : id( createGLTextureObject() )
{
}


TextureBase::~TextureBase()
{
    glDeleteTextures( 1, &id );
}


void TextureBase::uploadGLTextureData
    ( const Eigen::Matrix< unsigned int, 1, 1 >& size
    , int internalFormat
    , int pixelFormat
    , int bufferType
    , const void* bufferPtr )
{
    checkGLTextureDataParameters( internalFormat, pixelFormat, bufferType, bufferPtr );
    glTexImage1D( GL_TEXTURE_1D, 0, internalFormat, size.x()
                , 0, pixelFormat, bufferType, bufferPtr );
    REPORT_GL_ERROR;
}


void TextureBase::uploadGLTextureData
    ( const math::Vector2ui& size
    , int internalFormat
    , int pixelFormat
    , int bufferType
    , const void* bufferPtr )
{
    checkGLTextureDataParameters( internalFormat, pixelFormat, bufferType, bufferPtr );
    glTexImage2D( GL_TEXTURE_2D, 0, internalFormat, size.x(), size.y(), 0, pixelFormat, bufferType, bufferPtr );
    REPORT_GL_ERROR;
}


void TextureBase::uploadGLTextureData
    ( const math::Vector3ui& size
    , int internalFormat
    , int pixelFormat
    , int bufferType
    , const void* bufferPtr )
{
    checkGLTextureDataParameters( internalFormat, pixelFormat, bufferType, bufferPtr );
    glTexImage3D( GL_TEXTURE_3D, 0, internalFormat, size.x(), size.y(), size.z(), 0, pixelFormat, bufferType, bufferPtr );
    REPORT_GL_ERROR;
}



}  // namespace Carna :: base

}  // namespace Carna
