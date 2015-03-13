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
#include <Carna/base/HUVolumeTexture.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// HUVolumeTexture
// ----------------------------------------------------------------------------------

HUVolumeTexture::HUVolumeTexture( const HUVolume& huVolume, int internalFormat, int bufferType, const void* bufferPtr )
    : Texture3D( huVolume.size, internalFormat, GL_RED, bufferType, bufferPtr )
    , huVolume( huVolume )
{
}


bool HUVolumeTexture::controlsSameVideoResource( const GeometryFeature& other ) const
{
    const HUVolumeTexture* const other2 = dynamic_cast< const HUVolumeTexture* >( &other );
    if( other2 == nullptr )
    {
        return false;
    }
    else
    {
        return &huVolume == &other2->huVolume;
    }
}



}  // namespace Carna :: base

}  // namespace Carna
