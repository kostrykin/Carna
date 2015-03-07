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

#include "DRRStageTest.h"
#include <Carna/base/Node.h>
#include <Carna/base/Color.h>
#include <Carna/base/FrameRenderer.h>
#include <Carna/presets/DRRStage.h>



// ----------------------------------------------------------------------------------
// DRRStageTest
// ----------------------------------------------------------------------------------

void DRRStageTest::initTestCase()
{
    const unsigned int width  = 640;
    const unsigned int height = 480;

    qglContextHolder.reset( new QGLContextHolder() );
    testFramebuffer.reset( new TestFramebuffer( qglContextHolder->glContext(), width, height ) );
    scene.reset( new TestScene( qglContextHolder->glContext() ) );
    renderer.reset( new base::FrameRenderer( qglContextHolder->glContext(), width, height, true ) );

    drr = new presets::DRRStage( TestScene::GEOMETRY_TYPE_VOLUMETRIC );
    renderer->appendStage( drr );
}


void DRRStageTest::cleanupTestCase()
{
    renderer.reset();
    scene.reset();
    testFramebuffer.reset();
    qglContextHolder.reset();
}


void DRRStageTest::init()
{
    drr->setSampleRate( 100 );
    drr->setWaterAttenuation( 2.f );
    drr->setBaseIntensity( 1.f );
    drr->setLowerThreshold( -400 );
    drr->setUpperThreshold( +400 );
    drr->setUpperMultiplier( 1.5f );
    drr->setRenderingInverse( false );
    renderer->setBackgroundColor( base::Color::BLACK_NO_ALPHA );
}


void DRRStageTest::cleanup()
{
}


void DRRStageTest::test_nonInverted()
{
    renderer->render( scene->cam(), *scene->root );
    VERIFY_FRAMEBUFFER( *testFramebuffer );
}


void DRRStageTest::test_inverted()
{
    drr->setRenderingInverse( true );
    renderer->setBackgroundColor( base::Color::WHITE_NO_ALPHA );

    renderer->render( scene->cam(), *scene->root );
    VERIFY_FRAMEBUFFER( *testFramebuffer );
}
