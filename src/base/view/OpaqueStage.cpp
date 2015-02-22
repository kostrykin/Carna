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
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
