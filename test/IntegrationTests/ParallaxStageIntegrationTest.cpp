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

#include "ParallaxStageIntegrationTest.h"
#include <Carna/base/Node.h>
#include <Carna/base/Geometry.h>
#include <Carna/base/Vertex.h>
#include <Carna/base/Material.h>
#include <Carna/base/Mesh.h>
#include <Carna/base/MeshFactory.h>
#include <Carna/base/FrameRenderer.h>
#include <Carna/presets/DRRStage.h>
#include <Carna/presets/ParallaxStage.h>



// ----------------------------------------------------------------------------------
// ParallaxStageIntegrationTest
// ----------------------------------------------------------------------------------

void ParallaxStageIntegrationTest::initTestCase()
{
    const unsigned int width  = 640;
    const unsigned int height = 480;

    qglContextHolder.reset( new QGLContextHolder() );
    testFramebuffer.reset( new TestFramebuffer( qglContextHolder->glContext(), width, height ) );
    scene.reset( new TestScene() );
    scene->cam().localTransform *= base::math::translation4f( 0, 0, -200 );

    /* Create and add opaque objects to scene.
     */
    //! [parallax_scene_setup]
    base::ManagedMeshBase& boxMesh = base::MeshFactory< base::PVertex >::createBox( 40, 40, 40 );
    base::Material& material = base::Material::create( "unshaded" );
    material.setParameter( "color", base::Color::RED );
    
    base::Geometry* const box = new base::Geometry( GEOMETRY_TYPE_OPAQUE );
    box->putFeature( presets::OpaqueRenderingStage::ROLE_DEFAULT_MESH, boxMesh );
    box->putFeature( presets::OpaqueRenderingStage::ROLE_DEFAULT_MATERIAL, material );
    scene->root->attachChild( box );

    boxMesh.release();
    material.release();
    //! [parallax_scene_setup]
}


void ParallaxStageIntegrationTest::cleanupTestCase()
{
    scene.reset();
    testFramebuffer.reset();
    qglContextHolder.reset();
}


void ParallaxStageIntegrationTest::init()
{
    renderer.reset( new base::FrameRenderer( qglContextHolder->glContext(), testFramebuffer->width(), testFramebuffer->height(), true ) );
    
    //! [parallax_instantiation]
    /* Create parallax rendering stage.
     */
    parallax = new presets::ParallaxStage( presets::ParallaxStage::aside );
    parallax->setEyeDistance( presets::ParallaxStage::DEFAULT_EYE_DISTANCE * 3 );
    renderer->appendStage( parallax );
    //! [parallax_instantiation]

    const static unsigned int GEOMETRY_TYPE_VOLUMETRIC = TestScene::GEOMETRY_TYPE_VOLUMETRIC;
    //! [parallax_instantiation_others]
    /* Create opaque rendering stage.
     */
    presets::OpaqueRenderingStage* const opaque = new presets::OpaqueRenderingStage( GEOMETRY_TYPE_OPAQUE );
    renderer->appendStage( opaque );

    /* Create volumetric rendering stage.
     */
    presets::DRRStage* const drr = new presets::DRRStage( GEOMETRY_TYPE_VOLUMETRIC );
    renderer->appendStage( drr );
    drr->setSampleRate( 100 );
    drr->setWaterAttenuation( 5e-3f );
    drr->setBaseIntensity( 1.f );
    drr->setLowerThreshold( -400 );
    drr->setUpperThreshold( +400 );
    drr->setUpperMultiplier( 1.5f );
    //! [parallax_instantiation_others]
}


void ParallaxStageIntegrationTest::cleanup()
{
    renderer.reset();
}


void ParallaxStageIntegrationTest::test_aside()
{
    parallax->setCompositionMode( presets::CompositionStage::aside );
    renderer->render( scene->cam(), *scene->root );
    VERIFY_FRAMEBUFFER( *testFramebuffer );
}


void ParallaxStageIntegrationTest::test_interleaved()
{
    parallax->setEyeDistance( presets::ParallaxStage::DEFAULT_EYE_DISTANCE / 2 );
    parallax->setCompositionMode( presets::CompositionStage::interleave );
    renderer->render( scene->cam(), *scene->root );
    VERIFY_FRAMEBUFFER( *testFramebuffer );
}
