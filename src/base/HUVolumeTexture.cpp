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

HUVolumeTexture::HUVolumeTexture( const HUVolume& volume )
    : volume( volume )
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
        return &volume == &other2->volume;
    }
}



}  // namespace Carna :: base

}  // namespace Carna
