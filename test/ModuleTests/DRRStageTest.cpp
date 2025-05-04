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

#include "DRRStageTest.hpp"
#include <LibCarna/base/Node.hpp>
#include <LibCarna/base/Color.hpp>
#include <LibCarna/base/FrameRenderer.hpp>
#include <LibCarna/presets/DRRStage.hpp>



// ----------------------------------------------------------------------------------
// DRRStageTest
// ----------------------------------------------------------------------------------

void DRRStageTest::initTestCase()
{
    const unsigned int width  = 640;
    const unsigned int height = 480;

    qglContextHolder.reset( new QGLContextHolder() );
    testFramebuffer.reset( new TestFramebuffer( qglContextHolder->glContext(), width, height ) );
    scene.reset( new TestScene() );
    renderer.reset( new base::FrameRenderer( qglContextHolder->glContext(), width, height, true ) );

    const static unsigned int GEOMETRY_TYPE_VOLUMETRIC = TestScene::GEOMETRY_TYPE_VOLUMETRIC;
    //! [drr_instantiation]
    drr = new presets::DRRStage( GEOMETRY_TYPE_VOLUMETRIC );
    renderer->appendStage( drr );
    //! [drr_instantiation]
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
    //! [drr_setup_base]
    drr->setSampleRate( 100 );
    drr->setWaterAttenuation( 5e-3f );
    drr->setBaseIntensity( 1.f );
    drr->setLowerThreshold( base::HUV::abs( -400 ) );
    drr->setUpperThreshold( base::HUV::abs( +400 ) );
    drr->setUpperMultiplier( 1.5f );
    //! [drr_setup_base]
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
    //! [drr_setup_inverse]
    drr->setRenderingInverse( true );
    renderer->setBackgroundColor( base::Color::WHITE_NO_ALPHA );
    //! [drr_setup_inverse]

    renderer->render( scene->cam(), *scene->root );
    VERIFY_FRAMEBUFFER( *testFramebuffer );
}
