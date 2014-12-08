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

#include <Carna/VolumeRenderings/VolumeControllerUI.h>
#include <Carna/VolumeRenderings/VolumeControllerUIDetails.h>
#include <Carna/VolumeRenderings/VolumeRenderer.h>
#include <Carna/VolumeRenderings/VolumeRendererMode.h>
#include <Carna/base/model/Scene.h>
#include <Carna/base/view/SceneProvider.h>
#include <Carna/base/qt/RotationEdit.h>
#include <Carna/base/qt/Object3DChooser.h>
#include <Carna/base/qt/ExpandableGroupBox.h>
#include <Carna/base/qt/FlowLayout.h>
#include <Carna/base/CarnaException.h>
#include <QTabWidget>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QSpinBox>
#include <QComboBox>
#include <limits>
#include <QGroupBox>

namespace Carna
{

namespace VolumeRenderings
{



// ----------------------------------------------------------------------------------
// VolumeControllerUI
// ----------------------------------------------------------------------------------

VolumeControllerUI::VolumeControllerUI( VolumeRenderer& renderer, base::model::Scene& model )
    : pimpl( new Details( renderer, model ) )
{
    CARNA_ASSERT( renderer.modes().empty() || renderer.hasMode() );

    QVBoxLayout* const general = new QVBoxLayout();
    general->setContentsMargins( 0, Details::CONTROLLER_MARGINS_TOP, 0, 0 );

    connect
        ( &renderer
        , SIGNAL( modeChanged( Carna::VolumeRenderings::VolumeRendererMode& ) )
        , pimpl.get()
        , SLOT( updateModeConfiguration( Carna::VolumeRenderings::VolumeRendererMode& ) ) );
    
 // rendering group box

    base::qt::ExpandableGroupBox* const gbRendering = new base::qt::ExpandableGroupBox( "3D Processing", true );
    QFormLayout* const rendering = new QFormLayout();
    gbRendering->child()->setLayout( rendering );
    gbRendering->child()->layout()->setContentsMargins( 0, 0, 0 ,0 );
    general->addWidget( gbRendering );

 // sample count

    QSpinBox* const sbSampleCount = new QSpinBox();

    rendering->addRow( "Samples Per Pixel:", sbSampleCount );

    sbSampleCount->setRange( 2, std::numeric_limits< short >::max() );
    sbSampleCount->setValue( renderer.sampleCount() );
    sbSampleCount->setSingleStep( 10 );

    connect
        ( sbSampleCount
        , SIGNAL( valueChanged( int ) )
        , &renderer
        , SLOT( setSampleCount( int ) ) );

 // quality reduction

    pimpl->cbReduceQuality = new QCheckBox( "Allow Quality Reduction" );
    
    rendering->addRow( pimpl->cbReduceQuality );

    pimpl->syncCameraSettings();

    connect
        ( &renderer
        , SIGNAL( cameraExchanged() )
        , pimpl.get()
        , SLOT( syncCameraSettings() ) );

 // 2D processing group box
    
    QVBoxLayout* const gbFlags_master = new QVBoxLayout();
    base::qt::ExpandableGroupBox* const gbFlags = new base::qt::ExpandableGroupBox( "2D Processing", true );
    gbFlags->child()->setLayout( gbFlags_master );
    gbFlags->child()->layout()->setContentsMargins( 0, 0, 0 ,0 );
    general->addWidget( gbFlags );

    FlowLayout* const gbFlags_flow = new FlowLayout();
    QFormLayout* const gbFlags_form = new QFormLayout();

    gbFlags_master->addLayout( gbFlags_flow );
    gbFlags_master->addLayout( gbFlags_form );

 // mask

    pimpl->cbMask = new QCheckBox( "Circular Mas&k" );
    pimpl->cbMask->setChecked( renderer.hasMask() );
    gbFlags_flow->addWidget( pimpl->cbMask );

    connect
        ( pimpl->cbMask
        , SIGNAL( toggled( bool ) )
        , &renderer
        , SLOT( setMask( bool ) ) );

 // flip horizontal

    pimpl->cbFlipHorizontal = new QCheckBox( "Flip &Horizontal" );
    pimpl->cbFlipHorizontal->setChecked( renderer.isFlippedHorizontal() );
    gbFlags_flow->addWidget( pimpl->cbFlipHorizontal );

    connect
        ( pimpl->cbFlipHorizontal
        , SIGNAL( toggled( bool ) )
        , &renderer
        , SLOT( setFlippedHorizontal( bool ) ) );

 // flip vertical

    pimpl->cbFlipVertical = new QCheckBox( "Flip &Vertical" );
    pimpl->cbFlipVertical->setChecked( renderer.isFlippedVertical() );
    gbFlags_flow->addWidget( pimpl->cbFlipVertical );

    connect
        ( pimpl->cbFlipVertical
        , SIGNAL( toggled( bool ) )
        , &renderer
        , SLOT( setFlippedVertical( bool ) ) );

 // color post processing

    pimpl->cbColorPostProcessing = new QComboBox();
    gbFlags_form->addRow( "Color Post Processing:", pimpl->cbColorPostProcessing );
    pimpl->cbColorPostProcessing->insertItem( VolumeRenderer::noColorOperation, "Disabled" );
    pimpl->cbColorPostProcessing->insertItem( VolumeRenderer::invertBlackAndWhite, "Invert Black & White" );
    pimpl->cbColorPostProcessing->insertItem( VolumeRenderer::invertAllColors, "Invert All Colors" );
    pimpl->cbColorPostProcessing->setCurrentIndex( renderer.colorPostProcessing() );

    connect
        ( pimpl->cbColorPostProcessing
        , SIGNAL( currentIndexChanged( int ) )
        , pimpl.get()
        , SLOT( setColorPostProcessing( int ) ) );

 // rotation 2D

    pimpl->sbRotation2D = new QSpinBox();
    pimpl->sbRotation2D->setValue( renderer.rotation2D() );
    pimpl->sbRotation2D->setMinimum(   0 );
    pimpl->sbRotation2D->setMaximum( 359 );
    pimpl->sbRotation2D->setWrapping( true );
    pimpl->sbRotation2D->setSuffix( "°" );
    gbFlags_form->addRow( "Rotation:", pimpl->sbRotation2D );

    connect
        ( pimpl->sbRotation2D
        , SIGNAL( valueChanged( int ) )
        , &renderer
        , SLOT( setRotation2D( int ) ) );

 // rotation offset

    pimpl->rotationOffset = new base::qt::RotationEdit();
    connect( pimpl->rotationOffset, SIGNAL( rotationChanged( const Carna::base::Transformation& ) ), &renderer, SLOT( setRotationOffset( const Carna::base::Transformation& ) ) );

    base::qt::ExpandableGroupBox* const gbRotationOffset = new base::qt::ExpandableGroupBox( "Rotation Offset", false );
    general->addWidget( gbRotationOffset );

    QFormLayout* const rotationForm = new QFormLayout();
    gbRotationOffset->child()->setLayout( rotationForm );
    gbRotationOffset->child()->layout()->setContentsMargins( 0, 0, 0 ,0 );
    rotationForm->addRow( pimpl->rotationOffset );

    pimpl->rotationOffsetCenter = new base::qt::Object3DChooser( model );
    QWidget* const rotationOffsetCenterController = new QWidget();
    QPushButton* const buSetRotationCenter = new QPushButton( "Set" );
    buSetRotationCenter->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
    rotationOffsetCenterController->setLayout( new QHBoxLayout() );
    rotationOffsetCenterController->layout()->setContentsMargins( 0, 0, 0, 0 );
    rotationOffsetCenterController->layout()->addWidget( pimpl->rotationOffsetCenter );
    rotationOffsetCenterController->layout()->addWidget( buSetRotationCenter );
    rotationForm->addRow( "Center:", rotationOffsetCenterController );

    connect
        ( buSetRotationCenter
        , SIGNAL( clicked() )
        , pimpl.get()
        , SLOT( setRotationCenter() ) );

 // cropping

    base::qt::ExpandableGroupBox* const gbCropping = new base::qt::ExpandableGroupBox( "Cropping", false );
    QFormLayout* const cropping = new QFormLayout();
    gbCropping->child()->setLayout( cropping );
    gbCropping->child()->layout()->setContentsMargins( 0, 0, 0 ,0 );
    general->addWidget( gbCropping );

    QDoubleSpinBox* const sbMinX = new QDoubleSpinBox();
    QDoubleSpinBox* const sbMaxX = new QDoubleSpinBox();
    QDoubleSpinBox* const sbMinY = new QDoubleSpinBox();
    QDoubleSpinBox* const sbMaxY = new QDoubleSpinBox();
    QDoubleSpinBox* const sbMinZ = new QDoubleSpinBox();
    QDoubleSpinBox* const sbMaxZ = new QDoubleSpinBox();

    auto configure_cropping_spinbox = []( QDoubleSpinBox* sb )
    {
        sb->setMinimum( 0 );
        sb->setMaximum( 1 );
        sb->setDecimals( 3 );
        sb->setSingleStep( 0.01 );
    };
    configure_cropping_spinbox( sbMinX );
    configure_cropping_spinbox( sbMaxX );
    configure_cropping_spinbox( sbMinY );
    configure_cropping_spinbox( sbMaxY );
    configure_cropping_spinbox( sbMinZ );
    configure_cropping_spinbox( sbMaxZ );

    cropping->addRow( "x<sub>min</sub>:"  , sbMinX );
    cropping->addRow( "x<sub>max</sub>:" , sbMaxX );
    cropping->addRow( "y<sub>min</sub>:", sbMinY );
    cropping->addRow( "y<sub>max</sub>:"   , sbMaxY );
    cropping->addRow( "z<sub>min</sub>:" , sbMinZ );
    cropping->addRow( "z<sub>max</sub>:"  , sbMaxZ );

    sbMinX->setValue( renderer.minX() );
    sbMaxX->setValue( renderer.maxX() );
    sbMinY->setValue( renderer.minY() );
    sbMaxY->setValue( renderer.maxY() );
    sbMinZ->setValue( renderer.minZ() );
    sbMaxZ->setValue( renderer.maxZ() );

    connect( sbMinX, SIGNAL( valueChanged( double ) ), &renderer, SLOT( setMinX( double ) ) );
    connect( sbMaxX, SIGNAL( valueChanged( double ) ), &renderer, SLOT( setMaxX( double ) ) );
    connect( sbMinY, SIGNAL( valueChanged( double ) ), &renderer, SLOT( setMinY( double ) ) );
    connect( sbMaxY, SIGNAL( valueChanged( double ) ), &renderer, SLOT( setMaxY( double ) ) );
    connect( sbMinZ, SIGNAL( valueChanged( double ) ), &renderer, SLOT( setMinZ( double ) ) );
    connect( sbMaxZ, SIGNAL( valueChanged( double ) ), &renderer, SLOT( setMaxZ( double ) ) );

 // put all together

    if( !renderer.modes().empty() )
    {
        QComboBox* cbMode = new QComboBox();
        for( unsigned int index = 0; index < renderer.modes().size(); ++index )
        {
            cbMode->addItem( QString::fromStdString( renderer.modes()[ index ]->name ), index );
            if( renderer.modes()[ index ] == &renderer.mode() )
            {
                cbMode->setCurrentIndex( index );
            }
        }
        rendering->insertRow( 0, "Mode:", cbMode );

        connect( cbMode, SIGNAL( currentIndexChanged( int ) ), pimpl.get(), SLOT( setModeByIndex( int ) ) );

        pimpl->toolbox = new QTabWidget();
        pimpl->toolbox->setTabPosition( QTabWidget::North );
        pimpl->toolbox->setDocumentMode( true );
        QWidget* generalWidget = new QWidget();
        general->addStretch( 1 );
        generalWidget->setLayout( general );
        pimpl->toolbox->addTab( generalWidget, "General" );

        QVBoxLayout* const master = new QVBoxLayout();
        master->addWidget( pimpl->toolbox );

     // configuration widget of current mode

        QWidget* modeController = renderer.mode().createControllerUI();
        if( modeController )
        {
            if( modeController->layout() )
            {
                modeController->layout()->setContentsMargins( 0, Details::CONTROLLER_MARGINS_TOP, 0, 0 );
            }
            pimpl->mode_cfg_widget_index = pimpl->toolbox->addTab( modeController, QString::fromStdString( renderer.mode().name ) );
        }

     // finish

        this->setLayout( master );
    }
    else
    {
        this->setLayout( general );
    }

    connect
        ( &renderer
        , SIGNAL( destroyed() )
        , this
        , SLOT( deleteLater() ) );
}


VolumeControllerUI::~VolumeControllerUI()
{
    if( pimpl->mode_cfg_widget_index != -1 )
    {
        pimpl->toolbox->removeTab( pimpl->mode_cfg_widget_index );
    }
}



}  // namespace Carna :: VolumeRenderings

}  // namespace Carna
