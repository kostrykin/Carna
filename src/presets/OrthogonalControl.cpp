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

#include <Carna/presets/OrthogonalControl.h>
#include <Carna/base/math.h>

namespace Carna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// OrthogonalControl :: Details
// ----------------------------------------------------------------------------------

struct OrthogonalControl::Details
{
    Details( base::CameraControl* cc );
    const std::unique_ptr< base::CameraControl > cc;
    
    float weightedAxialPosition;
    float zoomStrength;
    float zoomFactor() const;
    
    bool isRotationEnabled;
};


OrthogonalControl::Details::Details( base::CameraControl* cc )
    : cc( cc )
    , weightedAxialPosition( 0 )
    , zoomStrength( DEFAULT_ZOOM_STRENGTH )
    , isRotationEnabled( true )
{
}


float OrthogonalControl::Details::zoomFactor() const
{
    if( weightedAxialPosition >= 0 )
    {
        return 1 + weightedAxialPosition;
    }
    else
    {
        return 1 / ( 1 - weightedAxialPosition );
    }
}



// ----------------------------------------------------------------------------------
// OrthogonalControl
// ----------------------------------------------------------------------------------

const float OrthogonalControl::DEFAULT_ZOOM_STRENGTH = 1e-1f;


OrthogonalControl::OrthogonalControl( base::CameraControl* cc )
    : pimpl( new Details( cc ) )
{
    CARNA_ASSERT( cc != nullptr );
}


OrthogonalControl::~OrthogonalControl()
{
}


void OrthogonalControl::setZoomStrength( float zoomStrength )
{
    CARNA_ASSERT( zoomStrength > 0 );
    if( !base::math::isEqual( pimpl->zoomStrength, zoomStrength ) )
    {
        pimpl->zoomStrength = zoomStrength;
        invalidateProjection();
    }
}


float OrthogonalControl::zoomStrength() const
{
    return pimpl->zoomStrength;
}


float OrthogonalControl::zoomFactor() const
{
    return pimpl->zoomFactor();
}


void OrthogonalControl::updateProjection( base::math::Matrix4f& projection ) const
{
    const float shortSideLength = std::min( viewportWidth(), viewportHeight() );
    const float halfZoomFactor = pimpl->zoomFactor() / 2;

    const float left   = viewportWidth () * ( -halfZoomFactor );
    const float right  = viewportWidth () * ( +halfZoomFactor );
    const float bottom = viewportHeight() * ( -halfZoomFactor );
    const float top    = viewportHeight() * ( +halfZoomFactor );
    const float zMin = minimumVisibleDistance();
    const float zMax = maximumVisibleDistance();
    projection = base::math::ortho4f( left, right, bottom, top, zMin, zMax );
    setProjectionValidated();
}


void OrthogonalControl::moveAxially( float units )
{
    if( !base::math::isEqual( units, 0.f ) )
    {
        pimpl->weightedAxialPosition += pimpl->zoomStrength * units;
        invalidateProjection();
    }
}


void OrthogonalControl::setCamera( base::Spatial& cam )
{
    pimpl->cc->setCamera( cam );
}


void OrthogonalControl::rotateHorizontally( float radians )
{
    if( pimpl->isRotationEnabled )
    {
        pimpl->cc->rotateHorizontally( radians );
    }
}


void OrthogonalControl::rotateVertically( float radians )
{
    if( pimpl->isRotationEnabled )
    {
        pimpl->cc->rotateVertically( radians );
    }
}


void OrthogonalControl::moveLaterally( float unitsX, float unitsY )
{
    const float zoomFactor = pimpl->zoomFactor();
    pimpl->cc->moveLaterally( zoomFactor * unitsX, zoomFactor * unitsY );
}


void OrthogonalControl::setRotationEnabled( bool isRotationEnabled )
{
    pimpl->isRotationEnabled = isRotationEnabled;
}


bool OrthogonalControl::isRotationEnabled() const
{
    return pimpl->isRotationEnabled;
}



}  // namespace Carna :: presets

}  // namespace Carna

