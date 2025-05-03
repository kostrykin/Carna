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

#include <LibCarna/base/BoundingVolume.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// BoundingVolume
// ----------------------------------------------------------------------------------

struct BoundingVolume::Details
{
    Details();

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    math::Matrix4f transform;
    math::Matrix4f inverseTransform;
    bool isInverseTransformDirty;
};


BoundingVolume::Details::Details()
    : transform( math::identity4f() )
    , isInverseTransformDirty( true )
{
}



// ----------------------------------------------------------------------------------
// BoundingVolume
// ----------------------------------------------------------------------------------

BoundingVolume::BoundingVolume()
    : pimpl( new Details() )
{
}


BoundingVolume::~BoundingVolume()
{
}


void BoundingVolume::setTransform( const math::Matrix4f& transform )
{
    pimpl->transform = transform;
}


const math::Matrix4f& BoundingVolume::transform() const
{
    return pimpl->transform;
}


const math::Matrix4f& BoundingVolume::inverseTransform() const
{
    if( pimpl->isInverseTransformDirty )
    {
        pimpl->inverseTransform = pimpl->transform.inverse();
        pimpl->isInverseTransformDirty = false;
    }
    return pimpl->inverseTransform;
}



}  // namespace Carna :: base

}  // namespace Carna
