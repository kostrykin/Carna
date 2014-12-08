#pragma once

#include <Carna/base/Application.h>
#include <Carna/base/view/SceneProvider.h>
#include <Carna/base/qt/Display.h>

#include <Carna/MPR/DefaultAxialPlaneVisualization.h>
#include <Carna/MPR/AxialPlaneControllerUI.h>
#include <Carna/MPR/IntersectionSynchronisationFactory.h>

class MPRDemo
{

    unsigned int initializedVisualizationsCount;
    std::vector< Carna::MPR::AxialPlaneVisualization* > views;
    Carna::MPR::AxialPlaneControllerUI* controller_ui;

public:

    MPRDemo() : views( 4 )
    {
    }


    int exec( Carna::base::model::Scene& scene )
    {
        controller_ui = new Carna::MPR::AxialPlaneControllerUI( scene );

        for( unsigned int i = 0; i < 4; ++i )
        {
            views[ i ] = new Carna::MPR::DefaultAxialPlaneVisualization( scene );
        }

     // configure
    
        views[ 0 ]->makeFrontView();

        views[ 1 ]->makeLeftView();

        views[ 2 ]->makeTopView();

        views[ 3 ]->makeRotatableView();

     // run

        initializedVisualizationsCount = 0;
        Carna::base::view::SceneProvider provider( scene );
        for( unsigned int i = 0; i < 4; ++i )
        {
            Carna::base::qt::Display* const display
                = new Carna::base::qt::Display( views[ i ], provider );  // takes ownership of 'views[ i ]'

            display->setAttribute( Qt::WA_DeleteOnClose );
            display->resize( 200, 200 );
            display->show();

            views[ i ]->doAfterInitialization( [&]()
                {
                    if( ++initializedVisualizationsCount == 4 )
                    {
                        setupRenderers();
                    }
                }
            );
        }

        return Carna::base::Application::exec();
    }


private:

    void setupRenderers()
    {
        Carna::MPR::IntersectionSynchronisationFactory intersection_sync;

        for( unsigned int i = 0; i < 4; ++i )
        {
            intersection_sync.addRenderer( views[ i ]->renderer() );
            controller_ui->addRenderer( views[ i ]->renderer() );
        }

        intersection_sync.create();

        controller_ui->setDeletedWhenEmpty( true );
        controller_ui->setAttribute( Qt::WA_DeleteOnClose );
        controller_ui->show();
    }

};
