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

#include <Carna/helpers/VolumeGridHelper.h>

namespace Carna
{

namespace helpers
{



// ----------------------------------------------------------------------------------
// VolumeGridHelperBase
// ----------------------------------------------------------------------------------

VolumeGridHelperBase::VolumeGridHelperBase( const base::math::Vector3ui& nativeResolution )
    : nativeResolution( nativeResolution )
{
}


VolumeGridHelperBase::~VolumeGridHelperBase()
{
}


VolumeGridHelperBase::Spacing::Spacing( const base::math::Vector3f& millimeters )
    : millimeters( millimeters )
{
}


VolumeGridHelperBase::Dimensions::Dimensions( const base::math::Vector3f& millimeters )
    : millimeters( millimeters )
{
}


void VolumeGridHelperBase::loadHUData( const std::function< base::HUV( const base::math::Vector3ui& ) >& huData )
{
    loadIntensities( [&huData]( const base::math::Vector3ui& loc )
        {
            auto intensity = ( huData( loc ) + 1024 ) / 4095.f;
            if( intensity < 0 ) intensity = 0;
            if( intensity > 1 ) intensity = 1;
            return intensity;
        }
    );
}



}  // namespace Carna :: helpers

}  // namespace Carna
