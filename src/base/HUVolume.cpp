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

#include <LibCarna/base/HUVolume.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// HUVolume
// ----------------------------------------------------------------------------------

HUVolume::HUVolume()
{
}


HUVolume::HUVolume( const math::Vector3ui& size )
    : size( size )
{
}



}  // namespace Carna :: base

}  // namespace Carna
