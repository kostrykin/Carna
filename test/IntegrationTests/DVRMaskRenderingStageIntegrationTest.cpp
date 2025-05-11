/*
 *  Copyright (C) 2021 - 2025 Leonid Kostrykin
 * 
 */

#include "DVRMaskRenderingStageIntegrationTest.hpp"
#include <LibCarna/base/Node.hpp>
#include <LibCarna/base/Color.hpp>
#include <LibCarna/base/FrameRenderer.hpp>
#include <LibCarna/presets/MaskRenderingStage.hpp>



static const unsigned int DVRMaskRenderingStageIntegrationTest_WIDTH  = 640;
static const unsigned int DVRMaskRenderingStageIntegrationTest_HEIGHT = 480;



// ----------------------------------------------------------------------------------
// DVRMaskRenderingStageIntegrationTest
// ----------------------------------------------------------------------------------

void DVRMaskRenderingStageIntegrationTest::initTestCase()
{
    qglContextHolder.reset( new QGLContextHolder() );
    testFramebuffer.reset( new TestFramebuffer( qglContextHolder->glContext(), DRRMaskRenderingStageIntegrationTest_WIDTH, DRRMaskRenderingStageIntegrationTest_HEIGHT ) );
}


void DVRMaskRenderingStageIntegrationTest::cleanupTestCase()
{
    testFramebuffer.reset();
    qglContextHolder.reset();
}


void DVRMaskRenderingStageIntegrationTest::init()
{
    scene.reset( new TestScene() );
    const TestScene& scene = *( this->scene );
    const base::math::Vector3f spacings = scene.getSpacings();

    renderer.reset( new base::FrameRenderer( qglContextHolder->glContext(), DRRMaskRenderingStageIntegrationTest_WIDTH, DRRMaskRenderingStageIntegrationTest_HEIGHT, true ) );
    renderer->setBackgroundColor( base::Color::BLACK_NO_ALPHA );

    dvr = new presets::DVRStage( GEOMETRY_TYPE_VOLUMETRIC );
    dvr->colorMap.writeLinearSegment( 0.15f, 0.25f, base::Color:: BLUE_NO_ALPHA, base::Color:: BLUE );
    dvr->colorMap.writeLinearSegment( 0.25f, 0.35f, base::Color::GREEN_NO_ALPHA, base::Color::GREEN );
    dvr->setSampleRate( 1000 );
    dvr->setTranslucency( 2 );
    renderer->appendStage( dvr );

    mr = new presets::MaskRenderingStage( GEOMETRY_TYPE_MASK );
    mr->setFilling( false );
    renderer->appendStage( mr );

    /* Perform global thresholding and configure geometry node for the mask data.
     */
    maskGridHelper.reset( new MaskGridHelper( scene.volume().size ) );
    maskGridHelper->setIntensitiesRole( mr->maskRole );
    maskGridHelper->loadIntensities( [&scene]( const base::math::Vector3ui& voxel )
        {
            const base::HUV huv = base::HUV( scene.volume()( voxel ) );
            return huv >= -400 ? 1.f : 0.f;
        }
    );
    scene.root->attachChild( maskGridHelper->createNode( GEOMETRY_TYPE_MASK, MaskGridHelper::Spacing( spacings ) ) );
    maskGridHelper->releaseGeometryFeatures();
}


void DVRMaskRenderingStageIntegrationTest::cleanup()
{
    scene.reset();
    renderer.reset();
}


void DVRMaskRenderingStageIntegrationTest::renderFrames( unsigned int n )
{
    for( unsigned int itdx = 0; itdx < n; ++itdx )
    {
        renderer->render( this->scene->cam(), *this->scene->root );
        this->scene->cam().localTransform = base::math::rotation4f( 0, 1, 0, base::math::deg2rad( 10 ) ) * this->scene->cam().localTransform;

        /* Now that the textures have been uploaded (after the first iteration), we can delete the helper.
         */
        maskGridHelper.reset();
    }
}


void DVRMaskRenderingStageIntegrationTest::test_after1Frame()
{
    renderFrames( 1 );
    VERIFY_FRAMEBUFFER( *testFramebuffer );
}


void DVRMaskRenderingStageIntegrationTest::test_after2Frames()
{
    renderFrames( 2 );
    VERIFY_FRAMEBUFFER( *testFramebuffer );
}


void DVRMaskRenderingStageIntegrationTest::test_after3Frames()
{
    renderFrames( 3 );
    VERIFY_FRAMEBUFFER( *testFramebuffer );
}
