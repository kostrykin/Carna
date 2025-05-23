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

#include "MIPStageTest.hpp"
#include <LibCarna/base/Node.hpp>
#include <LibCarna/base/FrameRenderer.hpp>
#include <LibCarna/presets/MIPStage.hpp>



// ----------------------------------------------------------------------------------
// MIPStageTest
// ----------------------------------------------------------------------------------

void MIPStageTest::initTestCase()
{
}


void MIPStageTest::cleanupTestCase()
{
}


void MIPStageTest::init()
{
    const unsigned int width  = 640;
    const unsigned int height = 480;

    qglContextHolder.reset( new QGLContextHolder() );
    testFramebuffer.reset( new TestFramebuffer( qglContextHolder->glContext(), width, height ) );
    scene.reset( new TestScene( true ) );
    renderer.reset( new base::FrameRenderer( qglContextHolder->glContext(), width, height, true ) );

    const static unsigned int GEOMETRY_TYPE_VOLUMETRIC = TestScene::GEOMETRY_TYPE_VOLUMETRIC;
    //! [mip_instantiation]
    mip = new presets::MIPStage( GEOMETRY_TYPE_VOLUMETRIC );
    mip->colorMap.writeLinearSegment(
        0, 1,
        base::Color( 0, 0, 255, 255 ), base::Color( 255, 0, 0, 255 )
    );
    renderer->appendStage( mip );
    //! [mip_instantiation]

    mip->setSampleRate( 100 );
}


void MIPStageTest::cleanup()
{
    renderer.reset();
    scene.reset();
    testFramebuffer.reset();
    qglContextHolder.reset();
}


void MIPStageTest::test_writeLinearSegment()
{
    renderer->render( scene->cam(), *scene->root );
    testFramebuffer->numIgnore = 60; // Software rendering produces some artifacts at the corners of the volume, which are negligible.
    VERIFY_FRAMEBUFFER( *testFramebuffer );
}


void MIPStageTest::test_writeLinearSpline()
{
    //! [mip_jet]
    std::vector< base::Color > colors =
        { base::Color(   0,   0, 255,   0 )
        , base::Color(   0,   0, 255, 255 )
        , base::Color(   0, 255, 255, 255 )
        , base::Color( 255, 255,   0, 255 )
        , base::Color( 255,   0,   0, 255 ) };
    mip->colorMap.writeLinearSpline( colors );
    //! [mip_jet]
    renderer->render( scene->cam(), *scene->root );
    VERIFY_FRAMEBUFFER( *testFramebuffer );
}


void MIPStageTest::test_colorMapLimits()
{
    mip->colorMap.setMinimumIntensity( 0.25f );
    mip->colorMap.setMaximumIntensity( 0.5f );
    renderer->render( scene->cam(), *scene->root );
    testFramebuffer->numIgnore = 60; // Software rendering produces some artifacts at the corners of the volume, which are negligible.
    VERIFY_FRAMEBUFFER( *testFramebuffer );
}
