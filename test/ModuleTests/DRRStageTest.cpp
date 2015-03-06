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

#include "DRRStageTest.h"
#include <Carna/base/Node.h>
#include <Carna/base/FrameRenderer.h>
#include <Carna/presets/DRRStage.h>



// ----------------------------------------------------------------------------------
// DRRStageTest
// ----------------------------------------------------------------------------------

void DRRStageTest::initTestCase()
{
    qglContextHolder.reset( new QGLContextHolder() );
    scene.reset( new TestScene( qglContextHolder->glContext() ) );
    renderer.reset( new base::FrameRenderer( qglContextHolder->glContext(), 640, 480, true ) );

    drr = new presets::DRRStage( TestScene::GEOMETRY_TYPE_VOLUMETRIC );
    renderer->appendStage( drr );
}


void DRRStageTest::cleanupTestCase()
{
    renderer.reset();
    scene.reset();
    qglContextHolder.reset();
}


void DRRStageTest::init()
{
}


void DRRStageTest::cleanup()
{
}


void DRRStageTest::testDefault()
{
}


void DRRStageTest::testInversed()
{
}
