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
#include <Carna/base/Texture3D.h>
#include <Carna/base/CarnaException.h>
#include <Carna/base/Text.h>

namespace Carna
{

namespace base
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


const math::Vector3ui& Texture3D::size() const
{
    return mySize;
}


const base::math::Matrix4f& Texture3D::textureCoordinatesCorrection() const
{
    if( myTextureCoordinatesCorrection.get() == nullptr )
    {
        const base::math::Vector3f texelSize   = mySize.cast< float >().cwiseInverse();
        const base::math::Vector3f texelOffset = texelSize / 2;
        const base::math::Vector3f texelScale  = base::math::Vector3f( 1, 1, 1 ) - texelSize;

        base::math::Matrix4f& m = *new base::math::Matrix4f();
        m << texelScale.x(),              0,              0, texelOffset.x(),
                          0, texelScale.y(),              0, texelOffset.y(),
                          0,              0, texelScale.z(), texelOffset.z(),
                          0,              0,              0,               1;
        myTextureCoordinatesCorrection.reset( &m );
    }
    return *myTextureCoordinatesCorrection;
}


void Texture3D::upload( int internalFormat, const math::Vector3ui& size, int pixelFormat, int bufferType, const void* bufferData )
{
    CARNA_ASSERT( size.x() % 2 == 0 && size.y() % 2 == 0 && size.z() % 2 == 0 );
    bind( SETUP_UNIT );
    glTexImage3D( GL_TEXTURE_3D, 0, internalFormat
                , size.x(), size.y(), size.z()
                , 0, pixelFormat, bufferType, bufferData );

    mySize = size;
    myTextureCoordinatesCorrection.reset();
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


bool Texture3D::controlsSameVideoResource( const GeometryFeature& ) const
{
    return false;
}



}  // namespace Carna :: base

}  // namespace Carna
