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

#include <Carna/MPR/AxialPlaneRenderer.h>

namespace Carna
{

namespace MPR
{



// ----------------------------------------------------------------------------------
// MPR :: AxialPlaneRenderer
// ----------------------------------------------------------------------------------

AxialPlaneRenderer::AxialPlaneRenderer
    ( base::Visualization& module
    , base::view::SceneProvider& provider
    , QWidget* parent )

    : Renderer( module, provider, parent )
{
    connect( &provider.scene, SIGNAL( recommendedWindowingChanged() ), this, SLOT( invalidate() ) ); // TODO: document that this is performed
}


AxialPlaneRenderer::~AxialPlaneRenderer()
{
}



}  // namespace MPR

}  // namespace Carna
