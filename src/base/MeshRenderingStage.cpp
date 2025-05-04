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

#include <LibCarna/base/MeshRenderingStage.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// MeshRenderingMixin
// ----------------------------------------------------------------------------------

MeshRenderingMixin::MeshRenderingMixin( unsigned int geometryType )
    : geometryType( geometryType )
{
}


MeshRenderingMixin::~MeshRenderingMixin()
{
}



}  // namespace Carna :: base

}  // namespace Carna
