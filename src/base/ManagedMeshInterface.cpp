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

#include <LibCarna/base/ManagedMeshInterface.hpp>
#include <LibCarna/base/Mesh.hpp>

namespace LibCarna
{

namespace base
{



// ----------------------------------------------------------------------------------
// ManagedMeshInterface
// ----------------------------------------------------------------------------------

ManagedMeshInterface::ManagedMeshInterface( ManagedMeshBase& managed )
    : GeometryFeature::ManagedInterface( managed )
    , managed( managed )
{
    managed.acquireMesh();
}


ManagedMeshInterface::~ManagedMeshInterface()
{
    managed.releaseMesh();
}


const MeshBase& ManagedMeshInterface::get() const
{
    return managed.mesh();
}



}  // namespace LibCarna :: base

}  // namespace LibCarna
