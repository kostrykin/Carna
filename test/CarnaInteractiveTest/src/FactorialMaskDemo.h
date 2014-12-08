#pragma once

#include <Carna/base/Application.h>
#include <Carna/base/view/SceneProvider.h>
#include <Carna/base/qt/Display.h>

#include <Carna/VolumeRenderings/DefaultVolumeVisualization.h>
#include <Carna/VolumeRenderings/VolumeControllerUI.h>
#include <Carna/VolumeRenderings/DRR/XRay.h>
#include <Carna/VolumeRenderings/MIP/MaximumIntensityProjection.h>
#include <Carna/VolumeRenderings/DVR/DirectVolumeRendering.h>

#include <Carna/base/view/Point3D.h>
#include <Carna/base/model/Position.h>

#include <Carna/base/Composition.h>
#include <Carna/base/model/Volume.h>
#include <Carna/base/model/BufferedMaskAdapter.h>

class FactorialMaskDemo
{

    class FactorialMask : public Carna::base::model::ScalarField< Carna::base::model::BufferedMaskAdapter::FactorialMask::ValueType >
    {

        const Carna::base::Vector3ui size;

    public:

        FactorialMask( const Carna::base::Vector3ui& size )
            : size( size )
        {
        }

        virtual ValueType operator()
            ( unsigned int x
            , unsigned int y
            , unsigned int z ) const override
        {
            if( x < size.x / 2 && y < size.y / 2 )
            {
                return 2.f;
            }
            else
            {
                return 1.f;
            }
        }

    }; // FactorialMask

public:

    int exec( Carna::base::model::Scene& scene )
    {
        Carna::VolumeRenderings::VolumeVisualization* const view = new Carna::VolumeRenderings::DefaultVolumeVisualization();

     // setup model

        new Carna::base::view::Point3D( scene, Carna::base::model::Position::fromVolumeUnits( scene, 0.5, 0.5, 0.5 ) );

        scene.setVolumeMask(
            new Carna::base::model::BufferedMaskAdapter(
                new Carna::base::Composition< Carna::base::model::BufferedMaskAdapter::FactorialMask >(
                    new Carna::base::model::BufferedMaskAdapter::FactorialMask
                        ( scene.volume().size
                        , FactorialMask( scene.volume().size ) ) ) ) );

     // setup renderer

        view->doAfterInitialization( [&]()
            {
                view->renderer().installMode( new Carna::VolumeRenderings::DRR::XRay( scene ) );

             // set initial mode

                view->renderer().setMode( Carna::VolumeRenderings::DRR::XRay::NAME );
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
        display->show();

        return Carna::base::Application::exec();
    }

};
