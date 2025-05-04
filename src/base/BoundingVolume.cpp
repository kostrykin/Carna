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

#include <LibCarna/base/BoundingVolume.h>

namespace LibCarna
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



}  // namespace LibCarna :: base

}  // namespace LibCarna
