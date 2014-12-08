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

#include <Carna/SlicePlanes/DefaultSlicePlaneVisualization.h>
#include <Carna/SlicePlanes/DefaultSlicePlaneRenderer.h>

namespace Carna
{

namespace SlicePlanes
{



// ----------------------------------------------------------------------------------
// DefaultSlicePlaneVisualization
// ----------------------------------------------------------------------------------

base::view::Renderer* DefaultSlicePlaneVisualization::createRenderer( base::view::SceneProvider& provider )
{
    return new DefaultSlicePlaneRenderer( *this, provider );
}


DefaultSlicePlaneRenderer& DefaultSlicePlaneVisualization::renderer()
{
    return static_cast< DefaultSlicePlaneRenderer& >( Visualization::renderer() );
}


const DefaultSlicePlaneRenderer& DefaultSlicePlaneVisualization::renderer() const
{
    return static_cast< const DefaultSlicePlaneRenderer& >( Visualization::renderer() );
}



}  // namespace Carna :: SlicePlanes

}  // namespace Carna
