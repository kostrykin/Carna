#pragma once

#include <Carna/base/Application.h>
#include <Carna/base/view/SceneProvider.h>
#include <Carna/base/qt/Display.h>

#include <Carna/base/view/Point3D.h>
#include <Carna/base/model/Position.h>
#include "MyVisualization.h"

class MyVisualizationDemo
{

public:

    int exec( Carna::base::model::Scene& scene )
    {
        new Carna::base::view::Point3D( scene, Carna::base::model::Position::fromVolumeUnits( scene, 0.0, 0.0, 0.5 ) );
        new Carna::base::view::Point3D( scene, Carna::base::model::Position::fromVolumeUnits( scene, 0.5, 0.5, 0.5 ) );
        new Carna::base::view::Point3D( scene, Carna::base::model::Position::fromVolumeUnits( scene, 1.0, 1.0, 0.5 ) );

     // create and run

        using namespace Carna::testing;

        Carna::base::view::SceneProvider provider( scene );
        Carna::base::qt::Display* const display
            = new Carna::base::qt::Display( new MyVisualization(), provider );   // takes ownership of 'new MyVisualization'

        display->setAttribute( Qt::WA_DeleteOnClose );
        display->show();

        return Carna::base::Application::exec();
    }

};
