#include "OffscreenDisplayDemo.h"

#include <Carna/base/Application.h>
#include <Carna/base/view/SceneProvider.h>

#include <Carna/base/OffscreenDisplay.h>
#include <Carna/base/OffscreenVisualizationEnvironment.h>

#include <Carna/VolumeRenderings/VolumeRenderer.h>
#include <Carna/VolumeRenderings/DefaultVolumeVisualization.h>
#include <Carna/VolumeRenderings/DRR/XRay.h>
#include <QLabel>


int OffscreenDisplayDemo::exec( Carna::base::model::Scene& scene )
{
    Carna::VolumeRenderings::VolumeVisualization* visualization = new Carna::VolumeRenderings::DefaultVolumeVisualization();
        visualization->doAfterInitialization( [&]()
        {
            visualization->renderer().installMode( new Carna::VolumeRenderings::DRR::XRay( scene ) );
            visualization->renderer().setMode( Carna::VolumeRenderings::DRR::XRay::NAME );
        }
    );

    Carna::base::view::SceneProvider provider( scene );
    Carna::base::OffscreenVisualizationEnvironment enviroment;
    Carna::base::OffscreenDisplay display( enviroment, 500, 300, visualization, provider );

    QLabel window;
    window.setPixmap( QPixmap::fromImage( display.framebuffer() ) );
    window.setFixedSize( display.width(), display.height() );
    window.show();

    return Carna::base::Application::exec();
}
