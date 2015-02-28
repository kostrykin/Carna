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

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// MeshManager
// ----------------------------------------------------------------------------------

MeshManager::MeshManager( const std::function< MeshBase*() >& createMesh )
    : VideoResourceManagerBase::VideoResourceManagerBase( createMesh )
{
}


MeshBase* MeshManager::createResource()
{
    return source();
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
