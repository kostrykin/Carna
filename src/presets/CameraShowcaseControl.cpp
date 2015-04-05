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
#include <Carna/base/Log.h>

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

const float CameraShowcaseControl::DEFAULT_MIN_DISTANCE =   50;
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
    CARNA_ASSERT( minDistance >= 0 );
    pimpl->minDistance = minDistance;
    if( pimpl->cam != nullptr )
    {
        moveAxially( 0 );
    }
}


void CameraShowcaseControl::setMaxDistance( float maxDistance )
{
    CARNA_ASSERT( maxDistance >= 0 );
    pimpl->maxDistance = maxDistance;
    if( pimpl->cam != nullptr )
    {
        moveAxially( 0 );
    }
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
    const base::math::Matrix4f rotation = base::math::rotation4f( base::math::vector3( rotAxis ), radians );
    pimpl->cam->localTransform = rotation * pimpl->cam->localTransform;
}


void CameraShowcaseControl::moveAxially( float units )
{
    CARNA_ASSERT( pimpl->cam != nullptr );
    if( pimpl->minDistance > pimpl->maxDistance )
    {
        base::Log::instance().record( base::Log::error, "CameraShowcaseControl: 'minDistance' > 'maxDistance'" );
        return;
    }

    /* Update location.
     */
    pimpl->cam->localTransform = pimpl->cam->localTransform * base::math::translation4f( 0, 0, units );
    
    /* Pay attention to 'pimpl->minDistance' and 'pimpl->maxDistance'.
     */
    const float offset = pimpl->cam->localTransform.col( 2 ).dot( pimpl->cam->localTransform.col( 3 ) );
    if( offset < pimpl->minDistance || offset > pimpl->maxDistance )
    {
        const float correctedOffset = base::math::clamp( offset, pimpl->minDistance, pimpl->maxDistance );
        const base::math::Vector3f location = base::math::vector3< float, 4 >( pimpl->cam->localTransform.col( 2 ) ) * correctedOffset;
        pimpl->cam->localTransform.col( 3 ) = base::math::vector4( location, 1 );
    }
}


void CameraShowcaseControl::moveLaterally( float unitsX, float unitsY )
{
    base::Log::instance().record( base::Log::warning, "CameraShowcaseControl::moveLaterally not implemented yet." );
}



}  // namespace Carna :: presets

}  // namespace Carna
