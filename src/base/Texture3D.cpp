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
// Texture3D :: VideoResourceAcquisition
// ----------------------------------------------------------------------------------

Texture3D::VideoResourceAcquisition::VideoResourceAcquisition( Texture3D& texture )
    : GeometryFeature::VideoResourceAcquisition( texture )
    , texture( texture )
{
    if( texture.videoResourceAcquisitionsCount() == 1 )
    {
        CARNA_ASSERT_EX
            ( texture.size.x() % 2 == 0 && texture.size.y() % 2 == 0 && texture.size.z() % 2 == 0
            , "Texture3D only supports even sizes." );
        
        /* Acquire the texture.
         */
        glGenTextures( 1, &texture.id );
        CARNA_ASSERT_EX( texture.id != 0, "Texture3D acquisition failed." );
        
        /* Upload texture data.
         */
        bind( SETUP_UNIT );
        glTexImage3D( GL_TEXTURE_3D, 0, texture.internalFormat
                    , texture.size.x(), texture.size.y(), texture.size.z()
                    , 0, texture.pixelFormat, texture.bufferType, texture.bufferPtr );

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
}


Texture3D::VideoResourceAcquisition::~VideoResourceAcquisition()
{
    if( texture.videoResourceAcquisitionsCount() == 1 )
    {
        /* Release the texture.
         */
        glDeleteTextures( 1, &texture.id );
        texture.id = 0;
    }
}


unsigned int Texture3D::VideoResourceAcquisition::id() const
{
    return texture.id;
}


void Texture3D::VideoResourceAcquisition::bind( unsigned int unit ) const
{
    glActiveTexture( GL_TEXTURE0 + unit );
    glBindTexture( GL_TEXTURE_3D, texture.id );
}



// ----------------------------------------------------------------------------------
// computeTextureCoordinatesCorrection
// ----------------------------------------------------------------------------------

static base::math::Matrix4f computeTextureCoordinatesCorrection( const math::Vector3ui& size )
{
    const base::math::Vector3f texelSize   = size.cast< float >().cwiseInverse();
    const base::math::Vector3f texelOffset = texelSize / 2;
    const base::math::Vector3f texelScale  = base::math::Vector3f( 1, 1, 1 ) - texelSize;

    base::math::Matrix4f m;
    m << texelScale.x(),              0,              0, texelOffset.x(),
                      0, texelScale.y(),              0, texelOffset.y(),
                      0,              0, texelScale.z(), texelOffset.z(),
                      0,              0,              0,               1;
    return m;
}



// ----------------------------------------------------------------------------------
// Texture3D
// ----------------------------------------------------------------------------------

Texture3D::Texture3D
        ( const math::Vector3ui& size
        , int internalFormat
        , int pixelFormat
        , int bufferType
        , const void* bufferPtr )
    : bufferPtr( bufferPtr )
    , size( size )
    , internalFormat( internalFormat )
    , pixelFormat( pixelFormat )
    , bufferType( bufferType )
    , textureCoordinatesCorrection( computeTextureCoordinatesCorrection( size ) )
{
}


Texture3D::~Texture3D()
{
}


bool Texture3D::controlsSameVideoResource( const GeometryFeature& ) const
{
    return false;
}


Texture3D::VideoResourceAcquisition* Texture3D::acquireVideoResource( GLContext& glc )
{
    return new VideoResourceAcquisition( *this );
}



}  // namespace Carna :: base

}  // namespace Carna
