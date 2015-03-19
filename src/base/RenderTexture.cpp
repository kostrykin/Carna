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

#include <Carna/base/glew.h>
#include <Carna/base/glError.h>
#include <Carna/base/RenderTexture.h>
#include <Carna/base/CarnaException.h>
#include <stdexcept>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// createGlTextureObject
// ----------------------------------------------------------------------------------

static inline unsigned int createGlTextureObject()
{
    unsigned int n;
    glGenTextures( 1, &n );
    return n;
}



// ----------------------------------------------------------------------------------
// RenderTexture
// ----------------------------------------------------------------------------------

RenderTexture::RenderTexture( unsigned int w, unsigned int h, bool fp )
    : id( createGlTextureObject() )
    , floatingPoint( fp )
{
    glBindTexture( GL_TEXTURE_2D, id );
    resize( w, h );
}


RenderTexture::~RenderTexture()
{
    glDeleteTextures( 1, &id );
}


void RenderTexture::resize( unsigned int width, unsigned int height )
{
    CARNA_ASSERT_EX(  width > 0, "Texture width must be greater zero!" );
    CARNA_ASSERT_EX( height > 0, "Texture height must be greater zero!" );

    myWidth  = width;
    myHeight = height;

    glBindTexture( GL_TEXTURE_2D, id );

    if( floatingPoint )
    {
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0
                    , GL_RGBA, GL_UNSIGNED_BYTE, NULL );
    }
    else
    {
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0
                    , GL_RGBA, GL_UNSIGNED_BYTE, NULL );
    }

    REPORT_GL_ERROR;
}


void RenderTexture::bind( unsigned int unit ) const
{
    glActiveTexture( GL_TEXTURE0 + unit );
    glBindTexture( GL_TEXTURE_2D, id );
}



}  // namespace Carna :: base

}  // namespace Carna
