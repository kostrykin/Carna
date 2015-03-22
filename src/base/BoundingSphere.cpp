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

#include <Carna/base/BoundingSphere.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// BoundingSphere
// ----------------------------------------------------------------------------------

struct BoundingSphere::Details
{
    float radius;
};



// ----------------------------------------------------------------------------------
// BoundingSphere
// ----------------------------------------------------------------------------------

BoundingSphere::BoundingSphere( float radius )
    : pimpl( new Details() )
{
    CARNA_ASSERT( radius > 0 );
    pimpl->radius = radius;
}


BoundingSphere::~BoundingSphere()
{
}


float BoundingSphere::radius() const
{
    return pimpl->radius;
}


void BoundingSphere::setRadius( float radius )
{
    CARNA_ASSERT( radius > 0 );
    pimpl->radius = radius;
}


float BoundingSphere::computeDistance2( const math::Vector3f& point ) const
{
    /* Transform 'point' from model space to local coordinate system.
     */
    const math::Vector3f pointLocal = math::vector3f( inverseTransform() * math::vector4f( point, 1 ) );

    /* Construct plane in 'pointLocal' that faces the origin, i.e. the sphere's
     * center in its local coordinate system.
     */
    const math::Vector3f normal = pointLocal.normalized();

    /* Compute the distance.
     */
    const float distance = normal.dot( point );
    return math::sq( distance );
}



}  // namespace Carna :: base

}  // namespace Carna
