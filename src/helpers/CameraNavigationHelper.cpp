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

#include <Carna/helpers/CameraNavigationHelper.h>
#include <Carna/base/Camera.h>
#include <Carna/base/math.h>

namespace Carna
{

namespace helpers
{



// ----------------------------------------------------------------------------------
// CameraNavigationHelper
// ----------------------------------------------------------------------------------


CameraNavigationHelper::CameraNavigationHelper()
    : cam( nullptr )
{
}


void CameraNavigationHelper::setCamera( base::Camera& cam )
{
    pimpl->cam = &cam;
}


void CameraNavigationHelper::rotateHorizontally( float radians )
{
}


void CameraNavigationHelper::rotateVertically( float radians )
{
}


void CameraNavigationHelper::moveAxially( float distance )
{
}



}  // namespace Carna :: helpers

}  // namespace Carna
