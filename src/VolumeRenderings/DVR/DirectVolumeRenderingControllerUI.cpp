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

#include <Carna/VolumeRenderings/DVR/DirectVolumeRenderingControllerUI.h>
#include <Carna/VolumeRenderings/DVR/DirectVolumeRendering.h>
#include <Carna/VolumeRenderings/DVR/ColorMapEditor.h>
#include <Carna/base/qt/Object3DChooser.h>
#include <Carna/base/qt/ExpandableGroupBox.h>
#include <Carna/base/CarnaException.h>
#include <Carna/base/Visualization.h>
#include <Carna/base/VisualizationEnvironment.h>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QFormLayout>
#include <QPushButton>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QTextStream>
#include <QMouseEvent>
#include <QDebug>
#include <QFileDialog>

namespace Carna
{

namespace VolumeRenderings
{

namespace DVR
{



// ----------------------------------------------------------------------------------
// DVR :: DirectVolumeRenderingControllerUI
// ----------------------------------------------------------------------------------

DirectVolumeRenderingControllerUI::DirectVolumeRenderingControllerUI( DirectVolumeRendering& mode )
    : mode( mode )
    , colorMapEditor( new ColorMapEditor() )
    , cbLighting( new QCheckBox( "Enabled" ) )
    , sbLightAmplitude( new QDoubleSpinBox() )
    , sbLightBias( new QDoubleSpinBox() )
    , sbLightBackFaceAmount( new QDoubleSpinBox() )
    , sbPickingSaturation( new QDoubleSpinBox() )
    , cbPickingRepresentation( new base::qt::Object3DChooser( mode.mutableModel() ) )
{
    colorMapEditor->setFirst( -1024 );
    colorMapEditor->setLast( 3071 );
    colorMapEditor->setTickInterval( 200 );
    colorMapEditor->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::MinimumExpanding );

 // fetch color map

    for( auto it = mode.colorMap().data().begin(); it != mode.colorMap().data().end(); ++it )
    {
        const ColorMap::HUSpan& huRange = it->first;
        const ColorMap::ColorSpan& colorGradient = it->second;

     // create span

        const unsigned int spanIndex = colorMapEditor->addSpan( base::IntSpan( huRange.first, huRange.last ) );

     // set colors

        colorMapEditor->getFirstColor( spanIndex ) = colorGradient.first;
        colorMapEditor->getLastColor( spanIndex ) = colorGradient.last;
    }

 // create remaining UI elements

    sbLightAmplitude->setMinimum( 1. );
    sbLightAmplitude->setMaximum( std::numeric_limits< float >::max() );
    sbLightAmplitude->setSingleStep( 0.1 );
    sbLightAmplitude->setValue( mode.getLightAmplitude() );

    sbLightBias->setMinimum( 0 );
    sbLightBias->setMaximum( 1 );
    sbLightBias->setSingleStep( 0.05 );
    sbLightBias->setValue( mode.getLightBias() );

    sbLightBackFaceAmount->setMinimum( 0 );
    sbLightBackFaceAmount->setMaximum( 1 );
    sbLightBackFaceAmount->setSingleStep( 0.1 );
    sbLightBackFaceAmount->setValue( mode.getLightBackFaceAmount() );

    base::qt::ExpandableGroupBox* const gbLight = new base::qt::ExpandableGroupBox( "Lighting", false );
    QFormLayout* const light = new QFormLayout();
    gbLight->child()->setLayout( light );
    gbLight->child()->layout()->setContentsMargins( 0, 0, 0 ,0 );

    cbLighting->setCheckState( mode.hasLighting() ? Qt::Checked : Qt::Unchecked );

    light->addRow( cbLighting );
    light->addRow( "Amplitude:", sbLightAmplitude );
    light->addRow( "Bias:", sbLightBias );
    light->addRow( "Back-face Amount:", sbLightBackFaceAmount );

    QHBoxLayout* const bottom_buttons = new QHBoxLayout();
    QPushButton* const buSaveColorConfig = new QPushButton( "Save" );
    QPushButton* const buLoadColorConfig = new QPushButton( "Load" );
    bottom_buttons->addWidget( buSaveColorConfig );
    bottom_buttons->addWidget( buLoadColorConfig );
    bottom_buttons->addStretch( 1 );

    base::qt::ExpandableGroupBox* const gbPicking = new base::qt::ExpandableGroupBox( "Depth Occultation & Object Positioning", false );
    QFormLayout* const picking = new QFormLayout();
    gbPicking->child()->setLayout( picking );
    gbPicking->child()->layout()->setContentsMargins( 0, 0, 0 ,0 );

