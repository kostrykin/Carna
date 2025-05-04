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

#include "FrameRendererIntegrationTest.hpp"
#include <LibCarna/base/Node.hpp>
#include <LibCarna/base/Geometry.hpp>
#include <LibCarna/base/FrameRenderer.hpp>
#include <LibCarna/presets/CuttingPlanesStage.hpp>
#include <LibCarna/helpers/VolumeGridHelper.hpp>



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

    using namespace LibCarna;

    base::FrameRenderer fr( glContext, 800, 600, true );
    helpers::FrameRendererHelper< > frHelper( fr );
    frHelper << new presets::CuttingPlanesStage( GEOMETRY_TYPE_VOLUMETRIC, GEOMETRY_TYPE_PLANE );
    frHelper << new presets::OpaqueRenderingStage( GEOMETRY_TYPE_OPAQUE );
    frHelper << new presets::DRRStage( GEOMETRY_TYPE_VOLUMETRIC );
    frHelper << new presets::OccludedRenderingStage();
    frHelper.commit();

    fr.findStage< presets::CuttingPlanesStage >()->setWindowingWidth( base::HUV::rel( 4000 ).relIntensity() );
    fr.findStage< presets::CuttingPlanesStage >()->setWindowingLevel( base::HUV::abs( 1024 ).absIntensity() );
    fr.findStage< presets::CuttingPlanesStage >()->setRenderingInverse( true );
    fr.findStage< presets::DRRStage >()->setRenderingInverse( true );
    fr.setBackgroundColor( base::Color::WHITE_NO_ALPHA );
    //! [typical_renderer_setup]

    /* Load test volume data.
     */
    base::math::Vector3f spacings;
    const std::unique_ptr< base::IntensityVolumeUInt16 > dataPtr
        ( HUGZSceneFactory::importVolume( SOURCE_PATH + "/res/pelves_reduced.hugz", spacings ) );
    const base::IntensityVolumeUInt16& data = *dataPtr;

    //! [typical_scene_setup]
    base::Node root;

    /* Configure camera.
     */
    base::Camera* const cam = new base::Camera();
    cam->localTransform = base::math::rotation4f( 0, 1, 0, base::math::deg2rad( 20 ) ) * base::math::translation4f( 0, 0, 350 );
    cam->setProjection( base::math::frustum4f( base::math::deg2rad( 90 ), 1, 10, 2000 ) );
    root.attachChild( cam );

    /* Configure geometry node for volume data.
     */
    typedef helpers::VolumeGridHelper< base::IntensityVolumeUInt16, base::NormalMap3DInt8 > UInt16GridHelper;
    UInt16GridHelper gridHelper( data.size );
    //gridHelper.loadIntensities( data ); // TODO: fix this
    gridHelper.loadIntensities( [&data]( const base::math::Vector3ui& c )->float { return data( c ); } );
    root.attachChild( gridHelper.createNode( GEOMETRY_TYPE_VOLUMETRIC, UInt16GridHelper::Spacing( spacings ) ) );

    /* Configure cutting planes.
     */
    base::Geometry* const plane1 = new base::Geometry( GEOMETRY_TYPE_PLANE );
    plane1->localTransform = base::math::plane4f( base::math::Vector3f( 1, 1, 1 ).normalized(), 0 );
    root.attachChild( plane1 );

    /* Configure opaque geometries.
     */
    base::ManagedMeshBase& boxMesh = base::MeshFactory< base::PNVertex >::createBox( 40, 40, 40 );
    base::Material& boxMaterial = base::Material::create( "solid" );
    boxMaterial.setParameter( "color", base::Color::GREEN );
    base::Geometry* const boxGeometry = new base::Geometry( GEOMETRY_TYPE_OPAQUE );
    boxGeometry->putFeature( presets::OpaqueRenderingStage::ROLE_DEFAULT_MATERIAL, boxMaterial );
    boxGeometry->putFeature( presets::OpaqueRenderingStage::ROLE_DEFAULT_MESH, boxMesh );
    boxGeometry->localTransform = base::math::translation4f( 0, -15, 0 );
    root.attachChild( boxGeometry );

    /* Release geometry features.
     */
    boxMesh.release();
    boxMaterial.release();
    gridHelper.releaseGeometryFeatures();
    //! [typical_scene_setup]

    //! [typical_renderer_invocation]
    fr.render( *cam );
    //! [typical_renderer_invocation]

    VERIFY_FRAMEBUFFER( testFramebuffer );
}


