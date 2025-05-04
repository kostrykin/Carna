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

#include "PointMarkerHelperTest.h"
#include <LibCarna/base/Node.h>
#include <LibCarna/base/FrameRenderer.h>
#include <LibCarna/helpers/PointMarkerHelper.h>



// ----------------------------------------------------------------------------------
// PointMarkerHelperTest
// ----------------------------------------------------------------------------------

void PointMarkerHelperTest::initTestCase()
{
    const unsigned int width  = 640;
    const unsigned int height = 480;

    qglContextHolder.reset( new QGLContextHolder() );
    testFramebuffer.reset( new TestFramebuffer( qglContextHolder->glContext(), width, height ) );
    renderer.reset( new base::FrameRenderer( qglContextHolder->glContext(), width, height, true ) );
    
    /* Configure opaque rendering stage.
     */
    opaque = new presets::OpaqueRenderingStage( GEOMETRY_TYPE_OPAQUE );
    renderer->appendStage( opaque );

    /* Configure camera.
     */
    cam.reset( new base::Camera() );
    cam->localTransform = base::math::translation4f( 0, 0, 350 );
    cam->setProjection( base::math::frustum4f( base::math::deg2rad( 90 ), 1, 10, 2000 ) );
}


void PointMarkerHelperTest::cleanupTestCase()
{
    renderer.reset();
    testFramebuffer.reset();
    qglContextHolder.reset();
}


void PointMarkerHelperTest::init()
{
    root.reset( new base::Node() );
    root->attachChild( cam.get() );
    helpers::PointMarkerHelper::resetDefaultColor();
}


void PointMarkerHelperTest::cleanup()
{
    cam->detachFromParent();
    root.reset();
}


void PointMarkerHelperTest::test_multiple()
{
    //! [multiple]
    helpers::PointMarkerHelper markers( GEOMETRY_TYPE_OPAQUE );

    const float maxOffset = 300;
    const unsigned int markersCount = 10;
    for( unsigned int i = 0; i < markersCount; ++i )
    {
        const float x = -maxOffset + i * 2 * maxOffset / ( markersCount - 1 );
        base::Geometry* const marker = markers.createPointMarker();
        marker->localTransform = base::math::translation4f( x, 0, 0 );
        root->attachChild( marker );
    }
    //! [multiple]

    /* Render and verify.
     */
    renderer->render( *cam, *root );
    VERIFY_FRAMEBUFFER( *testFramebuffer );
}


void PointMarkerHelperTest::test_fixed_color()
{
    helpers::PointMarkerHelper markers( GEOMETRY_TYPE_OPAQUE );

    const float maxOffset = 300;
    const unsigned int markersCount = 10;
    for( unsigned int i = 0; i < markersCount; ++i )
    {
        const float x = -maxOffset + i * 2 * maxOffset / ( markersCount - 1 );
        base::Geometry* marker;
        if( i <= markersCount / 2 )
        {
            const unsigned int j = ( 255 * i / ( markersCount / 2 ) );
            const base::Color fixedColor( 255 - j, 0, j, 255 );
            marker = markers.createPointMarker( fixedColor );
        }
        else
        {
            marker = markers.createPointMarker();;
        }
        marker->localTransform = base::math::translation4f( x, 0, 0 );
        root->attachChild( marker );
    }

    /* Render and verify.
     */
    renderer->render( *cam, *root );
    VERIFY_FRAMEBUFFER( *testFramebuffer );
}
