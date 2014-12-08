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

#include <Carna/SlicePlanes/SlicePlaneRenderer.h>

namespace Carna
{

namespace SlicePlanes
{



// ----------------------------------------------------------------------------------
// SlicePlaneRenderer
// ----------------------------------------------------------------------------------

const float SlicePlaneRenderer::DEFAULT_BORDER_WIDTH = 0.01f;


SlicePlaneRenderer::SlicePlaneRenderer
    ( base::Visualization& module
    , base::view::SceneProvider& provider
    , QObject* parent )

    : Renderer( module, provider, parent )
{
}


SlicePlaneRenderer::~SlicePlaneRenderer()
{
}



}  // namespace Carna :: SlicePlanes

}  // namespace Carna
