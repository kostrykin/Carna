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

#include <LibCarna/presets/PerspectiveControl.hpp>
#include <LibCarna/base/math.hpp>

namespace LibCarna
{

namespace presets
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
    LIBCARNA_ASSERT( radians > 0 );
    if( !base::math::isEqual( pimpl->fovHorizontal, radians ) )
    {
        pimpl->fovHorizontal = radians;
        invalidateProjection();
    }
}


float PerspectiveControl::fovHorizontal() const
{
    return pimpl->fovHorizontal;
}


void PerspectiveControl::updateProjection( base::math::Matrix4f& projection ) const
{
    LIBCARNA_ASSERT_EX
        ( minimumVisibleDistance() > 0
        , "PerspectiveControl does not support 'minimumVisibleDistance' set to 0!" );
    
    const float aspect = viewportHeight() / static_cast< float >( viewportWidth() );
    const float zNear  = minimumVisibleDistance();
    const float zFar   = maximumVisibleDistance();
    projection = base::math::frustum4f( pimpl->fovHorizontal, aspect, zNear, zFar );
    setProjectionValidated();
}



}  // namespace LibCarna :: presets

}  // namespace LibCarna
