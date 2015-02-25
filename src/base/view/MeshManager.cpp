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

#include <Carna/base/view/MeshManager.h>
#include <Carna/base/view/Mesh.h>
#include <Carna/base/CarnaException.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// MeshManager
// ----------------------------------------------------------------------------------

void MeshManager::uploadResources()
{
    CARNA_ASSERT( myMesh.get() == nullptr );
    myMesh.reset( createMesh() );
}


void MeshManager::deleteResources()
{
    myMesh.reset();
}


const MeshBase& MeshManager::mesh() const
{
    CARNA_ASSERT( myMesh.get() != nullptr );
    return *myMesh;
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
