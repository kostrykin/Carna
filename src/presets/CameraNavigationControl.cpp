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

#include <Carna/presets/CameraNavigationControl.h>
#include <Carna/base/Camera.h>
#include <Carna/base/math.h>
#include <Carna/base/Log.h>

namespace Carna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// CameraNavigationControl :: Details
// ----------------------------------------------------------------------------------

struct CameraNavigationControl::Details
{
    Details();
    
    base::Spatial* cam;
    
    void rotate( float axisX, float axisY, float axisZ, float radians );
};


CameraNavigationControl::Details::Details()
    : cam( nullptr )
{
}


void CameraNavigationControl::Details::rotate( float axisX, float axisY, float axisZ, float radians )
{
    const base::math::Matrix4f newRotation = base::math::rotation4f( axisX, axisY, axisZ, radians );
    cam->localTransform = cam->localTransform * newRotation;
}



// ----------------------------------------------------------------------------------
// CameraNavigationControl
// ----------------------------------------------------------------------------------

CameraNavigationControl::CameraNavigationControl()
    : pimpl( new Details() )
{
}


CameraNavigationControl::~CameraNavigationControl()
{
}


void CameraNavigationControl::setCamera( base::Spatial& cam )
{
    pimpl->cam = &cam;
}


void CameraNavigationControl::rotateHorizontally( float radians )
{
    CARNA_ASSERT( pimpl->cam != nullptr );
    pimpl->rotate( 0, 1, 0, radians );
}


void CameraNavigationControl::rotateVertically( float radians )
{
    CARNA_ASSERT( pimpl->cam != nullptr );
    pimpl->rotate( 1, 0, 0, radians );
}


void CameraNavigationControl::moveAxially( float units )
{
    CARNA_ASSERT( pimpl->cam != nullptr );
    pimpl->cam->localTransform = pimpl->cam->localTransform * base::math::translation4f( 0, 0, units );
}


void CameraNavigationControl::moveLaterally( float unitsX, float unitsY )
{
    CARNA_ASSERT( pimpl->cam != nullptr );
    pimpl->cam->localTransform = pimpl->cam->localTransform * base::math::translation4f( unitsX, unitsY, 0 );
}



}  // namespace Carna :: presets

}  // namespace Carna
