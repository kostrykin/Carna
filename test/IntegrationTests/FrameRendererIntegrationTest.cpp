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

#include "FrameRendererIntegrationTest.h"
#include <Carna/base/Node.h>
#include <Carna/base/Geometry.h>
#include <Carna/base/FrameRenderer.h>
#include <Carna/presets/CuttingPlanesStage.h>
#include <Carna/helpers/HUVolumeGridHelper.h>



// ----------------------------------------------------------------------------------
// FrameRendererIntegrationTest
// ----------------------------------------------------------------------------------

void FrameRendererIntegrationTest::initTestCase()
{
}


void FrameRendererIntegrationTest::cleanupTestCase()
{
}


void FrameRendererIntegrationTest::init()
{
}


void FrameRendererIntegrationTest::cleanup()
{
}


void FrameRendererIntegrationTest::test_typical()
{
    QGLContextHolder glContextHolder;
    base::GLContext& glContext = glContextHolder.glContext();
    TestFramebuffer testFramebuffer( glContext, 800, 600 );

    //! [typical_renderer_setup]
    const static unsigned int GEOMETRY_TYPE_VOLUMETRIC = 0;
    const static unsigned int GEOMETRY_TYPE_PLANE      = 1;
    const static unsigned int GEOMETRY_TYPE_OPAQUE     = 2;

    using namespace Carna;

    base::FrameRenderer fr( glContext, 800, 600, true );
    helpers::FrameRendererHelper< > frHelper( fr );
    frHelper << new presets::CuttingPlanesStage( GEOMETRY_TYPE_VOLUMETRIC, GEOMETRY_TYPE_PLANE );
    frHelper << new presets::OpaqueRenderingStage( GEOMETRY_TYPE_OPAQUE );
    frHelper << new presets::DRRStage( GEOMETRY_TYPE_VOLUMETRIC );
    frHelper << new presets::OccludedRenderingStage();
    frHelper.commit();

    fr.findStage< presets::CuttingPlanesStage >()->setWindowingWidth( 2000 );
    fr.findStage< presets::CuttingPlanesStage >()->setRenderingInverse( true );
    fr.findStage< presets::DRRStage >()->setRenderingInverse( true );
    fr.setBackgroundColor( base::Color::WHITE_NO_ALPHA );
    //! [typical_renderer_setup]

    /* Load test volume data.
     */
    base::math::Vector3f spacings;
    const std::unique_ptr< base::UInt16HUVolume >dataPtr
        ( HUGZSceneFactory::importVolume( SOURCE_PATH + "/res/pelves_reduced.hugz", spacings ) );
    const base::UInt16HUVolume& data = *dataPtr;

    //! [typical_scene_setup]
    base::Node root;

    /* Configure camera.
     */
    base::Camera* const cam = new base::Camera();
    cam->localTransform = base::math::rotation4f( 0, 1, 0, base::math::deg2rad( 20 ) ) * base::math::translation4f( 0, 0, 350 );
    cam->setProjection( base::math::frustum4f( base::math::deg2rad( 45 ), 1, 10, 2000 ) );
    root.attachChild( cam );

    /* Configure geometry node for volume data.
     */
    typedef helpers::HUVolumeGridHelper< base::UInt16HUVolume > UInt16HUGridHelper;
    UInt16HUGridHelper gridHelper( data.size );
    gridHelper.loadData( data );
    root.attachChild( gridHelper.createNode( GEOMETRY_TYPE_VOLUMETRIC, UInt16HUGridHelper::Spacing( spacings ), presets::DRRStage::ROLE_HU_VOLUME ) );
    gridHelper.invalidateTextures( glContext );

    /* Configure cutting planes.
     */
    base::Geometry* const plane1 = new base::Geometry( GEOMETRY_TYPE_PLANE );
    plane1->localTransform = base::math::plane4f( base::math::Vector3f( 1, 1, 1 ).normalized(), 0 );
    root.attachChild( plane1 );

    /* Configure opaque geometries.
     */
    base::MeshBase& boxMesh = base::MeshFactory< base::VertexBase >::createBox( 40, 40, 40 );
    base::Material& boxMaterial = base::Material::create( "unshaded" );
    boxMaterial.addUniform( new base::ShaderUniform< base::math::Vector4f >( "color", base::Color::GREEN ) );
    base::Geometry* const boxGeometry = new base::Geometry( GEOMETRY_TYPE_OPAQUE );
    boxGeometry->putFeature( presets::OpaqueRenderingStage::ROLE_DEFAULT_MATERIAL, boxMaterial );
    boxGeometry->putFeature( presets::OpaqueRenderingStage::ROLE_DEFAULT_MESH, boxMesh );
    boxGeometry->localTransform = base::math::translation4f( 0, -15, 0 );
    boxMaterial.release();
    boxMesh.release();
    root.attachChild( boxGeometry );
    //! [typical_scene_setup]

    //! [typical_renderer_invocation]
    fr.render( *cam );
    //! [typical_renderer_invocation]

    VERIFY_FRAMEBUFFER( testFramebuffer );
}
