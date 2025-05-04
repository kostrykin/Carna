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

#include <LibCarna/helpers/VolumeGridHelper.h>

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


VolumeGridHelperBase::Spacing::Spacing( const base::math::Vector3f& units )
    : units( units )
{
}


VolumeGridHelperBase::Extent::Extent( const base::math::Vector3f& units )
    : units( units )
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
