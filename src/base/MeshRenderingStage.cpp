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

#include <Carna/base/MeshRenderingStage.h>

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
