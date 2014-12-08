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

#include <Carna/VolumeRenderings/VolumeRenderer.h>
#include <Carna/VolumeRenderings/VolumeVisualization.h>

namespace Carna
{

namespace VolumeRenderings
{



// ----------------------------------------------------------------------------------
// VolumeRenderer
// ----------------------------------------------------------------------------------

VolumeRenderer::VolumeRenderer
    ( base::Visualization& module
    , base::view::SceneProvider& provider
    , QObject* parent )

    : Renderer( module, provider, parent )
{
}


VolumeRenderer::~VolumeRenderer()
{
}



}  // namespace Carna :: VolumeRenderings

}  // namespace Carna
