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

#include <Carna/presets/CameraShowcaseControl.h>
#include <Carna/base/Camera.h>
#include <Carna/base/math.h>

namespace Carna
{

namespace presets
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
    , minDistance( DEFAULT_MIN_DISTANCE )
    , maxDistance( DEFAULT_MAX_DISTANCE )
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
    
    const base::math::Vector4f rotAxis  = pimpl->cam->localTransform * base::math::Vector4f( 1, 0, 0, 0 );
    const base::math::Matrix4f rotation = base::math::rotation4f( base::math::vector3f( rotAxis ), radians );
    pimpl->cam->localTransform = rotation * pimpl->cam->localTransform;
}


void CameraShowcaseControl::moveAxially( float distance )
{
    CARNA_ASSERT( pimpl->cam != nullptr );

    /* TODO: Make below pay attention to 'pimpl->minDistance' and 'pimpl->maxDistance'.
     */
    pimpl->cam->localTransform = pimpl->cam->localTransform * base::math::translation4f( 0, 0, distance );
}



}  // namespace Carna :: presets

}  // namespace Carna
