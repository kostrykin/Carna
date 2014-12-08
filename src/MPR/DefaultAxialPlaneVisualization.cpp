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

#include <Carna/MPR/DefaultAxialPlaneVisualization.h>
#include <Carna/MPR/DefaultAxialPlaneRenderer.h>

namespace Carna
{

namespace MPR
{



// ----------------------------------------------------------------------------------
// MPR :: DefaultAxialPlaneVisualization
// ----------------------------------------------------------------------------------

DefaultAxialPlaneVisualization::DefaultAxialPlaneVisualization( base::model::Scene& model )
    : AxialPlaneVisualization( model )
{
    this->makeRotatableView();
}


DefaultAxialPlaneVisualization::~DefaultAxialPlaneVisualization()
{
}


base::view::Renderer* DefaultAxialPlaneVisualization::createRenderer( base::view::SceneProvider& provider )
{
    return new DefaultAxialPlaneRenderer( *this, provider );
}


DefaultAxialPlaneRenderer& DefaultAxialPlaneVisualization::renderer()
{
    return static_cast< DefaultAxialPlaneRenderer& >( base::Visualization::renderer() );
}


const DefaultAxialPlaneRenderer& DefaultAxialPlaneVisualization::renderer() const
{
    return static_cast< const DefaultAxialPlaneRenderer& >( base::Visualization::renderer() );
}



}  // namespace MPR

}  // namespace Carna
