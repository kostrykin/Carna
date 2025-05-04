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

#include <LibCarna/base/ProjectionControl.h>
#include <LibCarna/base/CarnaException.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// ProjectionControl :: Details
// ----------------------------------------------------------------------------------

struct ProjectionControl::Details
{
    Details();
    
    unsigned int viewportWidth;
    unsigned int viewportHeight;
    
    float minimumVisibleDistance;
    float maximumVisibleDistance;
    
    bool isUpdateAvailable;
};


ProjectionControl::Details::Details()
    : minimumVisibleDistance( DEFAULT_MINIMUM_VISIBLE_DISTANCE )
    , maximumVisibleDistance( DEFAULT_MAXIMUM_VISIBLE_DISTANCE )
    , isUpdateAvailable( true )
{
}



// ----------------------------------------------------------------------------------
// ProjectionControl
// ----------------------------------------------------------------------------------

const float ProjectionControl::DEFAULT_MINIMUM_VISIBLE_DISTANCE = 1e1f;
const float ProjectionControl::DEFAULT_MAXIMUM_VISIBLE_DISTANCE = 1e5f;


ProjectionControl::ProjectionControl()
    : pimpl( new Details() )
{
}


ProjectionControl::~ProjectionControl()
{
}


void ProjectionControl::setViewportWidth( unsigned int width )
{
    LIBCARNA_ASSERT( width > 0 );
    if( pimpl->viewportWidth != width )
    {
        pimpl->viewportWidth = width;
        invalidateProjection();
    }
}


void ProjectionControl::setViewportHeight( unsigned int height )
{
    LIBCARNA_ASSERT( height > 0 );
    if( pimpl->viewportHeight != height )
    {
        pimpl->viewportHeight = height;
        invalidateProjection();
    }
}


unsigned int ProjectionControl::viewportWidth() const
{
    return pimpl->viewportWidth;
}


unsigned int ProjectionControl::viewportHeight() const
{
    return pimpl->viewportHeight;
}


void ProjectionControl::setMinimumVisibleDistance( float minimumVisibleDistance )
{
    LIBCARNA_ASSERT( minimumVisibleDistance >= 0 );
    if( !math::isEqual( pimpl->minimumVisibleDistance, minimumVisibleDistance ) )
    {
        pimpl->minimumVisibleDistance = minimumVisibleDistance;
        invalidateProjection();
    }
}


void ProjectionControl::setMaximumVisibleDistance( float maximumVisibleDistance )
{
    LIBCARNA_ASSERT( maximumVisibleDistance > 0 );
    if( !math::isEqual( pimpl->maximumVisibleDistance, maximumVisibleDistance ) )
    {
        pimpl->maximumVisibleDistance = maximumVisibleDistance;
        invalidateProjection();
    }
}


float ProjectionControl::minimumVisibleDistance() const
{
    return pimpl->minimumVisibleDistance;
}


float ProjectionControl::maximumVisibleDistance() const
{
    return pimpl->maximumVisibleDistance;
}


bool ProjectionControl::isUpdateAvailable() const
{
    return pimpl->isUpdateAvailable;
}


void ProjectionControl::invalidateProjection()
{
    pimpl->isUpdateAvailable = true;
}


void ProjectionControl::setProjectionValidated() const
{
    pimpl->isUpdateAvailable = false;
}



}  // namespace Carna :: base

}  // namespace Carna
