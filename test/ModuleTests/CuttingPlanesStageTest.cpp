/*
 *  Copyright (C) 2010 - 2014 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#include "CuttingPlanesStageTest.h"
#include <Carna/base/Node.h>
#include <Carna/base/Geometry.h>
#include <Carna/base/FrameRenderer.h>
#include <Carna/presets/CuttingPlanesStage.h>



// ----------------------------------------------------------------------------------
// CuttingPlanesStageTest
// ----------------------------------------------------------------------------------

void CuttingPlanesStageTest::initTestCase()
{
    const unsigned int width  = 640;
    const unsigned int height = 480;

    qglContextHolder.reset( new QGLContextHolder() );
    testFramebuffer.reset( new TestFramebuffer( qglContextHolder->glContext(), width, height ) );
    scene.reset( new TestScene( qglContextHolder->glContext() ) );
    renderer.reset( new base::FrameRenderer( qglContextHolder->glContext(), width, height, true ) );
    
    /* Rotate camera.
     */
    scene->cam().localTransform = base::math::rotation4f( 0, 1, 0, base::math::deg2rad( 45 ) ) * scene->cam().localTransform;

    planes = new presets::CuttingPlanesStage( TestScene::GEOMETRY_TYPE_VOLUMETRIC, GEOMETRY_TYPE_CUTTING_PLANE );
    renderer->appendStage( planes );

    QCOMPARE( planes->windowingLevel(), presets::CuttingPlanesStage::DEFAULT_WINDOWING_LEVEL );
    QCOMPARE( planes->windowingWidth(), presets::CuttingPlanesStage::DEFAULT_WINDOWING_WIDTH );
}


void CuttingPlanesStageTest::cleanupTestCase()
{
    renderer.reset();
    scene.reset();
    testFramebuffer.reset();
    qglContextHolder.reset();
}


void CuttingPlanesStageTest::init()
{
    planes->setWindowingWidth( 1000 );
    planes->setWindowingLevel( -100 );
}


void CuttingPlanesStageTest::cleanup()
{
}


void CuttingPlanesStageTest::test_noPlanes()
{
    renderer->render( scene->cam(), *scene->root );
    VERIFY_FRAMEBUFFER( *testFramebuffer );
}


void CuttingPlanesStageTest::test_threePlanes()
{
    base::Node* const pivot = new base::Node();
    scene->root->attachChild( pivot );
    pivot->localTransform = base::math::plane4f( base::math::Vector3f( 1, 1, 1 ).normalized(), 0 );

    /* Create the planes.
     */
    base::Geometry* planes[ 3 ];
    for( unsigned int i = 0; i < 3; ++i )
    {
        planes[ i ] = new base::Geometry( GEOMETRY_TYPE_CUTTING_PLANE );
        pivot->attachChild( planes[ i ] );
    }

    /* Configure the planes.
     */
    planes[ 0 ]->localTransform = base::math::plane4f( base::math::Vector3f( 1, 0, 0 ), 0.f );
    planes[ 1 ]->localTransform = base::math::plane4f( base::math::Vector3f( 0, 1, 0 ), 0.f );
    planes[ 2 ]->localTransform = base::math::plane4f( base::math::Vector3f( 0, 0, 1 ), 0.f );

    /* Do the test.
     */
    renderer->render( scene->cam(), *scene->root );
    VERIFY_FRAMEBUFFER( *testFramebuffer );

    /* Remove the planes.
     */
    delete pivot->detachFromParent();
}
