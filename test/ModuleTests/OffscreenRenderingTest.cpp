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

#include "OffscreenRenderingTest.h"
#include "FramebufferTester.h"
#include <Carna/base/Scene.h>
#include <Carna/base/SceneProvider.h>
#include <Carna/base/OffscreenDisplay.h>
#include <Carna/base/OffscreenVisualizationEnvironment.h>
#include <MyVisualization.h>
#include <MockSceneFactory.h>
#include <HUGZSceneFactory.h>
#include <QImage>
#include <QColor>



// ----------------------------------------------------------------------------------
// OffscreenRenderingTest
// ----------------------------------------------------------------------------------

void OffscreenRenderingTest::initTestCase()
{
    disableQDebug();

    try
    {
        const MockSceneFactory::Dimensions modelSize = MockSceneFactory::Dimensions::fromCentimeters( 10, 10, 10 );
        const double modelResolution = 0.5;

        scene.reset( MockSceneFactory( modelSize, modelResolution ).createMockScene() );
        provider.reset( new Carna::base::SceneProvider( *scene ) );
        environment.reset( new Carna::base::OffscreenVisualizationEnvironment() );
    }
    catch( const Carna::base::AssertionFailure& ex )
    {
        QFAIL( ex.details.c_str() );
    }
}


void OffscreenRenderingTest::cleanupTestCase()
{
    try
    {
        environment.reset();
        provider.reset();
        scene.reset();
    }
    catch( const Carna::base::AssertionFailure& ex )
    {
        QFAIL( ex.details.c_str() );
    }
}


void OffscreenRenderingTest::init()
{
    displayWidth = 400;
    displayHeight = 400;

    try
    {
        module = new MyVisualization();
        display.reset( new Carna::base::OffscreenDisplay( *environment, displayWidth, displayHeight, module, *provider ) );
    }
    catch( const Carna::base::AssertionFailure& ex )
    {
        QFAIL( ex.details.c_str() );
    }
}


void OffscreenRenderingTest::cleanup()
{
    module = nullptr;
    display.reset();
}


void OffscreenRenderingTest::testModule()
{
    QCOMPARE( &display->module(), module );
    QCOMPARE( &static_cast< const Carna::base::OffscreenDisplay* >( display.get() )->module(), module );
}


void OffscreenRenderingTest::testPaint()
{
    display->paint();
    verifyFramebufferSize();
    VERIFY_FRAMEBUFFER( *display, "OffscreenRenderingTest_testPaint.png" );
}


void OffscreenRenderingTest::testWidth()
{
    QCOMPARE( static_cast< signed >( display->width() ), displayWidth );
}


void OffscreenRenderingTest::testHeight()
{
    QCOMPARE( static_cast< signed >( display->height() ), displayHeight );
}


void OffscreenRenderingTest::testIsInvalidated()
{
    QCOMPARE( display->isInvalidated(), true );

    display->paint();

    QCOMPARE( display->isInvalidated(), false );
}


void OffscreenRenderingTest::testTextureID()
{
    QVERIFY( display->textureID() > 0 );
}


void OffscreenRenderingTest::testInvalidate()
{
    display->paint();

    QCOMPARE( display->isInvalidated(), false );

    display->invalidate();

    QCOMPARE( display->isInvalidated(), true );
}


void OffscreenRenderingTest::testMinimalRepainting()
{
    /**************************************************************************************************
     *                                       S  R  A  T  E  G  Y                                      *
     /*************************************************************************************************
     *
     *
     *  1. The renderer is triggered by the 'framebuffer' invocation.
     *  2. The mutable framebuffer image is overwritten by gray shaded color.
     *  3. The 'framebuffer' method is invoked again, but this time it must not trigger the renderer.
     *  4. This is verified by comparing each pixel to the previously set color.
     *  5. The size of the image referenced by 'framebuffer' is verified.
     *
     */

    const QColor color( 100, 100, 100 );

    display->framebuffer().fill( color );

    for( int x = 0; x < display->framebuffer().width(); ++x )
    for( int y = 0; y < display->framebuffer().height(); ++y )
    {
        QCOMPARE( display->framebuffer().pixel( x, y ), color.rgb() );
    }

    verifyFramebufferSize();
}


