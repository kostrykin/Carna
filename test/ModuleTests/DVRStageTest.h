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

#pragma once

#include <Carna/Carna.h>

namespace Carna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// DVRStageTest
// ----------------------------------------------------------------------------------

class DVRStageTest : public QObject
{

    Q_OBJECT

private slots:

    /** \brief  Called before the first test function is executed.
      */
    void initTestCase();

    /** \brief  Called after the last test function is executed.
      */
    void cleanupTestCase();

    /** \brief  Called before each test function is executed
      */
    void init();

    /** \brief  Called after each test function is executed
      */
    void cleanup();

 // ---------------------------------------------------------------------------------

    void test_withLighting();

    void test_withoutLighting();

    void test_withoutColormap();

 // ---------------------------------------------------------------------------------

private:

    const static unsigned int GEOMETRY_TYPE_VOLUMETRIC = 0;

    std::unique_ptr< QGLContextHolder > qglContextHolder;
    std::unique_ptr< TestFramebuffer > testFramebuffer;
    std::unique_ptr< base::Node > root;
    std::unique_ptr< base::FrameRenderer > renderer;
    std::unique_ptr< base::HUVolumeUInt16 > data;

    base::math::Vector3f dataSpacings;

    presets::DVRStage* dvr;
    base::Camera* cam;

}; // DVRStageTest



}  // namespace testing

}  // namespace Carna
