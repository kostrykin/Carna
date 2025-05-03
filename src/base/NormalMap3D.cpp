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

#include <LibCarna/base/NormalMap3D.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// NormalMap3D
// ----------------------------------------------------------------------------------

NormalMap3D::NormalMap3D()
{
}


NormalMap3D::NormalMap3D( const math::Vector3ui& size )
    : size( size )
{
}



}  // namespace Carna :: base

}  // namespace Carna
