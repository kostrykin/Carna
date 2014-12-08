#pragma once

#include <Carna/base/Application.h>
#include <Carna/base/view/SceneProvider.h>
#include <Carna/base/qt/Display.h>

#include <Carna/SlicePlanes/SlicePlaneRenderer.h>
#include <Carna/SlicePlanes/DefaultSlicePlaneVisualization.h>
#include <Carna/SlicePlanes/SlicePlaneControllerUI.h>

class SlicePlaneVisualizationDemo
{

public:

    int exec( Carna::base::model::Scene& scene )
    {
        Carna::SlicePlanes::SlicePlaneVisualization* const view = new Carna::SlicePlanes::DefaultSlicePlaneVisualization();

     // setup renderer

        view->doAfterInitialization( [&]()
            {
                view->renderer().setPlane( TRTK::Coordinate< double >( 0, 0, 1 ), 0.5 );
            }
        );

     // create controller

        view->doAfterInitialization( [&]()
            {
                Carna::SlicePlanes::SlicePlaneControllerUI* const controller_ui = new Carna::SlicePlanes::SlicePlaneControllerUI( view->renderer() );
                controller_ui->setAttribute( Qt::WA_DeleteOnClose );
                controller_ui->show();
            }
        );
    
     // run

        Carna::base::view::SceneProvider provider( scene );
        Carna::base::qt::Display* const display
            = new Carna::base::qt::Display( view, provider );    // takes ownership of 'view'

        display->setAttribute( Qt::WA_DeleteOnClose );
        display->show();

        return Carna::base::Application::exec();
    }

};
