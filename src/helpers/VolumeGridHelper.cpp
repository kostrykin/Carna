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

#include <LibCarna/helpers/VolumeGridHelper.hpp>

namespace LibCarna
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



}  // namespace LibCarna :: helpers

}  // namespace LibCarna
