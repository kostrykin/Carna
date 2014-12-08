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

#include <Carna/VolumeRenderings/DefaultVolumeVisualization.h>
#include <Carna/VolumeRenderings/DefaultVolumeRenderer.h>

namespace Carna
{

namespace VolumeRenderings
{



// ----------------------------------------------------------------------------------
// DefaultVolumeVisualization
// ----------------------------------------------------------------------------------

DefaultVolumeVisualization::DefaultVolumeVisualization()
{
}


base::view::Renderer* DefaultVolumeVisualization::createRenderer( base::view::SceneProvider& provider )
{
    return new DefaultVolumeRenderer( *this, provider );
}


DefaultVolumeRenderer& DefaultVolumeVisualization::renderer()
{
    return static_cast< DefaultVolumeRenderer& >( Visualization::renderer() );
}


const DefaultVolumeRenderer& DefaultVolumeVisualization::renderer() const
{
    return static_cast< const DefaultVolumeRenderer& >( Visualization::renderer() );
}



}  // namespace Carna :: VolumeRenderings

}  // namespace Carna
