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

#include <Carna/base/view/glew.h>
#include <Carna/base/view/glError.h>
#include <Carna/base/view/Texture3D.h>
#include <Carna/base/CarnaException.h>
#include <Carna/base/Text.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// Texture3D
// ----------------------------------------------------------------------------------

Texture3D::Texture3D()
    : id([]()->unsigned int
        {
            unsigned int id;
            glGenTextures( 1, &id );
            return id;
        }())
{
    if( !id )
    {
        throw std::runtime_error( "Texture3D texture creation failed." );
    }
}


Texture3D::~Texture3D()
{
    glDeleteTextures( 1, &id );
}


void Texture3D::bind( unsigned int unit ) const
{
    glActiveTexture( GL_TEXTURE0 + unit );
    glBindTexture( GL_TEXTURE_3D, id );
}


void Texture3D::upload( int internalFormat, const math::Vector3ui& size, int pixelFormat, int bufferType, const void* bufferData )
{
    bind( SETUP_UNIT );
    glTexImage3D( GL_TEXTURE_3D, 0, internalFormat
                , size.x(), size.y(), size.z()
                , 0, pixelFormat, bufferType, bufferData );

    const unsigned int err = glGetError();

    if( err != GL_NO_ERROR )
    {
        std::string err_msg;

        switch( err )
        {
            case GL_INVALID_ENUM:
                err_msg = "invalid enumerator";
                break;

            case GL_INVALID_VALUE:
                err_msg = "invalid value";
                break;

            case GL_INVALID_OPERATION:
                err_msg = "invalid operation";
                break;

            case GL_OUT_OF_MEMORY:
                err_msg = "out of memory";
                break;

            default:
                err_msg = base::Text::lexical_cast< std::string >( err );
        }

        std::stringstream ss;
        ss << "3D texture acquisition failed: "
           << err_msg;

        CARNA_FAIL( ss.str() );
    }
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
