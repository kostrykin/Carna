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

#include <Carna/helpers/CameraNavigationControl.h>
#include <Carna/base/Camera.h>
#include <Carna/base/math.h>

namespace Carna
{

namespace helpers
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
    base::math::Matrix4f rotation = cam->localTransform;
    rotation( 0, 3 ) = rotation( 1, 3 ) = rotation( 2, 3 ) = 0;

    const base::math::Matrix4f newRotation = base::math::rotation4f( axisX, axisY, axisZ, radians );
    const base::math::Matrix4f translation = base::math::translation4f( cam->localTransform.col( 3 ) );
    cam->localTransform = rotation * newRotation * translation;
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


void CameraNavigationControl::moveAxially( float distance )
{
    CARNA_ASSERT( pimpl->cam != nullptr );
    pimpl->cam->localTransform *= base::math::translation4f( 0, 0, -distance );
}



}  // namespace Carna :: helpers

}  // namespace Carna
