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
#include <Carna/base/model/Volume.h>

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// Volume
// ----------------------------------------------------------------------------------

Volume::Volume()
{
}


Volume::Volume( const Vector3ui& size )
    : size( size )
{
}



// ----------------------------------------------------------------------------------
// Volume :: Texture
// ----------------------------------------------------------------------------------

Volume::Texture::Texture( const Volume& volume )    // TODO: move model::Volume::Texture to view::VolumeTexture,
    : textureID( []()->unsigned int                 // and kick out the glew.h dependency in this file
        {
            unsigned int textureID;
            glGenTextures( 1, &textureID );
            return textureID;
        }() )
{
    if( !textureID )
    {
        throw std::runtime_error( "Volume texture creation failed." );
    }

    glBindTexture( GL_TEXTURE_3D, textureID );

    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    volume.uploadTexture();
}


Volume::Texture::~Texture()
{
    if( textureID != 0 )
    {
        glDeleteTextures( 1, &textureID );
    }
}



}  // namespace Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna
