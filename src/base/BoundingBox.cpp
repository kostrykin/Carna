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

#include <Carna/base/BoundingBox.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// BoundingBox
// ----------------------------------------------------------------------------------

struct BoundingBox::Details
{
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    math::Vector3f size;
};



// ----------------------------------------------------------------------------------
// BoundingBox
// ----------------------------------------------------------------------------------

BoundingBox::BoundingBox( const math::Vector3f& size )
    : pimpl( new Details() )
{
    setSize( size );
}


BoundingBox::BoundingBox( float width, float height, float depth )
    : pimpl( new Details() )
{
    setSize( width, height, depth );
}


BoundingBox::~BoundingBox()
{
}


void BoundingBox::setSize( const math::Vector3f& size )
{
    CARNA_ASSERT( size.x() > 0 && size.y() > 0 && size.z() > 0 );
    pimpl->size = size;
}


void BoundingBox::setSize( float width, float height, float depth )
{
    setSize( math::Vector3f( width, height, depth ) );
}


float BoundingBox::computeDistance2( const math::Vector3f& point ) const
{
    /* Transform 'point' from model space to local coordinate system.
     */
    const math::Vector3f pointLocal = math::vector3f( inverseTransform() * math::vector4f( point, 1 ) );

    /* Compute point 'q' within the box volume that is close-most to 'pointLocal'.
     */
    const math::Vector3f halfSize = pimpl->size / 2;
    const math::Vector3f q = pointLocal.cwiseMin( halfSize ).cwiseMax( -halfSize );

    /* Compute distance.
     */
    return q.squaredNorm();
}



}  // namespace Carna :: base

}  // namespace Carna
