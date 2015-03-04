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

#include "VolumeRendererTest.h"
#include "FramebufferTester.h"
#include <HUGZSceneFactory.h>
#include <Carna/VolumeRenderings/DefaultVolumeRenderer.h>
#include <Carna/VolumeRenderings/DefaultVolumeVisualization.h>
#include <Carna/VolumeRenderings/DRR/XRay.h>
#include <Carna/VolumeRenderings/MIP/MaximumIntensityProjection.h>
#include <Carna/VolumeRenderings/DVR/DirectVolumeRendering.h>
#include <Carna/VolumeRenderings/DVR/ColorMap.h>
#include <Carna/base/OffscreenDisplay.h>
#include <Carna/base/OffscreenVisualizationEnvironment.h>
#include <Carna/base/Point3D.h>



// ----------------------------------------------------------------------------------
// VolumeRendererTest
// ----------------------------------------------------------------------------------

void VolumeRendererTest::initTestCase()
{
    disableQDebug();

    if( Carna::base::SceneProvider::max3DTextureSize() < 512 )
    {
        QFAIL( "GL_MAX_3D_TEXTURE_SIZE is suspiciously low. Verify that you are running the test suite on the correct video device." );
    }

    try
    {
        scene.reset( HUGZSceneFactory::importVolume( SOURCE_PATH + "/res/pelves_reduced.hugz" ) );
        provider.reset( new Carna::base::SceneProvider( *scene ) );
        environment.reset( new Carna::base::OffscreenVisualizationEnvironment() );

        new Carna::base::Point3D( *scene, Carna::base::Position::fromVolumeUnits( *scene, 0.5, 0.5, 0.5 ) );
    }
    catch( const Carna::base::AssertionFailure& ex )
    {
        QFAIL( ex.details.c_str() );
    }
}


void VolumeRendererTest::cleanupTestCase()
{
    environment.reset();
    provider.reset();
    scene.reset();
}


void VolumeRendererTest::init()
{
    try
    {
        module = new Carna::VolumeRenderings::DefaultVolumeVisualization();
        display.reset( new Carna::base::OffscreenDisplay( *environment, 512, 512, module, *provider ) );
    }
    catch( const Carna::base::AssertionFailure& ex )
    {
        QFAIL( ex.details.c_str() );
    }
}


void VolumeRendererTest::cleanup()
{
    module = nullptr;
    display.reset();
}


static void setupVolumeRendererState( Carna::VolumeRenderings::VolumeRenderer& renderer )
{
    renderer.setSampleCount( 213 );
    renderer.setColorPostProcessing( Carna::VolumeRenderings::VolumeRenderer::noColorOperation );
    renderer.setFlippedHorizontal( false );
    renderer.setFlippedVertical( false );
    renderer.setMask( false );
    renderer.setMinX( 0 );
    renderer.setMinY( 0 );
    renderer.setMinZ( 0 );
    renderer.setMaxX( 1 );
    renderer.setMaxY( 1 );
    renderer.setMaxZ( 1 );
    renderer.setRotation2D( 0 );
    renderer.setRotationOffset( Carna::base::Transformation() );
}


void VolumeRendererTest::testDRR()
{
    module->doAfterInitialization( [&]()
        {
            auto const drr = new Carna::VolumeRenderings::DRR::XRay( *scene );
            module->renderer().installMode( drr );
            module->renderer().setMode( Carna::VolumeRenderings::DRR::XRay::NAME );
            setupVolumeRendererState( module->renderer() );

            drr->setBrightness( 0.6 );
            drr->setLowerThreshold( 0 );
            drr->setUpperMultiplier( 1.5 );
            drr->setUpperThreshold( 400 );
            drr->setWaterAttenuation( 2 );
            drr->setWindowingLevel( 0.5 );
            drr->setWindowingWidth( 1.0 );
        }
    );
    display->paint();
    VERIFY_FRAMEBUFFER( *display, "pelves_reduced_drr_213spp_perspective.png" );
}


void VolumeRendererTest::testMIP()
{
    module->doAfterInitialization( [&]()
        {
            auto const mip = new Carna::VolumeRenderings::MIP::MaximumIntensityProjection();
            module->renderer().installMode( mip );
            module->renderer().setMode( Carna::VolumeRenderings::MIP::MaximumIntensityProjection::NAME );
            setupVolumeRendererState( module->renderer() );
        }
    );
    display->paint();
    VERIFY_FRAMEBUFFER( *display, "pelves_reduced_mip_213spp_orthogonal.png" );
}


void VolumeRendererTest::testDVR()
{
    module->doAfterInitialization( [&]()
    {
        auto const dvr = new Carna::VolumeRenderings::DVR::DirectVolumeRendering( *scene );
        module->renderer().installMode( dvr );
        module->renderer().setMode( Carna::VolumeRenderings::DVR::DirectVolumeRendering::NAME );
        setupVolumeRendererState( module->renderer() );

        /*
        dvr->setLighting( false );
        dvr->colorMap().clear();
        dvr->colorMap().put
            ( Carna::VolumeRenderings::DVR::ColorMap::HUSpan( -1024, 3071 )
            , Carna::VolumeRenderings::DVR::ColorMap::ColorSpan( Qt::red, Qt::blue ) ) ) );
            */

        dvr->commitColorMap();
    }
    );
    display->paint();
    VERIFY_FRAMEBUFFER( *display, "pelves_reduced_dvr_213spp_perspective.png" );
}
