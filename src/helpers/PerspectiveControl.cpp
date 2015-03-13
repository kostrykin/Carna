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

#include <Carna/helpers/PerspectiveControl.h>
#include <Carna/base/math.h>

namespace Carna
{

namespace helpers
{



// ----------------------------------------------------------------------------------
// PerspectiveControl :: Details
// ----------------------------------------------------------------------------------

struct PerspectiveControl::Details
{
    Details();
    
    float fovHorizontal;
};


PerspectiveControl::Details::Details()
    : fovHorizontal( DEFAULT_FOV_HORIZONTAL )
{
}



// ----------------------------------------------------------------------------------
// PerspectiveControl
// ----------------------------------------------------------------------------------

const float PerspectiveControl::DEFAULT_FOV_HORIZONTAL = 3.14f * 45 / 180.f;


PerspectiveControl::PerspectiveControl()
    : pimpl( new Details() )
{
}


PerspectiveControl::~PerspectiveControl()
{
}


void PerspectiveControl::setFovHorizontal( float radians )
{
    pimpl->fovHorizontal = radians;
}


float PerspectiveControl::fovHorizontal() const
{
    return pimpl->fovHorizontal;
}


void PerspectiveControl::updateProjection( base::math::Matrix4f& projection ) const
{
    const float aspect = viewportHeight() / static_cast< float >( viewportWidth() );
    const float zNear  = minimumVisibleDistance();
    const float zFar   = maximumVisibleDistance();
    projection = base::math::frustum4f( pimpl->fovHorizontal, aspect, zNear, zFar );
}



}  // namespace Carna :: helpers

}  // namespace Carna