void FrameRendererIntegrationTest::test_8bit()
{
    QGLContextHolder glContextHolder;
    base::GLContext& glContext = glContextHolder.glContext();
    TestFramebuffer testFramebuffer( glContext, 800, 600 );

    const static unsigned int GEOMETRY_TYPE_VOLUMETRIC = 0;
    const static unsigned int GEOMETRY_TYPE_PLANE      = 1;
    const static unsigned int GEOMETRY_TYPE_OPAQUE     = 2;

    using namespace LibCarna;

    base::FrameRenderer fr( glContext, 800, 600, true );
    helpers::FrameRendererHelper< > frHelper( fr );
    frHelper << new presets::CuttingPlanesStage( GEOMETRY_TYPE_VOLUMETRIC, GEOMETRY_TYPE_PLANE );
    frHelper << new presets::OpaqueRenderingStage( GEOMETRY_TYPE_OPAQUE );
    frHelper << new presets::DRRStage( GEOMETRY_TYPE_VOLUMETRIC );
    frHelper << new presets::OccludedRenderingStage();
    frHelper.commit();

    fr.findStage< presets::CuttingPlanesStage >()->setWindowingWidth( base::HUV::rel( 4000 ).relIntensity() );
    fr.findStage< presets::CuttingPlanesStage >()->setWindowingLevel( base::HUV::abs( 1024 ).absIntensity() );
    fr.findStage< presets::CuttingPlanesStage >()->setRenderingInverse( true );
    fr.findStage< presets::DRRStage >()->setRenderingInverse( true );
    fr.setBackgroundColor( base::Color::WHITE_NO_ALPHA );

    /* Load test volume data.
     */
    base::math::Vector3f spacings;
    const std::unique_ptr< base::IntensityVolumeUInt16 > dataPtr
        ( HUGZSceneFactory::importVolume( SOURCE_PATH + "/res/pelves_reduced.hugz", spacings ) );
    const base::IntensityVolumeUInt16& data = *dataPtr;

    base::Node root;

    /* Configure camera.
     */
    base::Camera* const cam = new base::Camera();
    cam->localTransform = base::math::rotation4f( 0, 1, 0, base::math::deg2rad( 20 ) ) * base::math::translation4f( 0, 0, 350 );
    cam->setProjection( base::math::frustum4f( base::math::deg2rad( 90 ), 1, 10, 2000 ) );
    root.attachChild( cam );

    /* Configure geometry node for volume data.
     */
    typedef helpers::VolumeGridHelper< base::IntensityVolumeUInt8, base::NormalMap3DInt8 > UInt8HUGridHelper;
    UInt8HUGridHelper gridHelper( data.size );
    //gridHelper.loadIntensities( data ); // TODO: fix this
    gridHelper.loadIntensities( [&data]( const base::math::Vector3ui& c )->float { return data( c ); } );
    root.attachChild( gridHelper.createNode( GEOMETRY_TYPE_VOLUMETRIC, UInt8HUGridHelper::Spacing( spacings ) ) );

    /* Configure cutting planes.
     */
    base::Geometry* const plane1 = new base::Geometry( GEOMETRY_TYPE_PLANE );
    plane1->localTransform = base::math::plane4f( base::math::Vector3f( 1, 1, 1 ).normalized(), 0 );
    root.attachChild( plane1 );

    /* Configure opaque geometries.
     */
    base::ManagedMeshBase& boxMesh = base::MeshFactory< base::PNVertex >::createBox( 40, 40, 40 );
    base::Material& boxMaterial = base::Material::create( "solid" );
    boxMaterial.setParameter( "color", base::Color::GREEN );
    base::Geometry* const boxGeometry = new base::Geometry( GEOMETRY_TYPE_OPAQUE );
    boxGeometry->putFeature( presets::OpaqueRenderingStage::ROLE_DEFAULT_MATERIAL, boxMaterial );
    boxGeometry->putFeature( presets::OpaqueRenderingStage::ROLE_DEFAULT_MESH, boxMesh );
    boxGeometry->localTransform = base::math::translation4f( 0, -15, 0 );
    root.attachChild( boxGeometry );

    /* Release geometry features.
     */
    boxMesh.release();
    boxMaterial.release();
    gridHelper.releaseGeometryFeatures();

    fr.render( *cam );

    VERIFY_FRAMEBUFFER( testFramebuffer );
}
