#include <QDebug>

#include <Carna/Version.h>
#include <Carna/base/Application.h>
#include <Carna/base/model/Scene.h>

#include "OpeningDialog.h"
#include "MyVisualizationDemo.h"
#include "MPRDemo.h"
#include "SlicePlaneVisualizationDemo.h"
#include "VolumeVisualizationDemo.h"
#include "OffscreenDisplayDemo.h"
#include "InteractiveOffscreenDisplayDemo.h"
#include "FactorialMaskDemo.h"
#include "BinaryMaskDemo.h"
#include "HistogramExporter.h"
#include "CompressedVolumeExporter.h"

// ----------------------------------------------------------------------------------

CARNA_ASSERT_API_VERSION( EXPECTED_MAJOR_VERSION, EXPECTED_MINOR_VERSION, EXPECTED_RELEASE_VERSION )

// ----------------------------------------------------------------------------------



template< typename Demo >
static int runDemo( unsigned int max_allowed_side_length = 0 )
{
    Demo demo;

 // create data model

    OpeningDialog openingDialog( max_allowed_side_length );
    if( openingDialog.exec() == QDialog::Rejected )
    {
        return 0;
    }
    std::unique_ptr< Carna::base::model::Scene > scene( openingDialog.takeScene() );

 // run demo

    if( scene.get() != nullptr )
    {
        return demo.exec( *scene );
    }
    else
    {
        return 0;
    }
};

// ----------------------------------------------------------------------------------



#include <QInputDialog>
#include <QStringList>

const static QString MODE_INTERACTIVE_BINARY_MASK = "Interactive Binary Mask Test";
const static QString MODE_INTERACTIVE_FACTORIAL_MASK = "Interactive Factorial Mask Test";
const static QString MODE_INTERACTIVE_VOLUME_VISUALIZATION = "Interactive Volume Visualization Test";
const static QString MODE_INTERACTIVE_SLICE_PLANE_VISUALIZATION = "Interactive Slice Plane Visualization Test";
const static QString MODE_INTERACTIVE_MPR = "Interactive MPR Test";
const static QString MODE_INTERACTIVE_CUSTOM_VISUALIZATION = "Interactive 'MyVisualization' Test";
const static QString MODE_OFFSCREEN_DISPLAY_TEST = "Offscreen Display Test";
const static QString MODE_INTERACTIVE_OFFSCREEN_DISPLAY_TEST = "Interactive Offscreen Display Test";
const static QString MODE_HISTOGRAM_EXPORTER = "Histogram Exporter";
const static QString MODE_COMPRESSED_VOLUME_EXPORTER = "Compressed Volume Exporter";



int main( int argc, char** argv )
{
    Carna::base::Application app( argc, argv );

    QStringList modes;
    modes
        << MODE_INTERACTIVE_BINARY_MASK
        << MODE_INTERACTIVE_FACTORIAL_MASK
        << MODE_INTERACTIVE_VOLUME_VISUALIZATION
        << MODE_INTERACTIVE_SLICE_PLANE_VISUALIZATION
        << MODE_INTERACTIVE_MPR
        << MODE_INTERACTIVE_CUSTOM_VISUALIZATION
        << MODE_OFFSCREEN_DISPLAY_TEST
        << MODE_INTERACTIVE_OFFSCREEN_DISPLAY_TEST
        << MODE_HISTOGRAM_EXPORTER
        << MODE_COMPRESSED_VOLUME_EXPORTER;

    bool ok = true;
    const QString selectedMode = QInputDialog::getItem
        ( nullptr
        , "Carna Interactive Testing"
        , "Select test mode:"
        , modes
        , 0
        , false
        , &ok );

    if( !ok )
    {
        return 0;
    }

    qDebug() << "Runinng:" << selectedMode;

    if( selectedMode == MODE_INTERACTIVE_VOLUME_VISUALIZATION )
    {
        return runDemo< VolumeVisualizationDemo >();
    }
    else
    if( selectedMode == MODE_INTERACTIVE_SLICE_PLANE_VISUALIZATION )
    {
        return runDemo< SlicePlaneVisualizationDemo >();
    }
    else
    if( selectedMode == MODE_INTERACTIVE_MPR )
    {
        return runDemo< MPRDemo >();
    }
    else
    if( selectedMode == MODE_INTERACTIVE_CUSTOM_VISUALIZATION )
    {
        return runDemo< MyVisualizationDemo >();
    }
    else
    if( selectedMode == MODE_OFFSCREEN_DISPLAY_TEST )
    {
        return runDemo< OffscreenDisplayDemo >();
    }
    else
    if( selectedMode == MODE_INTERACTIVE_OFFSCREEN_DISPLAY_TEST )
    {
        return runDemo< InteractiveOffscreenDisplayDemo >();
    }
    else
    if( selectedMode == MODE_INTERACTIVE_FACTORIAL_MASK )
    {
        return runDemo< FactorialMaskDemo >();
    }
    if( selectedMode == MODE_INTERACTIVE_BINARY_MASK )
    {
        return runDemo< BinaryMaskDemo >();
    }
    else
    if( selectedMode == MODE_HISTOGRAM_EXPORTER )
    {
        return runDemo< HistogramExporter >();
    }
    else
    if( selectedMode == MODE_COMPRESSED_VOLUME_EXPORTER )
    {
        return runDemo< CompressedVolumeExporter >( 150 );
    }
}
