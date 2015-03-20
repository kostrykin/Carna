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

#include "OpaqueRenderingStageTest.h"
#include <Carna/base/Node.h>
#include <Carna/base/Geometry.h>
#include <Carna/base/FrameRenderer.h>
#include <Carna/base/Vertex.h>
#include <Carna/base/Material.h>
#include <Carna/base/Mesh.h>
#include <Carna/base/MeshFactory.h>



// ----------------------------------------------------------------------------------
// OpaqueRenderingStageTest
// ----------------------------------------------------------------------------------

void OpaqueRenderingStageTest::initTestCase()
{
    const unsigned int width  = 640;
    const unsigned int height = 480;

    qglContextHolder.reset( new QGLContextHolder() );
    testFramebuffer.reset( new TestFramebuffer( qglContextHolder->glContext(), width, height ) );
    scene.reset( new TestScene() );
    renderer.reset( new base::FrameRenderer( qglContextHolder->glContext(), width, height, true ) );

    //! [opaque_stage_instantiation]
    opaque = new presets::OpaqueRenderingStage( GEOMETRY_TYPE_OPAQUE );
    renderer->appendStage( opaque );
    //! [opaque_stage_instantiation]

    /* Create and add opaque objects to scene.
     */
    //! [opaque_stage_scene_setup]
    base::MeshBase& boxMesh = base::MeshFactory< base::VertexBase >::createBox( 40, 40, 40 );
    base::Material& redMaterial   = base::Material::create( "unshaded" );
    base::Material& greenMaterial = base::Material::create( "unshaded" );
    redMaterial  .setParameter( "color", base::math::Vector4f( 1, 0, 0, 1 ) );
    greenMaterial.setParameter( "color", base::math::Vector4f( 0, 1, 0, 1 ) );

    base::Geometry* const box1 = new base::Geometry( GEOMETRY_TYPE_OPAQUE );
    base::Geometry* const box2 = new base::Geometry( GEOMETRY_TYPE_OPAQUE );

    box1->putFeature( presets::OpaqueRenderingStage::ROLE_DEFAULT_MESH, boxMesh );
    box2->putFeature( presets::OpaqueRenderingStage::ROLE_DEFAULT_MESH, boxMesh );

    box1->putFeature( presets::OpaqueRenderingStage::ROLE_DEFAULT_MATERIAL,   redMaterial );
    box2->putFeature( presets::OpaqueRenderingStage::ROLE_DEFAULT_MATERIAL, greenMaterial );

    boxMesh.release();
    redMaterial.release();
    greenMaterial.release();

    scene->root->attachChild( box1 );
    scene->root->attachChild( box2 );

    box1->localTransform = base::math::translation4f( -10, -10, -40 );
    box2->localTransform = base::math::translation4f( +10, +10, +40 );
    //! [opaque_stage_scene_setup]
}


void OpaqueRenderingStageTest::cleanupTestCase()
{
    renderer.reset();
    scene.reset();
    testFramebuffer.reset();
    qglContextHolder.reset();
}


void OpaqueRenderingStageTest::init()
{
}


void OpaqueRenderingStageTest::cleanup()
{
}


void OpaqueRenderingStageTest::test_fromFront()
{
    scene->resetCamTransform();
    renderer->render( scene->cam(), *scene->root );
    VERIFY_FRAMEBUFFER( *testFramebuffer );
}


void OpaqueRenderingStageTest::test_fromBack()
{
    scene->cam().localTransform = base::math::rotation4f( 0, 1, 0, base::math::deg2rad( 180 ) ) * scene->cam().localTransform;
    renderer->render( scene->cam(), *scene->root );
    VERIFY_FRAMEBUFFER( *testFramebuffer );
}
