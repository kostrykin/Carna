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

#include <Carna/base/view/Renderable.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// Renderable
// ----------------------------------------------------------------------------------

Renderable::Renderable( const Geometry& geometry, const Matrix4f& modelViewTransform )
    : myGeometry( &geometry )
    , modelViewTransform( modelViewTransform )
{
}


const Geometry& Renderable::geometry() const
{
    return *myGeometry;
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
