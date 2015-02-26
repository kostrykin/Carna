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

Renderable::Renderable( const Geometry& geometry, const math::Matrix4f& modelViewTransform )
    : myGeometry( &geometry )
    , myModelViewTransform( new math::Matrix4f( modelViewTransform ) )
{
}


Renderable::Renderable( const Renderable& other )
    : myGeometry( other.myGeometry )
    , myModelViewTransform( new math::Matrix4f( *other.myModelViewTransform ) )
{
}


const Geometry& Renderable::geometry() const
{
    return *myGeometry;
}


const math::Matrix4f& Renderable::modelViewTransform() const
{
    return *myModelViewTransform;
}


Renderable& Renderable::operator=( const Renderable& r )
{
    myGeometry = r.myGeometry;
    *myModelViewTransform = *r.myModelViewTransform;
    return *this;
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
