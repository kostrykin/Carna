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

#include <Carna/base/view/Texture3DManager.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// Texture3DManager
// ----------------------------------------------------------------------------------

Texture3DManager::Texture3DManager( const std::function< Texture3D*() >& createTexture )
    : VideoResourceManagerBase::VideoResourceManagerBase( createTexture )
{
}


Texture3D* Texture3DManager::createResource()
{
    return source();
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
