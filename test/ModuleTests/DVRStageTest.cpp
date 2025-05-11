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

#include "DVRStageTest.hpp"
#include <LibCarna/base/Node.hpp>
#include <LibCarna/base/FrameRenderer.hpp>
#include <LibCarna/base/BufferedIntensityVolume.hpp>
#include <LibCarna/helpers/VolumeGridHelper.hpp>
#include <LibCarna/presets/DVRStage.hpp>



// ----------------------------------------------------------------------------------
// DVRStageTest
// ----------------------------------------------------------------------------------

void DVRStageTest::initTestCase()
{
    const unsigned int width  = 640;
    const unsigned int height = 480;

    qglContextHolder.reset( new QGLContextHolder() );
    testFramebuffer.reset( new TestFramebuffer( qglContextHolder->glContext(), width, height ) );

    /* Load test volume data.
     */
    data.reset( HUGZSceneFactory::importVolume( SOURCE_PATH + "/res/pelves_reduced.hugz", dataSpacings ) );
}


void DVRStageTest::cleanupTestCase()
{
    testFramebuffer.reset();
    qglContextHolder.reset();
}


void DVRStageTest::init()
{
    renderer.reset( new base::FrameRenderer( qglContextHolder->glContext(), testFramebuffer->width(), testFramebuffer->height(), true ) );
    root.reset( new base::Node() );

    //! [dvr_instantiation]
    dvr = new presets::DVRStage( GEOMETRY_TYPE_VOLUMETRIC );
    renderer->appendStage( dvr );
    //! [dvr_instantiation]

    /* Configure camera.
     */
    cam = new base::Camera();
    cam->localTransform = base::math::rotation4f( 0, 1, 0, base::math::deg2rad( 20 ) ) * base::math::translation4f( 0, 0, 350 );
    cam->setProjection( base::math::frustum4f( base::math::deg2rad( 90 ), 1, 10, 2000 ) );
    root->attachChild( cam );
}


void DVRStageTest::cleanup()
{
    renderer.reset();
    root.reset();
}


void DVRStageTest::test_withLighting()
{
    /* Add volume data to scene.
     */
    typedef helpers::VolumeGridHelper< base::IntensityVolumeUInt16, base::NormalMap3DInt8 > GridHelper;
    GridHelper gridHelper( data->size );
    gridHelper.loadIntensities( *data );
    root->attachChild( gridHelper.createNode( GEOMETRY_TYPE_VOLUMETRIC, GridHelper::Spacing( dataSpacings ) ) );

    /* Configure DVR stage.
     */
    //! [dvr_setup_with_lighting]
    dvr->colorMap.writeLinearSegment( base::HUV( -400 ).intensity(), base::HUV(   0 ).intensity(), base::Color:: BLUE_NO_ALPHA, base::Color:: BLUE );
    dvr->colorMap.writeLinearSegment( base::HUV(    0 ).intensity(), base::HUV( 400 ).intensity(), base::Color::GREEN_NO_ALPHA, base::Color::GREEN );
    dvr->setSampleRate( 1000 );
    dvr->setTranslucency( 2 );
    //! [dvr_setup_with_lighting]

    /* Render and verify.
     */
    renderer->render( *cam, *root );
    VERIFY_FRAMEBUFFER( *testFramebuffer );
}


void DVRStageTest::test_withoutLighting()
{
    /* Add volume data to scene.
     */
    typedef helpers::VolumeGridHelper< base::IntensityVolumeUInt16 > GridHelper;
    GridHelper gridHelper( data->size );
    gridHelper.loadIntensities( *data );
    root->attachChild( gridHelper.createNode( GEOMETRY_TYPE_VOLUMETRIC, GridHelper::Spacing( dataSpacings ) ) );

    /* Configure DVR stage.
     */
    //! [dvr_setup_without_lighting]
    dvr->colorMap.writeLinearSegment( base::HUV( -400 ).intensity(), base::HUV(   0 ).intensity(), base::Color:: BLUE_NO_ALPHA, base::Color:: BLUE );
    dvr->colorMap.writeLinearSegment( base::HUV(    0 ).intensity(), base::HUV( 400 ).intensity(), base::Color::GREEN_NO_ALPHA, base::Color::GREEN );
    dvr->setDiffuseLight( 0 );
    //! [dvr_setup_without_lighting]

    /* Render and verify.
     */
    renderer->render( *cam, *root );
    VERIFY_FRAMEBUFFER( *testFramebuffer );
}


void DVRStageTest::test_withoutColormap()
{
    /* Add volume data to scene.
     */
    test_withoutLighting();

    /* Configure DVR stage.
     */
    dvr->colorMap.clear();

    /* Render and verify.
     */
    renderer->render( *cam, *root );
    VERIFY_FRAMEBUFFER( *testFramebuffer );
}


void DVRStageTest::test_withColorMapLimits()
{
    /* Add volume data to scene.
     */
    typedef helpers::VolumeGridHelper< base::IntensityVolumeUInt16, base::NormalMap3DInt8 > GridHelper;
    GridHelper gridHelper( data->size );
    gridHelper.loadIntensities( *data );
    root->attachChild( gridHelper.createNode( GEOMETRY_TYPE_VOLUMETRIC, GridHelper::Spacing( dataSpacings ) ) );

    /* Configure DVR stage (should be equivalent to `test_withLighting`).
     */
    dvr->colorMap.writeLinearSegment( 0.0f, 0.5f, base::Color:: BLUE_NO_ALPHA, base::Color:: BLUE );
    dvr->colorMap.writeLinearSegment( 0.5f, 1.0f, base::Color::GREEN_NO_ALPHA, base::Color::GREEN );
    dvr->colorMap.setMinimumIntensity( base::HUV( -400 ).intensity() );
    dvr->colorMap.setMaximumIntensity( base::HUV( +400 ).intensity() );
    dvr->setSampleRate( 1000 );
    dvr->setTranslucency( 2 );

    /* Render and verify.
     */
    renderer->render( *cam, *root );
    VERIFY_FRAMEBUFFER( *testFramebuffer );
}
