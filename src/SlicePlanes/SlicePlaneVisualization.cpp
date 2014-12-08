/*
 *  Copyright (C) 2010 - 2014 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#include <Carna/SlicePlanes/SlicePlaneVisualization.h>
#include <Carna/SlicePlanes/SlicePlaneRenderer.h>

namespace Carna
{

namespace SlicePlanes
{



// ----------------------------------------------------------------------------------
// SlicePlaneVisualization
// ----------------------------------------------------------------------------------

SlicePlaneRenderer& SlicePlaneVisualization::renderer()
{
    return static_cast< SlicePlaneRenderer& >( Visualization::renderer() );
}


const SlicePlaneRenderer& SlicePlaneVisualization::renderer() const
{
    return static_cast< const SlicePlaneRenderer& >( Visualization::renderer() );
}



}  // namespace Carna :: SlicePlanes

}  // namespace Carna
