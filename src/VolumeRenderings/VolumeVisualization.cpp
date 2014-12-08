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

#include <Carna/VolumeRenderings/VolumeVisualization.h>
#include <Carna/VolumeRenderings/VolumeRenderer.h>
#include <Carna/VolumeRenderings/VolumeVisualizationCameraController.h>
#include <Carna/base/controller/DefaultInteractionStrategy.h>

namespace Carna
{

namespace VolumeRenderings
{



// ----------------------------------------------------------------------------------
// VolumeVisualization
// ----------------------------------------------------------------------------------

VolumeVisualization::VolumeVisualization()
    : Visualization
        ( new base::controller::DefaultInteractionStrategy
            ( *this
            , new VolumeVisualizationCameraController( *this ) ) )
{
}


VolumeVisualization::VolumeVisualization( base::controller::InteractionStrategy* initialController )
    : Visualization( initialController )
{
}


VolumeRenderer& VolumeVisualization::renderer()
{
    return static_cast< VolumeRenderer& >( Visualization::renderer() );
}


const VolumeRenderer& VolumeVisualization::renderer() const
{
    return static_cast< const VolumeRenderer& >( Visualization::renderer() );
}



}  // namespace Carna :: VolumeRenderings

}  // namespace Carna
