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

#include <LibCarna/base/Camera.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Camera
// ----------------------------------------------------------------------------------

Camera::Camera()
    : orthogonalProjectionHint( false )
{
}


void Camera::updateWorldTransform()
{
    Spatial::updateWorldTransform();
    myViewTransform = worldTransform().inverse();
}


const math::Matrix4f& Camera::viewTransform() const
{
    return myViewTransform;
}


void Camera::setProjection( const math::Matrix4f& projection )
{
    myProjection = projection;
}


const math::Matrix4f& Camera::projection() const
{
    return myProjection;
}


void Camera::setOrthogonalProjectionHint( bool isOrthogonalProjection )
{
    this->orthogonalProjectionHint = isOrthogonalProjection;
}


bool Camera::isOrthogonalProjectionHintSet() const
{
    return orthogonalProjectionHint;
}



}  // namespace Carna :: base

}  // namespace Carna
