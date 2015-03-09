/*
 *  Copyright (C) 2010 - 2015 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#include "FrameRendererHelperTest.h"
#include <Carna/presets/OpaqueRenderingStage.h>
#include <Carna/presets/TransparentRenderingStage.h>
#include <Carna/presets/CuttingPlanesStage.h>
#include <Carna/presets/DRRStage.h>
#include <Carna/presets/OccludedRenderingStage.h>
#include <Carna/presets/MeshColorCodingStage.h>
#include <Carna/helpers/FrameRendererHelper.h>



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
    QCOMPARE( renderer->stages(), 1u );
    helpers::FrameRendererHelper< > helper( *renderer );
    helper.commit();
    QCOMPARE( renderer->stages(), 0u );
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
    helper.commit();

    QCOMPARE( renderer->stages(), 6u );

    QVERIFY( dynamic_cast< presets::     MeshColorCodingStage* >( &renderer->stageAt( 0 ) ) != nullptr );
    QVERIFY( dynamic_cast< presets::       CuttingPlanesStage* >( &renderer->stageAt( 1 ) ) != nullptr );
    QVERIFY( dynamic_cast< presets::   OccludedRenderingStage* >( &renderer->stageAt( 2 ) ) != nullptr );
    QVERIFY( dynamic_cast< presets::     OpaqueRenderingStage* >( &renderer->stageAt( 3 ) ) != nullptr );
    QVERIFY( dynamic_cast< presets::TransparentRenderingStage* >( &renderer->stageAt( 4 ) ) != nullptr );
    QVERIFY( dynamic_cast< presets::                 DRRStage* >( &renderer->stageAt( 5 ) ) != nullptr );
}
