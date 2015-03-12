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

#include <Carna/helpers/CameraShowcaseHelper.h>
#include <Carna/base/Camera.h>
#include <Carna/base/math.h>

namespace Carna
{

namespace helpers
{



// ----------------------------------------------------------------------------------
// CameraShowcaseHelper
// ----------------------------------------------------------------------------------

CameraShowcaseHelper::CameraShowcaseHelper()
    : cam( nullptr )
    , exhibit( nullptr )
{
}


void CameraShowcaseHelper::setExhibit( base::Spatial& exhibit )
{
    this->exhibit = &exhibit;
    initialize();
}


void CameraShowcaseHelper::setCamera( base::Camera& cam )
{
    this->cam = &cam;
    initialize();
}


void CameraShowcaseHelper::rotateHorizontally( float radians )
{
}


void CameraShowcaseHelper::rotateVertically( float radians )
{
}


void CameraShowcaseHelper::moveAxially( float distance )
{
}



}  // namespace Carna :: helpers

}  // namespace Carna
