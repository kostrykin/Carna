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

#include <LibCarna/base/glew.hpp>
#include <LibCarna/base/glError.hpp>
#include <LibCarna/base/Texture.hpp>
#include <LibCarna/base/LibCarnaException.hpp>
#include <LibCarna/base/text.hpp>

namespace LibCarna
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


unsigned int TextureBase::maxTextureSize()
{
    GLint maxSize;
    glGetIntegerv( GL_MAX_TEXTURE_SIZE, &maxSize );
    return static_cast< unsigned int >( maxSize );
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
    GLint unpackAlignment; // TODO: in the future it will be better to use unpackAlignment as a parameter along of bufferPtr
    glGetIntegerv( GL_UNPACK_ALIGNMENT, &unpackAlignment );
    if( bufferType == GL_UNSIGNED_BYTE && pixelFormat == GL_RED )
    {
        glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    }
    checkGLTextureDataParameters( internalFormat, pixelFormat, bufferType, bufferPtr );
    glTexImage3D( GL_TEXTURE_3D, 0, internalFormat, size.x(), size.y(), size.z(), 0, pixelFormat, bufferType, bufferPtr );
    glPixelStorei( GL_UNPACK_ALIGNMENT, unpackAlignment );
    REPORT_GL_ERROR;
}



}  // namespace LibCarna :: base

}  // namespace LibCarna
