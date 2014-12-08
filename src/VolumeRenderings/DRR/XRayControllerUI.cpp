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

#include <Carna/VolumeRenderings/DRR/XRayControllerUI.h>
#include <Carna/VolumeRenderings/DRR/XRay.h>
#include <Carna/base/qt/ExpandableGroupBox.h>
#include <Carna/base/Math.h>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QDoubleSpinBox>
#include <limits>

namespace Carna
{

namespace VolumeRenderings
{

namespace DRR
{



// ----------------------------------------------------------------------------------
// XRayControllerUI
// ----------------------------------------------------------------------------------

XRayControllerUI::XRayControllerUI( XRay& view, QWidget* parent )
    : QWidget( parent )
    , view( view )
    , sbWaterAttenuation( new QDoubleSpinBox() )
    , sbBrightness( new QDoubleSpinBox() )
    , sbLowerThreshold( new QSpinBox() )
    , sbUpperThreshold( new QSpinBox() )
    , sbUpperMultiplier( new QDoubleSpinBox() )
    , sbWindowLevel( new QDoubleSpinBox() )
    , sbWindowWidth( new QDoubleSpinBox() )
    , sbMaskContrast( new QDoubleSpinBox() )
    , buReleaseMask( new QPushButton( "Release Mask" ) )
{
    QVBoxLayout* layout = new QVBoxLayout();

 // DRR parameters

    base::qt::ExpandableGroupBox* const gbDRR = new base::qt::ExpandableGroupBox( "DRR Parameters", true );
    QFormLayout* const drr = new QFormLayout();
    gbDRR->child()->setLayout( drr );
    gbDRR->child()->layout()->setContentsMargins( 0, 0, 0, 0 );
    layout->addWidget( gbDRR );

 // water attenuation

    drr->addRow( "Water Attenuation", sbWaterAttenuation );

    sbWaterAttenuation->setRange( 0, std::numeric_limits< float >::max() );
    sbWaterAttenuation->setValue( view.waterAttenuation() );
    sbWaterAttenuation->setSingleStep( 0.1 );

    connect( sbWaterAttenuation, SIGNAL( valueChanged( double ) ), &view, SLOT( setWaterAttenuation( double ) ) );

 // brightness

    drr->addRow( "Intensity", sbBrightness );

    sbBrightness->setRange( 0, std::numeric_limits< float >::max() );
    sbBrightness->setValue( view.brightness() );
    sbBrightness->setDecimals( 2 );
    sbBrightness->setSingleStep( 0.1 );

    connect( sbBrightness, SIGNAL( valueChanged( double ) ), &view, SLOT( setBrightness( double ) ) );

 // threshold filtering / bone enhancement

    base::qt::ExpandableGroupBox* const gbFiltering = new base::qt::ExpandableGroupBox( "Threshold Filtering / Bone Enhancement", true );
    QFormLayout* const filtering = new QFormLayout();
    gbFiltering->child()->setLayout( filtering );
    gbFiltering->child()->layout()->setContentsMargins( 0, 0, 0, 0 );
    layout->addWidget( gbFiltering );

 // lower threshold

    filtering->addRow( "Lower Threshold", sbLowerThreshold );

    sbLowerThreshold->setRange( -1024, 3071 );
    sbLowerThreshold->setValue( view.lowerThreshold() );
    sbLowerThreshold->setSingleStep( 10 );

    connect( sbLowerThreshold, SIGNAL( valueChanged( int ) ), &view, SLOT( setLowerThreshold( int ) ) );

 // upper threshold

    filtering->addRow( "Upper Threshold", sbUpperThreshold );

    sbUpperThreshold->setRange( -1024, 3071 );
    sbUpperThreshold->setValue( view.upperThreshold() );
    sbUpperThreshold->setSingleStep( 10 );

    connect( sbUpperThreshold, SIGNAL( valueChanged( int ) ), &view, SLOT( setUpperThreshold( int ) ) );

 // upper multiplier

    filtering->addRow( "Upper Multiplier", sbUpperMultiplier );

    sbUpperMultiplier->setRange( 0., std::numeric_limits< float >::max() );
    sbUpperMultiplier->setValue( view.upperMultiplier() );
    sbUpperMultiplier->setDecimals( 2 );
    sbUpperMultiplier->setSingleStep( 0.1 );

    connect( sbUpperMultiplier, SIGNAL( valueChanged( double ) ), &view, SLOT( setUpperMultiplier( double ) ) );

 // windowing

    base::qt::ExpandableGroupBox* const gbWindowing = new base::qt::ExpandableGroupBox( "Windowing", false );
    QFormLayout* const windowing = new QFormLayout();
    gbWindowing->child()->setLayout( windowing );
    gbWindowing->child()->layout()->setContentsMargins( 0, 0, 0, 0 );
    layout->addWidget( gbWindowing );

 // window level

    windowing->addRow( "Window Level", sbWindowLevel );

    sbWindowLevel->setRange( 0., std::numeric_limits< float >::max() );
    sbWindowLevel->setValue( view.windowingLevel() );
    sbWindowLevel->setDecimals( 2 );
    sbWindowLevel->setSingleStep( 0.01 );

    connect( sbWindowLevel, SIGNAL( valueChanged( double ) ), &view, SLOT( setWindowingLevel( double ) ) );

 // window width

    windowing->addRow( "Window Width", sbWindowWidth );

    sbWindowWidth->setRange( 0., std::numeric_limits< float >::max() );
    sbWindowWidth->setValue( view.windowingWidth() );
    sbWindowWidth->setDecimals( 2 );
    sbWindowWidth->setSingleStep( 0.01 );

    connect( sbWindowWidth, SIGNAL( valueChanged( double ) ), &view, SLOT( setWindowingWidth( double ) ) );

 // mask control

    base::qt::ExpandableGroupBox* const gbMaskControl = new base::qt::ExpandableGroupBox( "Mask Control", false );
    QFormLayout* const maskControl = new QFormLayout();
    gbMaskControl->child()->setLayout( maskControl );
    gbMaskControl->child()->layout()->setContentsMargins( 0, 0, 0, 0 );
    layout->addWidget( gbMaskControl );

 // mask contrast

    maskControl->addRow( "Contrast", sbMaskContrast );

    sbMaskContrast->setRange( 0, std::numeric_limits< float >::max() );
    sbMaskContrast->setValue( view.maskContrast() );
    sbMaskContrast->setDecimals( 2 );
    sbMaskContrast->setSingleStep( 0.1 );

    connect( sbMaskContrast, SIGNAL( valueChanged( double ) ), &view, SLOT( setMaskContrast( double ) ) );

 // mask acquisition control

    maskControl->addRow( buReleaseMask );
    updateMaskAcquistionControl();

    connect( buReleaseMask, SIGNAL( clicked() ), &view.renderer().provider.scene, SLOT( removeVolumeMask() ) );
    connect( &view.renderer().provider.scene, SIGNAL( volumeMaskExchanged() ), this, SLOT( updateMaskAcquistionControl() ) );

 // respond to view

    connect( &view, SIGNAL( waterAttenuationChanged( double ) ), this, SLOT( setWaterAttenuation( double ) ) );
    connect( &view, SIGNAL( brightnessChanged( double ) ), this, SLOT( setBrightness( double ) ) );
    connect( &view, SIGNAL( lowerThresholdChanged( int ) ), this, SLOT( setLowerThreshold( int ) ) );
    connect( &view, SIGNAL( upperThresholdChanged( int ) ), this, SLOT( setUpperThreshold( int ) ) );
    connect( &view, SIGNAL( upperMultiplierChanged( double ) ), this, SLOT( setUpperMultiplier( double ) ) );
    connect( &view, SIGNAL( windowingLevelChanged( double ) ), this, SLOT( setWindowLevel( double ) ) );
    connect( &view, SIGNAL( windowingWidthChanged( double ) ), this, SLOT( setWindowWidth( double ) ) );
    connect( &view, SIGNAL( maskContrastChanged( double ) ), this, SLOT( setMaskContrast( double ) ) );

 // finish

    layout->addStretch( 1 );
    this->setLayout( layout );
}


void XRayControllerUI::setWaterAttenuation( double mu_water )
{
    if( !base::Math::isEqual( sbWaterAttenuation->value(), mu_water ) )
    {
        sbWaterAttenuation->setValue( mu_water );
    }
}


void XRayControllerUI::setBrightness( double brightness )
{
    if( !base::Math::isEqual( sbBrightness->value(), brightness ) )
    {
        sbBrightness->setValue( brightness );
    }
}


void XRayControllerUI::setLowerThreshold( int lower_threshold )
{
    if( !base::Math::isEqual( sbLowerThreshold->value(), lower_threshold ) )
    {
        sbLowerThreshold->setValue( lower_threshold );
    }
}


void XRayControllerUI::setUpperThreshold( int upper_threshold )
{
    if( !base::Math::isEqual( sbUpperThreshold->value(), upper_threshold ) )
    {
        sbUpperThreshold->setValue( upper_threshold );
    }
}


void XRayControllerUI::setUpperMultiplier( double upper_multiplier )
{
    if( !base::Math::isEqual( sbUpperMultiplier->value(), upper_multiplier ) )
    {
        sbUpperMultiplier->setValue( upper_multiplier );
    }
}


void XRayControllerUI::setWindowLevel( double window_level )
{
    if( !base::Math::isEqual( sbWindowLevel->value(), window_level ) )
    {
        sbWindowLevel->setValue( window_level );
    }
}


void XRayControllerUI::setWindowWidth( double window_width )
{
    if( !base::Math::isEqual( sbWindowWidth->value(), window_width ) )
    {
        sbWindowWidth->setValue( window_width );
    }
}


void XRayControllerUI::setMaskContrast( double mask_contrast )
{
    if( !base::Math::isEqual( sbMaskContrast->value(), mask_contrast ) )
    {
        sbMaskContrast->setValue( mask_contrast );
    }
}


void XRayControllerUI::updateMaskAcquistionControl()
{
    buReleaseMask->setEnabled( view.renderer().provider.scene.hasVolumeMask() );
}



}  // namespace Carna :: VolumeRenderings :: DRR

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna
