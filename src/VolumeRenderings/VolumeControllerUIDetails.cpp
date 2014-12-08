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

#include <Carna/VolumeRenderings/VolumeControllerUIDetails.h>
#include <Carna/VolumeRenderings/VolumeRendererMode.h>
#include <Carna/VolumeRenderings/VolumeRenderer.h>
#include <Carna/VolumeRenderings/VolumeVisualization.h>
#include <Carna/VolumeRenderings/VolumeVisualizationCameraController.h>
#include <Carna/base/model/Object3D.h>
#include <Carna/base/model/Position.h>
#include <Carna/base/qt/Object3DChooser.h>
#include <Carna/base/view/SceneProvider.h>
#include <Carna/base/view/DefaultCamera.h>
#include <Carna/base/CarnaException.h>
#include <QLayout>
#include <QTabWidget>
#include <QCheckBox>

namespace Carna
{

namespace VolumeRenderings
{



// ----------------------------------------------------------------------------------
// VolumeControllerUI :: Details
// ----------------------------------------------------------------------------------

VolumeControllerUI::Details::Details( VolumeRenderer& renderer, base::model::Scene& model )
    : model( model )
    , renderer( renderer )
    , mode_cfg_widget_index( -1 )
{
    connect
        ( &renderer.module()
        , SIGNAL( controllerExchanged() )
        , this
        , SLOT( syncCameraSettings() ) );
}


void VolumeControllerUI::Details::updateModeConfiguration( VolumeRendererMode& mode )
{
    if( mode_cfg_widget_index == -1 )
    {

        // Controller does not support mode configuration. This happens when the controller
        // is created _before_ volume rendering modes have been associated to the renderer.

        return;

    }

 // ----------------------------------------------------------------------------------

    QWidget* const modeController = mode.createControllerUI();
    if( modeController->layout() )
    {
        modeController->layout()->setContentsMargins( 0, CONTROLLER_MARGINS_TOP, 0, 0 );
    }

    if( mode_cfg_widget_index != -1 )
    {
        toolbox->removeTab( mode_cfg_widget_index );
    }
    if( modeController )
    {
        mode_cfg_widget_index = toolbox->addTab( modeController, QString::fromStdString( mode.name ) );
    }
    else
    {
        mode_cfg_widget_index = -1;
    }
}


void VolumeControllerUI::Details::setModeByIndex( int index )
{
    CARNA_ASSERT( index >= 0 );
    CARNA_ASSERT( static_cast< unsigned int >( index ) < renderer.modes().size() );

    renderer.setMode( *( renderer.modes()[ static_cast< unsigned int >( index ) ] ) );
}


void VolumeControllerUI::Details::setRotationCenter()
{
    if( rotationOffsetCenter->isObject3DSelected() )
    {
        renderer.setRotationCenter( rotationOffsetCenter->selectedObject3D().position().toVolumeUnits() );
    }
    else
    {
        renderer.setRotationCenter( base::Vector( 0.5, 0.5, 0.5 ) );
    }
}


void VolumeControllerUI::Details::syncCameraSettings()
{
    base::controller::DefaultInteractionStrategy* const controller = dynamic_cast< base::controller::DefaultInteractionStrategy* >( &renderer.module().rawController() );
    if( controller != nullptr )
    {
        connect
            ( controller
            , SIGNAL( cameraControllerExchanged() )
            , this
            , SLOT( syncCameraSettings() ) );

        VolumeVisualizationCameraController* const cameraController = dynamic_cast< VolumeVisualizationCameraController* >( &controller->cameraController() );
        if( cameraController != nullptr )
        {
            cbReduceQuality->setEnabled( true );

            if( cameraController->qualityReduction() != VolumeVisualizationCameraController::NO_QUALITY_REDUCTION )
            {
                cbReduceQuality->setChecked( true );
            }

            connect( cbReduceQuality, SIGNAL( clicked( bool ) ), this, SLOT( setCameraQualityReduction( bool ) ) );

            return;
        }
    }

    cbReduceQuality->setEnabled( false );
}


void VolumeControllerUI::Details::setCameraQualityReduction( bool allowQualityReduction )
{
    base::controller::DefaultInteractionStrategy* const controller = dynamic_cast< base::controller::DefaultInteractionStrategy* >( &renderer.module().rawController() );
    if( controller != nullptr )
    {
        VolumeVisualizationCameraController* const cameraController = dynamic_cast< VolumeVisualizationCameraController* >( &controller->cameraController() );
        if( cameraController != nullptr )
        {
            cameraController->setQualityReduction
                ( allowQualityReduction
                ? VolumeVisualizationCameraController::DEFAULT_QUALITY_REDUCTION
                : VolumeVisualizationCameraController::NO_QUALITY_REDUCTION );
        }
    }
}


void VolumeControllerUI::Details::setColorPostProcessing( int colorPostProcessing )
{
    renderer.setColorPostProcessing( static_cast< VolumeRenderer::ColorOperation >( colorPostProcessing ) );
}



}  // namespace Carna :: VolumeRenderings

}  // namespace Carna
