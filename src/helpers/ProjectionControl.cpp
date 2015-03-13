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

#include <Carna/helpers/ProjectionControl.h>

namespace Carna
{

namespace helpers
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
};


ProjectionControl::Details::Details()
    : minimumVisibleDistance( DEFAULT_MINIMUM_VISIBLE_DISTANCE )
    , maximumVisibleDistance( DEFAULT_MAXIMUM_VISIBLE_DISTANCE )
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
    pimpl->viewportWidth = width;
}


void ProjectionControl::setViewportHeight( unsigned int height )
{
    pimpl->viewportHeight = height;
}


unsigned int ProjectionControl::viewportWidth() const
{
    return pimpl->viewportWidth;
}


unsigned int ProjectionControl::viewportHeight() const
{
    return pimpl->viewportHeight;
}


void ProjectionControl::setMinimumVisibileDistance( float minimumVisibleDistance )
{
    pimpl->minimumVisibleDistance = minimumVisibleDistance;
}


void ProjectionControl::setMaximumVisibileDistance( float maximumVisibleDistance )
{
    pimpl->maximumVisibleDistance = maximumVisibleDistance;
}


float ProjectionControl::minimumVisibleDistance() const
{
    return pimpl->minimumVisibleDistance;
}


float ProjectionControl::maximumVisibleDistance() const
{
    return pimpl->maximumVisibleDistance;
}



}  // namespace Carna :: helpers

}  // namespace Carna

