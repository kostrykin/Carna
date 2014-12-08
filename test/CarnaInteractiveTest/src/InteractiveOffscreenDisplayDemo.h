#pragma once

#include <Carna/Carna.h>
#include <Carna/base/OffscreenVisualizationEnvironment.h>

#include "OffscreenDisplayTestingWidget.h"

class InteractiveOffscreenDisplayDemo
{

public:

    int exec( Carna::base::model::Scene& scene )
    {
        Carna::VolumeRenderings::VolumeVisualization* const view = new Carna::VolumeRenderings::DefaultVolumeVisualization();

     // setup model

        new Carna::base::view::Point3D( scene, Carna::base::model::Position::fromVolumeUnits( scene, 0.5, 0.5, 0.5 ) );

     // setup renderer

        view->doAfterInitialization( [&]()
            {
                view->renderer().installMode( new Carna::VolumeRenderings::DRR::XRay( scene ) );
                view->renderer().installMode( new Carna::VolumeRenderings::MIP::MaximumIntensityProjection() );
                view->renderer().installMode( new Carna::VolumeRenderings::DVR::DirectVolumeRendering( scene ) );

             // set initial mode

                view->renderer().setMode( Carna::VolumeRenderings::DVR::DirectVolumeRendering::NAME );
            }
        );

     // create controller

        view->doAfterInitialization( [&]()
            {
                Carna::VolumeRenderings::VolumeControllerUI* const controller_ui = new Carna::VolumeRenderings::VolumeControllerUI( view->renderer(), scene );

                controller_ui->setAttribute( Qt::WA_DeleteOnClose );
                controller_ui->show();
            }
        );
    
     // run

        Carna::base::OffscreenVisualizationEnvironment environment;
        Carna::base::view::SceneProvider provider( scene );
        OffscreenDisplayTestingWidget* const display
            = new OffscreenDisplayTestingWidget( environment, view, provider );  // takes ownership of 'view'

        display->resize( 500, 300 );
        display->setAttribute( Qt::WA_DeleteOnClose );
        display->show();

        return Carna::base::Application::exec();
    }

};