    sbPickingSaturation->setMinimum( 0 );
    sbPickingSaturation->setMaximum( 1 );
    sbPickingSaturation->setSingleStep( 0.1 );
    sbPickingSaturation->setValue( mode.getPickingSaturation() );

    if( mode.hasPositionedObject() )
    {
        cbPickingRepresentation->selectObject( mode.getPositionedObject() );
    }
    else
    {
        cbPickingRepresentation->selectNone();
    }

    picking->addRow( "Solid Surface Threshold:", sbPickingSaturation );
    picking->addRow( "Put in Place on CTRL:", cbPickingRepresentation );

    QVBoxLayout* master = new QVBoxLayout();
    master->addWidget( gbPicking );
    master->addWidget( gbLight );
    master->addWidget( colorMapEditor );
    master->addLayout( bottom_buttons );

    this->setLayout( master );

    connect( sbLightAmplitude, SIGNAL( valueChanged( double ) ), &mode, SLOT( setLightAmplitude( double ) ) );
    connect( sbLightBias, SIGNAL( valueChanged( double ) ), &mode, SLOT( setLightBias( double ) ) );
    connect( sbLightBackFaceAmount, SIGNAL( valueChanged( double ) ), &mode, SLOT( setLightBackFaceAmount( double ) ) );
    connect( cbLighting, SIGNAL( toggled( bool ) ), &mode, SLOT( setLighting( bool ) ) );
    connect( cbLighting, SIGNAL( clicked() ), this, SLOT( updateLightingUI() ) );

    connect( sbPickingSaturation, SIGNAL( valueChanged( double ) ), &mode, SLOT( setPickingSaturation( double ) ) );
    connect( cbPickingRepresentation, SIGNAL( selectionChanged( Carna::base::model::Object3D& ) ), &mode, SLOT( setPositionedObject( Carna::base::model::Object3D& ) ) );
    connect( cbPickingRepresentation, SIGNAL( selectionDissolved() ), &mode, SLOT( removePositionedObject() ) );

    connect( colorMapEditor, SIGNAL( spansChanged( Carna::base::qt::MultiSpanSlider& ) ), this, SLOT( updateColorMap() ) ); // TODO: check whether this works!
    
    connect( buSaveColorConfig, SIGNAL( clicked() ), this, SLOT( saveColorConfig() ) );
    connect( buLoadColorConfig, SIGNAL( clicked() ), this, SLOT( loadColorConfig() ) );

