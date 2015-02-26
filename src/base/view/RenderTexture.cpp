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

#include <Carna/base/view/glew.h>
#include <Carna/base/view/glError.h>
#include <Carna/base/view/RenderTexture.h>
#include <Carna/base/CarnaException.h>
#include <stdexcept>

namespace Carna
{

namespace base
{

namespace view
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


void RenderTexture::resize( unsigned int w, unsigned int h )
{
    CARNA_ASSERT_EX( w > 0, "texture width must be greater zero" );
    CARNA_ASSERT_EX( h > 0, "texture height must be greater zero" );

    this->w = w;
    this->h = h;

    glBindTexture( GL_TEXTURE_2D, id );

    if( floatingPoint )
    {
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0
                    , GL_RGBA, GL_UNSIGNED_BYTE, NULL );
    }
    else
    {
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0
                    , GL_RGBA, GL_UNSIGNED_BYTE, NULL );
    }

    REPORT_GL_ERROR;
}


void RenderTexture::bind( unsigned int unit ) const
{
    glActiveTexture( GL_TEXTURE0 + unit );
    glBindTexture( GL_TEXTURE_2D, id );
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
