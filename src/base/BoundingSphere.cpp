/*
 *  Copyright (C) 2010 - 2016 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 * 
 * 
 *  Copyright (C) 2021 - 2025 Leonid Kostrykin
 * 
 */

#include <LibCarna/base/BoundingSphere.h>

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
    LIBCARNA_ASSERT( radius > 0 );
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
    LIBCARNA_ASSERT( radius > 0 );
    pimpl->radius = radius;
}


void BoundingSphere::computeClosemostPoint( math::Vector3f& out, const math::Vector3f& ref ) const
{
    /* Transform 'ref' from model space to local coordinate system.
     */
    const math::Vector3f refLocal = math::vector3< float, 4 >( inverseTransform() * math::vector4( ref, 1 ) );

    /* Construct plane in 'pointLocal' that faces the origin, i.e. the sphere's
     * center in its local coordinate system.
     */
    const math::Vector3f normal = refLocal.normalized();

    /* Compute the distance in local coordinates.
     */
    const float distance = normal.dot( refLocal );

    /* Compute the close-most point.
     */
    out = normal * distance;
    
    /* Transform 'out' from local coordinate system to model space.
     */
    out = math::vector3< float, 4 >( transform() * math::vector4( out, 1 ) );
}



}  // namespace Carna :: base

}  // namespace Carna
