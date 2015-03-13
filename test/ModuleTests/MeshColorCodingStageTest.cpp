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

#include "MeshColorCodingStageTest.h"
#include <Carna/base/Node.h>
#include <Carna/base/Geometry.h>
#include <Carna/base/FrameRenderer.h>
#include <Carna/base/Vertex.h>
#include <Carna/base/Material.h>
#include <Carna/base/Mesh.h>
#include <Carna/base/MeshFactory.h>
#include <Carna/base/Viewport.h>
#include <Carna/base/Aggregation.h>
#include <Carna/presets/MeshColorCodingStage.h>



// ----------------------------------------------------------------------------------
// MeshColorCodingStageTest
// ----------------------------------------------------------------------------------

void MeshColorCodingStageTest::initTestCase()
{
    const unsigned int width  = 640;
    const unsigned int height = 480;

    qglContextHolder.reset( new QGLContextHolder() );
    testFramebuffer.reset( new TestFramebuffer( qglContextHolder->glContext(), width, height ) );
    scene.reset( new TestScene() );
    renderer.reset( new base::FrameRenderer( qglContextHolder->glContext(), width, height, true ) );

    opaque = new presets::OpaqueRenderingStage( GEOMETRY_TYPE_OPAQUE );
    renderer->appendStage( opaque );

    mccs = new presets::MeshColorCodingStage();
    mccs->putGeometryType( GEOMETRY_TYPE_OPAQUE, presets::OpaqueRenderingStage::ROLE_DEFAULT_MESH );
    renderer->appendStage( mccs );

    /* Create and add objects to scene.
     */
    base::MeshBase& boxMesh = base::MeshFactory< base::VertexBase >::createBox( 40, 40, 40 );
    base::Material& redMaterial   = base::Material::create( "unshaded" );
    base::Material& greenMaterial = base::Material::create( "unshaded" );
    redMaterial  .addUniform( new base::ShaderUniform< base::math::Vector4f >( "color", base::math::Vector4f( 1, 0, 0, 1 ) ) );
    greenMaterial.addUniform( new base::ShaderUniform< base::math::Vector4f >( "color", base::math::Vector4f( 0, 1, 0, 1 ) ) );

    objRed   = new base::Geometry( GEOMETRY_TYPE_OPAQUE );
    objGreen = new base::Geometry( GEOMETRY_TYPE_OPAQUE );

    objRed  ->putFeature( presets::OpaqueRenderingStage::ROLE_DEFAULT_MESH, boxMesh );
    objGreen->putFeature( presets::OpaqueRenderingStage::ROLE_DEFAULT_MESH, boxMesh );

    objRed  ->putFeature( presets::OpaqueRenderingStage::ROLE_DEFAULT_MATERIAL,   redMaterial );
    objGreen->putFeature( presets::OpaqueRenderingStage::ROLE_DEFAULT_MATERIAL, greenMaterial );

    boxMesh.release();
    redMaterial.release();
    greenMaterial.release();

    scene->root->attachChild( objRed   );
    scene->root->attachChild( objGreen );

    objRed  ->localTransform = base::math::translation4f( -5, +25, -40 );
    objGreen->localTransform = base::math::translation4f( +5, +30, +40 );
}


void MeshColorCodingStageTest::cleanupTestCase()
{
    renderer.reset();
    scene.reset();
    testFramebuffer.reset();
    qglContextHolder.reset();
}


void MeshColorCodingStageTest::init()
{
}


void MeshColorCodingStageTest::cleanup()
{
}


Carna::base::math::Vector2ui MeshColorCodingStageTest::computeFrameLocation( const Carna::base::Spatial& obj ) const
{
    const base::Viewport& vp = renderer->viewport();

    const base::math::Matrix4f modelViewProjection = scene->cam().projection() * scene->cam().viewTransform() * obj.worldTransform();
    const base::math::Vector4f clippingCoordinates = modelViewProjection * base::math::Vector4f( 0, 0, 0, 1 );
    const base::math::Vector3f       ndCoordinates = base::math::vector3f( clippingCoordinates / clippingCoordinates.w() );
    const base::math::Vector2f    frameCoordinates = base::math::Vector2f
        ( (  ndCoordinates.x() / 2 + 0.5f ) * vp.width  + vp.left
        , ( -ndCoordinates.y() / 2 + 0.5f ) * vp.height + vp.top );

    return base::math::round_ui( frameCoordinates );
}


