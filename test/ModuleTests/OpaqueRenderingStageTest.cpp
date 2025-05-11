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

#include "OpaqueRenderingStageTest.hpp"
#include <LibCarna/base/Node.hpp>
#include <LibCarna/base/Geometry.hpp>
#include <LibCarna/base/FrameRenderer.hpp>
#include <LibCarna/base/Vertex.hpp>
#include <LibCarna/base/Material.hpp>
#include <LibCarna/base/Mesh.hpp>
#include <LibCarna/base/MeshFactory.hpp>



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
    base::ManagedMeshBase& boxMesh = base::MeshFactory< base::PVertex >::createBox( 40, 40, 40 );
    base::Material& redMaterial   = base::Material::create( "unshaded" );
    base::Material& greenMaterial = base::Material::create( "unshaded" );
    redMaterial  .setParameter( "color", base::math::Vector4f( 1, 0, 0, 1 ) );
    greenMaterial.setParameter( "color", base::math::Vector4f( 0, 1, 0, 1 ) );

    base::Geometry* const box1 = new base::Geometry( GEOMETRY_TYPE_OPAQUE );
    base::Geometry* const box2 = new base::Geometry( GEOMETRY_TYPE_OPAQUE );

    box1->putFeature( presets::OpaqueRenderingStage::DEFAULT_ROLE_MESH, boxMesh );
    box2->putFeature( presets::OpaqueRenderingStage::DEFAULT_ROLE_MESH, boxMesh );

    box1->putFeature( presets::OpaqueRenderingStage::DEFAULT_ROLE_MATERIAL,   redMaterial );
    box2->putFeature( presets::OpaqueRenderingStage::DEFAULT_ROLE_MATERIAL, greenMaterial );

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
