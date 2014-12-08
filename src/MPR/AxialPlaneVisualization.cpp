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

#include <Carna/MPR/AxialPlaneVisualization.h>
#include <Carna/MPR/AxialPlaneRenderer.h>
#include <Carna/MPR/AxialSliderScreenSpaceBuffer.h>
#include <Carna/MPR/NullSliderScreenSpaceBuffer.h>
#include <Carna/MPR/PartialSliderPainter.h>
#include <Carna/MPR/DefaultSliderPainter.h>
#include <Carna/MPR/AxialPlaneController.h>
#include <Carna/base/view/DefaultCamera.h>
#include <Carna/base/Composition.h>
#include <Carna/base/Transformation.h>

namespace Carna
{

namespace MPR
{



// ----------------------------------------------------------------------------------
// Types & Globals
// ----------------------------------------------------------------------------------

const base::Transformation frontRotation = base::Transformation().rotateY( 180, base::Transformation::DEGREES );
const base::Transformation leftRotation  = base::Transformation( frontRotation ).rotateY( 90, base::Transformation::DEGREES );
const base::Transformation topRotation   = base::Transformation( frontRotation ).rotateX( 90, base::Transformation::DEGREES );



// ----------------------------------------------------------------------------------
// MPR :: AxialPlaneVisualization
// ----------------------------------------------------------------------------------

AxialPlaneVisualization::AxialPlaneVisualization( base::model::Scene& model )
    : Visualization()
    , model( model )
{
}


AxialPlaneVisualization::~AxialPlaneVisualization()
{
}


void AxialPlaneVisualization::makeFrontView()
{
    doAfterInitialization( [&]()
        {
            AxialPlaneRenderer& axial_plane_renderer = static_cast< AxialPlaneRenderer& >( renderer() );

            axial_plane_renderer.setSliderScreenSpaceBuffer( new AxialSliderScreenSpaceBuffer( SliderType::x, SliderType::y ) );
            axial_plane_renderer.setSliderPainter( new PartialSliderPainter( true, true, false ) );

            setController( new AxialPlaneController( *this, AxialPlaneController::front ) );

            base::view::DefaultCamera* camera = new base::view::DefaultCamera( model );
            camera->setRotation( frontRotation );
            renderer().setCamera( new base::Composition< base::view::Camera >( camera ) );
        }
    );
}


void AxialPlaneVisualization::makeLeftView()
{
    doAfterInitialization( [&]()
        {
            AxialPlaneRenderer& axial_plane_renderer = static_cast< AxialPlaneRenderer& >( renderer() );

            axial_plane_renderer.setSliderScreenSpaceBuffer( new AxialSliderScreenSpaceBuffer( SliderType::z, SliderType::y ) );
            axial_plane_renderer.setSliderPainter( new PartialSliderPainter( false, true, true ) );

            setController( new AxialPlaneController( *this, AxialPlaneController::left ) );

            base::view::DefaultCamera* camera = new base::view::DefaultCamera( model );
            camera->setRotation( leftRotation );
            renderer().setCamera( new base::Composition< base::view::Camera >( camera ) );
        }
    );
}


void AxialPlaneVisualization::makeTopView()
{
    doAfterInitialization( [&]()
        {
            AxialPlaneRenderer& axial_plane_renderer = static_cast< AxialPlaneRenderer& >( renderer() );

            axial_plane_renderer.setSliderScreenSpaceBuffer( new AxialSliderScreenSpaceBuffer( SliderType::x, SliderType::z ) );
            axial_plane_renderer.setSliderPainter( new PartialSliderPainter( true, false, true ) );

            setController( new AxialPlaneController( *this, AxialPlaneController::top ) );

            base::view::DefaultCamera* camera = new base::view::DefaultCamera( model );
            camera->setRotation( topRotation );
            renderer().setCamera( new base::Composition< base::view::Camera >( camera ) );
        }
    );
}


void AxialPlaneVisualization::makeRotatableView()
{
    doAfterInitialization( [&]()
        {
            AxialPlaneRenderer& axial_plane_renderer = static_cast< AxialPlaneRenderer& >( renderer() );

            axial_plane_renderer.setSliderScreenSpaceBuffer( new NullSliderScreenSpaceBuffer() );
            axial_plane_renderer.setSliderPainter( new DefaultSliderPainter() );

            setController( new base::controller::DefaultInteractionStrategy( *this ) );

            base::view::Camera& camera = renderer().camera();
            base::view::DefaultCamera* defaultCamera;
            if( ( defaultCamera = dynamic_cast< base::view::DefaultCamera* >( &camera ) ) == nullptr )
            {
                defaultCamera = new base::view::DefaultCamera( model );
                renderer().setCamera( new base::Composition< base::view::Camera >( defaultCamera ) );
            }
        }
    );
}


AxialPlaneRenderer& AxialPlaneVisualization::renderer()
{
    return static_cast< AxialPlaneRenderer& >( base::Visualization::renderer() );
}


const AxialPlaneRenderer& AxialPlaneVisualization::renderer() const
{
    return static_cast< const AxialPlaneRenderer& >( base::Visualization::renderer() );
}



}  // namespace MPR

}  // namespace Carna
