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

#include <Carna/base/view/MeshControl.h>
#include <Carna/base/view/Mesh.h>
#include <Carna/base/CarnaException.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// MeshControl
// ----------------------------------------------------------------------------------

void MeshControl::uploadResource()
{
    CARNA_ASSERT( myMesh.get() == nullptr );
    myMesh.reset( createMesh() );
}


void MeshControl::deleteResource()
{
    myMesh.reset();
}


const MeshBase& MeshControl::mesh() const
{
    CARNA_ASSERT( myMesh.get() != nullptr );
    return *myMesh;
}


bool MeshControl::isSameResource( const VideoResourceControl& ) const
{
    return false;
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
