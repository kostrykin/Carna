/*
 *  Copyright (C) 2021 Leonid Kostrykin
 *
 */

#include <LibCarna/base/IntensityVolume.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// IntensityVolume
// ----------------------------------------------------------------------------------

IntensityVolume::IntensityVolume()
{
}


IntensityVolume::IntensityVolume( const math::Vector3ui& size )
    : size( size )
{
}



}  // namespace Carna :: base

}  // namespace Carna
