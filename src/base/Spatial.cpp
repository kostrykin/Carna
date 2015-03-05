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

#include <Carna/base/Spatial.h>
#include <Carna/base/Node.h>
#include <Carna/base/CarnaException.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Spatial
// ----------------------------------------------------------------------------------

Spatial::Spatial()
    : myParent( nullptr )
    , localTransform( math::identity4f() )
    , myUserData( nullptr )
    , movable( true )
{
}


Spatial::~Spatial()
{
}


void Spatial::updateParent( Node& parent )
{
    CARNA_ASSERT( parent.hasChild( *this ) );
    myParent = &parent;
}


bool Spatial::hasParent() const
{
    return myParent != nullptr;
}


Spatial* Spatial::detachFromParent()
{
    CARNA_ASSERT( hasParent() );
    Spatial* const result = myParent->detachChild( *this );
    myParent = nullptr;
    return result;
}

    
Node& Spatial::parent()
{
    CARNA_ASSERT( hasParent() );
    return *myParent;
}


const Node& Spatial::parent() const
{
    CARNA_ASSERT( hasParent() );
    return *myParent;
}


void Spatial::updateWorldTransform()
{
    if( hasParent() )
    {
        myWorldTransform = myParent->worldTransform() * localTransform;
    }
    else
    {
        myWorldTransform = localTransform;
    }
}


const math::Matrix4f& Spatial::worldTransform() const
{
    return myWorldTransform;
}


void Spatial::removeUserData()
{
    myUserData = nullptr;
    CARNA_ASSERT( !hasUserData() );
}


bool Spatial::hasUserData() const
{
    return myUserData != nullptr;
}


void Spatial::setMovable( bool movable )
{
    this->movable = movable;
}


bool Spatial::isMovable() const
{
    return movable;
}



}  // namespace Carna :: base

}  // namespace Carna
