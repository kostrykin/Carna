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

#include <Carna/base/view/Geometry.h>
#include <Carna/base/view/GeometryDefinition.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// Geometry
// ----------------------------------------------------------------------------------

Geometry::Geometry( int geometryType )
    : myDefinition( nullptr )
    , geometryType( geometryType )
{
}


Geometry::~Geometry()
{
    if( hasDefinition() )
    {
        myDefinition->removeFrom( *this );
    }
}


void Geometry::setDefinition( GeometryDefinition& gd )
{
    if( myDefinition != &gd )
    {
        removeDefinition();
        myDefinition = &gd;
        gd.applyTo( *this );
    }
}


void Geometry::removeDefinition()
{
    if( hasDefinition() )
    {
        GeometryDefinition* const gd = myDefinition;
        myDefinition = nullptr;
        gd->removeFrom( *this );
    }
}


bool Geometry::hasDefinition() const
{
    return myDefinition != nullptr;
}


GeometryDefinition& Geometry::definition() const
{
    return *myDefinition;
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
