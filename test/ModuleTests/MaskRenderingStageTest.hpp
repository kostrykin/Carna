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

#pragma once

#include <LibCarna/LibCarna.hpp>

namespace LibCarna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// MaskRenderingStageTest
// ----------------------------------------------------------------------------------

/** \brief
  * Module-tests of the \ref LibCarna::presets::MaskRenderingStage class.
  *
  * \author Leonid Kostrykin
  */
class MaskRenderingStageTest : public QObject
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

    void test_shared_geometry_node();

    void test_dedicated_geometry_node();

 // ---------------------------------------------------------------------------------

private:

    const static unsigned int GEOMETRY_TYPE_VOLUMETRIC = 0;

    std::unique_ptr< QGLContextHolder > qglContextHolder;
    std::unique_ptr< TestFramebuffer > testFramebuffer;
    std::unique_ptr< TestScene > scene;
    std::unique_ptr< base::FrameRenderer > renderer;
    std::unique_ptr< base::IntensityVolumeUInt8 > mask;
    
    presets::MaskRenderingStage* mr;

}; // MaskRenderingStageTest



}  // namespace testing

}  // namespace LibCarna