    this->updateLightingUI();
}


DirectVolumeRenderingControllerUI::~DirectVolumeRenderingControllerUI()
{
}


void DirectVolumeRenderingControllerUI::updateColorMap()
{
    mode.colorMap().clear();

    for( unsigned int spanIndex = 0; spanIndex < colorMapEditor->getSpanCount(); ++spanIndex )
    {
        const base::IntSpan& span = colorMapEditor->getSpan( spanIndex );

        const ColorMap::HUSpan huRange
            ( static_cast< short >( span.first )
            , static_cast< short >( span.last ) );

        const ColorMap::ColorSpan colorGradient
            ( colorMapEditor->getFirstColor( spanIndex )
            , colorMapEditor->getLastColor( spanIndex ) );

        mode.colorMap().put( huRange, colorGradient );
    }
}


void DirectVolumeRenderingControllerUI::updateLightingUI()
{
    const bool enabled = cbLighting->isChecked();

    sbLightAmplitude->setEnabled( enabled );
    sbLightBias->setEnabled( enabled );
    sbLightBackFaceAmount->setEnabled( enabled );
}


void DirectVolumeRenderingControllerUI::saveColorConfig()
{
    const QString filename = QFileDialog::getSaveFileName
        ( this
        , "Save Color Map"
        , ""
        , "XML-Files (*.XML)"
        , 0
        , QFileDialog::DontResolveSymlinks
        | QFileDialog::HideNameFilterDetails );

    if( filename.isEmpty() )
    {
        return;
    }

    QDomDocument dom;
    dom.createProcessingInstruction( "xml", "version=\"1.0\" encoding=\"utf-8\"" );

    QDomElement root = dom.createElement( "AbsorptionColorTable" );
    dom.appendChild( root );

    root.setAttribute( "lighting", mode.hasLighting() ? "on" : "none" );
    root.setAttribute( "amplitude", mode.getLightAmplitude() );
    root.setAttribute( "bias", mode.getLightBias() );
    root.setAttribute( "back", mode.getLightBackFaceAmount() );

 // see: http://www.digitalfanatics.org/projects/qt_tutorial/chapter09.html

    for( unsigned int i = 0; i < colorMapEditor->getSpanCount(); ++i )
    {
        const base::IntSpan& span = colorMapEditor->getSpan( i );
        const QColor& color0 = colorMapEditor->getFirstColor( i );
        const QColor& color1 = colorMapEditor->getLastColor( i );

        QDomElement node = dom.createElement( "Channel" );

        node.setAttribute( "minHuv", span.first );
        node.setAttribute( "maxHuv", span.last  );

        node.setAttribute( "minColorR", color0.red  () );
        node.setAttribute( "minColorG", color0.green() );
        node.setAttribute( "minColorB", color0.blue () );
        node.setAttribute( "minColorA", color0.alpha() );

        node.setAttribute( "maxColorR", color1.red  () );
        node.setAttribute( "maxColorG", color1.green() );
        node.setAttribute( "maxColorB", color1.blue () );
        node.setAttribute( "maxColorA", color1.alpha() );

        root.appendChild( node );
    }

    QFile file( filename );
    if( !file.open( QIODevice::WriteOnly | QIODevice::Truncate ) )
    {
        throw base::CarnaException
            ( "Save Color Map"
            , "Failed opening file for writing."
            , ( "Filename: '" + filename + "'" ).toStdString() );
    }

    QTextStream stream( &file );
    stream << dom.toString();

    file.close();
}


void DirectVolumeRenderingControllerUI::loadColorConfig()
{
    const QString filename = QFileDialog::getOpenFileName
        ( this
        , "Load Color Map"
        , ""
        , "XML-Files (*.xml)"
        , 0
        , QFileDialog::ReadOnly
        | QFileDialog::HideNameFilterDetails );

    if( filename.isEmpty() )
    {
        return;
    }

    QFile file( filename );
    if( !file.open( QIODevice::ReadOnly ) )
    {
        throw base::CarnaException
            ( "Load Color Map"
            , "Failed opening file for reading."
            , ( "Filename: '" + filename + "'" ).toStdString() );
    }
    
    colorMapEditor->removeAllSpans();

    struct unexpected_format_exception { };
    try
    {

        QDomDocument dom;
        if( !dom.setContent( &file ) )
        {
            throw unexpected_format_exception();
        }

        QDomElement root = dom.documentElement();
        if( root.tagName() != "AbsorptionColorTable" )
        {
            throw unexpected_format_exception();
        }

     // read lighting settings

        cbLighting->setChecked( root.attribute( "lighting", mode.hasLighting() ? "on" : "none" ).toLower() == "on" ? Qt::Checked : Qt::Unchecked );
        sbLightAmplitude->setValue( root.attribute( "amplitude", QString::number( mode.getLightAmplitude() ) ).toFloat() );
        sbLightBias->setValue( root.attribute( "bias", QString::number( mode.getLightBias() ) ).toFloat() );
        sbLightBackFaceAmount->setValue( root.attribute( "back", QString::number( mode.getLightBackFaceAmount() ) ).toFloat() );

        updateLightingUI();

     // read color map

        QDomNode n = root.firstChild();
        while( !n.isNull() )
        {
            QDomElement e = n.toElement();
            if( !e.isNull() )
            {
                if( e.tagName() != "Channel" )
                {
                    throw unexpected_format_exception();
                }

                const int huv0 = e.attribute( "minHuv" ).toInt();
                const int huv1 = e.attribute( "maxHuv" ).toInt();

                const int r0 = e.attribute( "minColorR" ).toInt();
                const int g0 = e.attribute( "minColorG" ).toInt();
                const int b0 = e.attribute( "minColorB" ).toInt();
                const int a0 = e.attribute( "minColorA" ).toInt();

                const int r1 = e.attribute( "maxColorR" ).toInt();
                const int g1 = e.attribute( "maxColorG" ).toInt();
                const int b1 = e.attribute( "maxColorB" ).toInt();
                const int a1 = e.attribute( "maxColorA" ).toInt();
    
                const int span = colorMapEditor->addSpan( base::IntSpan( huv0,  huv1 ) );

                colorMapEditor->getFirstColor( span ) = QColor( r0, g0, b0, a0 );
                colorMapEditor->getLastColor ( span ) = QColor( r1, g1, b1, a1 );
            }

            n = n.nextSibling();
        }
    }
    catch( const unexpected_format_exception& )
    {
        throw base::CarnaException
            ( "Load Color Map"
            , "Unexpected file format." );
    }

    this->updateColorMap();
}



}  // namespace Carna :: VolumeRenderings :: DVR

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna
