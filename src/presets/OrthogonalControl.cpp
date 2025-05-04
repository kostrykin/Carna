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

#include <LibCarna/presets/OrthogonalControl.h>
#include <LibCarna/base/math.h>

namespace LibCarna
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
    LIBCARNA_ASSERT( cc != nullptr );
}


OrthogonalControl::~OrthogonalControl()
{
}


void OrthogonalControl::setZoomStrength( float zoomStrength )
{
    LIBCARNA_ASSERT( zoomStrength > 0 );
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
    const float shortSideLength = static_cast< float >( std::min( viewportWidth(), viewportHeight() ) );
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
    base::Camera* const camera = dynamic_cast< base::Camera* >( &cam );
    if( camera != nullptr )
    {
        camera->setOrthogonalProjectionHint( true );
    }
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



}  // namespace LibCarna :: presets

}  // namespace LibCarna
