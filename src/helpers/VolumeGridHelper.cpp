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



}  // namespace Carna :: helpers

}  // namespace Carna