void MeshColorCodingStageTest::test_fromFront()
{
    /* Verify the framebuffer result to ensure the MeshColorCodingStage did not
     * interfere with OpaqueRenderingStage.
     */
    scene->resetCamTransform();
    renderer->render( scene->cam(), *scene->root );
    VERIFY_FRAMEBUFFER( *testFramebuffer );

    /* Now test the results of the MeshColorCodingStage. Since in this perspective
     * the green object almost occludes the red one, we must pick the green one at
     * the centers of both.
     */
    QCOMPARE( mccs->pick( computeFrameLocation( *objRed   ) ).get(), objGreen );
    QCOMPARE( mccs->pick( computeFrameLocation( *objGreen ) ).get(), objGreen );
}


void MeshColorCodingStageTest::test_fromBack()
{
    /* Verify the framebuffer result to ensure the MeshColorCodingStage did not
     * interfere with OpaqueRenderingStage.
     */
    scene->resetCamTransform();
    scene->cam().localTransform = base::math::rotation4f( 0, 1, 0, base::math::deg2rad( 180 ) ) * scene->cam().localTransform;
    renderer->render( scene->cam(), *scene->root );
    VERIFY_FRAMEBUFFER( *testFramebuffer );

    /* Now test the results of the MeshColorCodingStage. Since in this perspective
     * the red object almost occludes the green one, we must pick the red one at
     * the centers of both.
     */
    QCOMPARE( mccs->pick( computeFrameLocation( *objRed   ) ).get(), objRed );
    QCOMPARE( mccs->pick( computeFrameLocation( *objGreen ) ).get(), objRed );
}


void MeshColorCodingStageTest::test_fromSide()
{
    /* Verify the framebuffer result to ensure the MeshColorCodingStage did not
     * interfere with OpaqueRenderingStage.
     */
    scene->resetCamTransform();
    scene->cam().localTransform = base::math::rotation4f( 0, 1, 0, base::math::deg2rad( 90 ) ) * scene->cam().localTransform;
    renderer->render( scene->cam(), *scene->root );
    VERIFY_FRAMEBUFFER( *testFramebuffer );

    /* Now test the results of the MeshColorCodingStage. Since in this perspective
     * none of the objects is occluded, we must pick the object itself respectively.
     */
    QCOMPARE( mccs->pick( computeFrameLocation( *objRed   ) ).get(), objRed   );
    QCOMPARE( mccs->pick( computeFrameLocation( *objGreen ) ).get(), objGreen );

    /* Furthermore, we must pick nothing in between of them.
     */
    QVERIFY
        (  mccs->pick( ( computeFrameLocation( *objRed ) + computeFrameLocation( *objGreen ) ) / 2 )
        == base::Aggregation< const base::Geometry >::NULL_PTR );
}


void MeshColorCodingStageTest::test_atInvalidFrameLocations()
{
    scene->resetCamTransform();
    renderer->render( scene->cam(), *scene->root );

    QVERIFY
        (  mccs->pick( 0, 0 )
        == base::Aggregation< const base::Geometry >::NULL_PTR );

    QVERIFY
        (  mccs->pick( 0, renderer->height() / 2 )
        == base::Aggregation< const base::Geometry >::NULL_PTR );

    QVERIFY
        (  mccs->pick( renderer->width() / 2, 0 )
        == base::Aggregation< const base::Geometry >::NULL_PTR );

    QVERIFY
        (  mccs->pick( renderer->width() * 2, renderer->height() / 2 )
        == base::Aggregation< const base::Geometry >::NULL_PTR );

    QVERIFY
        (  mccs->pick( renderer->width() / 2, renderer->height() * 2 )
        == base::Aggregation< const base::Geometry >::NULL_PTR );

    QVERIFY
        (  mccs->pick( renderer->width() * 2, renderer->height() * 2 )
        == base::Aggregation< const base::Geometry >::NULL_PTR );
}
