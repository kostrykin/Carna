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

#include "MIPStageTest.h"
#include <Carna/base/Node.h>
#include <Carna/base/FrameRenderer.h>
#include <Carna/presets/MIPStage.h>
#include <Carna/presets/MIPLayer.h>



// ----------------------------------------------------------------------------------
// MIPStageTest
// ----------------------------------------------------------------------------------

void MIPStageTest::initTestCase()
{
    const unsigned int width  = 640;
    const unsigned int height = 480;

    qglContextHolder.reset( new QGLContextHolder() );
    testFramebuffer.reset( new TestFramebuffer( qglContextHolder->glContext(), width, height ) );
    scene.reset( new TestScene() );
    renderer.reset( new base::FrameRenderer( qglContextHolder->glContext(), width, height, true ) );

    const static unsigned int GEOMETRY_TYPE_VOLUMETRIC = TestScene::GEOMETRY_TYPE_VOLUMETRIC;
    //! [mip_instantiation]
    mip = new presets::MIPStage( GEOMETRY_TYPE_VOLUMETRIC );
    mip->appendLayer( new presets::MIPLayer( -1024,    0, base::math::Vector4f( 0, 0, 1, 0.5f ) ) );
    mip->appendLayer( new presets::MIPLayer(     0, 3071, base::math::Vector4f( 1, 1, 0, 0.5f ) ) );
    renderer->appendStage( mip );
    //! [mip_instantiation]
}


void MIPStageTest::cleanupTestCase()
{
    renderer.reset();
    scene.reset();
    testFramebuffer.reset();
    qglContextHolder.reset();
}


void MIPStageTest::init()
{
    mip->setSampleRate( 100 );
}


void MIPStageTest::cleanup()
{
}


void MIPStageTest::test_layerReplace()
{
    mip->layer( 1 ).setFunction( presets::MIPLayer::LAYER_FUNCTION_REPLACE );
    renderer->render( scene->cam(), *scene->root );
    VERIFY_FRAMEBUFFER( *testFramebuffer );
}


void MIPStageTest::test_layerAdditive()
{
    //! [mip_setup_additive]
    mip->layer( 1 ).setFunction( presets::MIPLayer::LAYER_FUNCTION_ADD );
    //! [mip_setup_additive]
    renderer->render( scene->cam(), *scene->root );
    VERIFY_FRAMEBUFFER( *testFramebuffer );
}
