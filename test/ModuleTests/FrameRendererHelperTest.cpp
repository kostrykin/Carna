/*
 *  Copyright (C) 2010 - 2016 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 * 
 * 
 *  Copyright (C) 2021 - 2025 Leonid Kostrykin
 * 
 */

#include "FrameRendererHelperTest.hpp"
#include <LibCarna/presets/OpaqueRenderingStage.hpp>
#include <LibCarna/presets/TransparentRenderingStage.hpp>
#include <LibCarna/presets/CuttingPlanesStage.hpp>
#include <LibCarna/presets/DRRStage.hpp>
#include <LibCarna/presets/OccludedRenderingStage.hpp>
#include <LibCarna/presets/MeshColorCodingStage.hpp>
#include <LibCarna/presets/ParallaxStage.hpp>
#include <LibCarna/helpers/FrameRendererHelper.hpp>



// ----------------------------------------------------------------------------------
// FrameRendererHelperTest
// ----------------------------------------------------------------------------------

void FrameRendererHelperTest::initTestCase()
{
    qglContextHolder.reset( new QGLContextHolder() );
}


void FrameRendererHelperTest::cleanupTestCase()
{
    qglContextHolder.reset();
}


void FrameRendererHelperTest::init()
{
    renderer.reset( new base::FrameRenderer( qglContextHolder->glContext(), 640, 480, false ) );
}


void FrameRendererHelperTest::cleanup()
{
    renderer.reset();
}


void FrameRendererHelperTest::test_emptyCommit()
{
    renderer->appendStage( new presets::OpaqueRenderingStage( 0 ) );
    QCOMPARE( renderer->stages(), static_cast< std::size_t >( 1u ) );
    helpers::FrameRendererHelper< > helper( *renderer );
    helper.commit();
    QCOMPARE( renderer->stages(), static_cast< std::size_t >( 0u ) );
}


void FrameRendererHelperTest::test_complete()
{
    helpers::FrameRendererHelper< > helper( *renderer );
    helper << new presets::TransparentRenderingStage( 3 );
    helper << new presets::OpaqueRenderingStage( 0 );
    helper << new presets::DRRStage( 1 );
    helper << new presets::CuttingPlanesStage( 1, 2 );
    helper << new presets::OccludedRenderingStage();
    helper << new presets::MeshColorCodingStage();
    helper << new presets::ParallaxStage( presets::ParallaxStage::aside );
    helper.commit();

    QCOMPARE( renderer->stages(), static_cast< std::size_t >( 7u ) );

    QVERIFY( dynamic_cast< presets::     MeshColorCodingStage* >( &renderer->stageAt( 0 ) ) != nullptr );
    QVERIFY( dynamic_cast< presets::            ParallaxStage* >( &renderer->stageAt( 1 ) ) != nullptr );
    QVERIFY( dynamic_cast< presets::       CuttingPlanesStage* >( &renderer->stageAt( 2 ) ) != nullptr );
    QVERIFY( dynamic_cast< presets::   OccludedRenderingStage* >( &renderer->stageAt( 3 ) ) != nullptr );
    QVERIFY( dynamic_cast< presets::     OpaqueRenderingStage* >( &renderer->stageAt( 4 ) ) != nullptr );
    QVERIFY( dynamic_cast< presets::TransparentRenderingStage* >( &renderer->stageAt( 5 ) ) != nullptr );
    QVERIFY( dynamic_cast< presets::                 DRRStage* >( &renderer->stageAt( 6 ) ) != nullptr );
}
