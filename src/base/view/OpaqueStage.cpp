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

#include <Carna/base/view/OpaqueStage.h>
#include <Carna/base/view/MeshControl.h>
#include <Carna/base/view/Mesh.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// OpaqueStage
// ----------------------------------------------------------------------------------

OpaqueStage::OpaqueStage()
    : GeometryStage< Renderable::ArbitraryOrder >::GeometryStage( GEOMETRY_TYPE )
{
}


void OpaqueStage::render( const Renderable& renderable )
{
    const VideoResourcesControl& vrm = renderable.geometry().aggregate( ROLE_DEFAULT_MESH ).videoResources();
    const MeshControl& meshManager = static_cast< const MeshControl& >( vrm );
    
    /* TODO: Activate proper shader, e.g. by querying the corresponding geometry aggregate.
     */

    meshManager.mesh().render();
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