void OffscreenRenderingTest::testFramebufferImageAddressConsistency()
{
    QImage* lastFramebuffer = nullptr;
    for( unsigned int i = 0; i < 5; ++i )
    {
        QImage* const currentFramebuffer = &display->framebuffer();

        if( lastFramebuffer != nullptr )
        {
            QCOMPARE( currentFramebuffer, lastFramebuffer );
        }

        lastFramebuffer = currentFramebuffer;
        display->invalidate();
    }
}


void OffscreenRenderingTest::testResize()
{
    displayWidth = displayWidth * 2;
    displayHeight = displayHeight * 2;

    display->resize( displayWidth, displayHeight );

    QCOMPARE( display->isInvalidated(), true );

    // (1) Verify the width has been updated properly.

    testWidth();

    // (2) Verify the height has been updated properly.

    testHeight();

    // (3) Verify the size consistency of the grabbed framebuffer image.

    verifyFramebufferSize();
}


void OffscreenRenderingTest::verifyFramebufferSize()
{
    QCOMPARE( display->framebuffer().width(), displayWidth );
    QCOMPARE( display->framebuffer().height(), displayHeight );
}


void OffscreenRenderingTest::verifyAnotherScene
    ( unsigned int scene_width
    , unsigned int scene_height
    , unsigned int scene_depth
    , double voxels_per_mm
    , unsigned int display_width
    , unsigned int display_height
    , const std::string& expected_filename )
{
    const MockSceneFactory::Dimensions model_size = MockSceneFactory::Dimensions::fromCentimeters
        ( scene_width
        , scene_height
        , scene_depth );

    const Carna::base::Scene* const scene2_ptr = MockSceneFactory( model_size, voxels_per_mm ).createMockScene();
    verifyAnotherScene( scene2_ptr, display_width, display_height, expected_filename );
}


void OffscreenRenderingTest::verifyAnotherScene
    ( const std::string& hugz_filename
    , unsigned int display_width
    , unsigned int display_height
    , const std::string& expected_filename )
{
    const Carna::base::Scene* const scene2_ptr = HUGZSceneFactory::importVolume( hugz_filename );
    verifyAnotherScene( scene2_ptr, display_width, display_height, expected_filename );
}


void OffscreenRenderingTest::verifyAnotherScene
    ( const Carna::base::Scene* const scene_ptr
    , unsigned int display_width
    , unsigned int display_height
    , const std::string& expected_filename )
{
    try
    {
        const Carna::base::Scene& scene2 = *scene_ptr;
        Carna::base::SceneProvider provider2( scene2 );
        Carna::base::OffscreenVisualizationEnvironment environment2;

        MyVisualization* const module2 = new MyVisualization();
        Carna::base::OffscreenDisplay display2( environment2, display_width, display_height, module2, provider2 );
        display2.paint();

        VERIFY_FRAMEBUFFER( display2, expected_filename );
    }
    catch( const Carna::base::AssertionFailure& ex )
    {
        QFAIL( ex.details.c_str() );
    }
}


void OffscreenRenderingTest::testMultipleInstances()
{
    testPaint();

    verifyAnotherScene
        ( MockSceneFactory( Carna::base::Vector3ui( 150, 150, 20 ), 0.2, 0.2, 0.9 ).createMockScene()
        , 400, 400      //< display size
        , "OffscreenRenderingTest_testMultipleInstances_150_150_20_0.2_0.2_0.9.png" );

    verifyAnotherScene
        ( SOURCE_PATH + "/res/pelves_reduced.hugz"
        , 400, 400      //< display size
        , "OffscreenRenderingTest_testMultipleInstances_pelves_reduced.png" );

    verifyAnotherScene
        ( 10, 10, 10    //< centimeters
        , 4             //< voxels per millimeter
        , 400, 400      //< display size
        , "OffscreenRenderingTest_testMultipleInstances_10_10_10_4.png" );

    testPaint();
}
