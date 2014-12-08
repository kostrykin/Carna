#pragma once

#include <Carna/base/Application.h>
#include <Carna/base/view/SceneProvider.h>
#include <Carna/base/qt/Display.h>

#include <Carna/VolumeRenderings/VolumeRenderer.h>
#include <Carna/VolumeRenderings/DefaultVolumeVisualization.h>
#include <Carna/VolumeRenderings/VolumeControllerUI.h>
#include <Carna/VolumeRenderings/DRR/XRay.h>
#include <Carna/VolumeRenderings/MIP/MaximumIntensityProjection.h>
#include <Carna/VolumeRenderings/DVR/DirectVolumeRendering.h>

#include <Carna/base/view/Point3D.h>
#include <Carna/base/model/Position.h>

class VolumeVisualizationDemo
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

        Carna::base::view::SceneProvider provider( scene );
        Carna::base::qt::Display* const display
            = new Carna::base::qt::Display( view, provider );    // takes ownership of 'view'

        display->setAttribute( Qt::WA_DeleteOnClose );
        display->resize( 512, 512 );
        display->show();

        return Carna::base::Application::exec();
    }

};
