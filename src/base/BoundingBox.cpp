/*
 *  Copyright (C) 2010 - 2014 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#include <Carna/base/BoundingBox.h>
#include <Carna/base/RayBoxHitTest.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// BoundingBox
// ----------------------------------------------------------------------------------

BoundingBox::BoundingBox( const math::Vector3f& boxSize )
    : myBoxSize( boxSize )
{
}


bool BoundingBox::testHit
    ( const math::Vector3f& rayOrigin
    , const math::Vector3f& rayDirection
    , math::Vector3f& hitLocation ) const
{
    RayBoxHitTest hitTest;
    hitTest.compute( rayDirection, rayOrigin, myBoxSize );
    if( hitTest.hitExists() )
    {
        hitLocation = hitTest.hitLocation();
        return true;
    }
    else
    {
        return false;
    }
}


void BoundingBox::setBoxSize( const math::Vector3f& boxSize )
{
    myBoxSize = boxSize;
}


const math::Vector3f& BoundingBox::boxSize() const
{
    return myBoxSize;
}



}  // namespace Carna :: base

}  // namespace Carna
