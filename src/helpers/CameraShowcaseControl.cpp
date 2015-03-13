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

#include <Carna/helpers/CameraShowcaseControl.h>
#include <Carna/base/Camera.h>
#include <Carna/base/math.h>

namespace Carna
{

namespace helpers
{



// ----------------------------------------------------------------------------------
// CameraShowcaseControl :: Details
// ----------------------------------------------------------------------------------

struct CameraShowcaseControl::Details
{
    Details();

    base::Spatial* cam;
    
    float minDistance;
    
    float maxDistance;
};


CameraShowcaseControl::Details::Details()
    : cam( nullptr )
{
}



// ----------------------------------------------------------------------------------
// CameraShowcaseControl
// ----------------------------------------------------------------------------------

const float CameraShowcaseControl::DEFAULT_MIN_DISTANCE =   10;
const float CameraShowcaseControl::DEFAULT_MAX_DISTANCE = 2000;


CameraShowcaseControl::CameraShowcaseControl()
    : pimpl( new Details() )
{
}


CameraShowcaseControl::~CameraShowcaseControl()
{
}


void CameraShowcaseControl::setMinDistance( float minDistance )
{
    pimpl->minDistance = minDistance;
}


void CameraShowcaseControl::setMaxDistance( float maxDistance )
{
    pimpl->maxDistance = maxDistance;
}


float CameraShowcaseControl::minDistance() const
{
    return pimpl->minDistance;
}


float CameraShowcaseControl::maxDistance() const
{
    return pimpl->maxDistance;
}


void CameraShowcaseControl::setCamera( base::Spatial& cam )
{
    pimpl->cam = &cam;
}


void CameraShowcaseControl::rotateHorizontally( float radians )
{
    CARNA_ASSERT( pimpl->cam != nullptr );

    const base::math::Matrix4f newRotation = base::math::rotation4f( 0, 1, 0, radians );
    pimpl->cam->localTransform = newRotation * pimpl->cam->localTransform;
}


void CameraShowcaseControl::rotateVertically( float radians )
{
    CARNA_ASSERT( pimpl->cam != nullptr );
    
    base::math::Matrix4f rotation = pimpl->cam->localTransform;
    rotation( 0, 3 ) = rotation( 1, 3 ) = rotation( 2, 3 ) = 0;

    const base::math::Matrix4f newRotation = base::math::rotation4f( 1, 0, 0, radians );
    const base::math::Matrix4f translation = base::math::translation4f( pimpl->cam->localTransform.col( 3 ) );
    pimpl->cam->localTransform = rotation * newRotation * translation;
}


void CameraShowcaseControl::moveAxially( float distance )
{
    CARNA_ASSERT( pimpl->cam != nullptr );

    /* Compute new offset from origin.
     */
    const float offset0 = std::sqrt( base::math::translationDistanceSq( pimpl->cam->localTransform ) );
    const float offset1 = base::math::clamp( offset0 + distance, pimpl->minDistance, pimpl->maxDistance );
    
    /* Compute new camera location.
     */
    const base::math::Vector3f viewDirection = base::math::vector3f( pimpl->cam->localTransform.col( 2 ) );
    const base::math::Vector3f location = viewDirection * offset1;
    
    /* Keep the rotation, update the translation.
     */
    pimpl->cam->localTransform.col( 3 ) = base::math::vector4f( location, 1 );
}



}  // namespace Carna :: helpers

}  // namespace Carna
