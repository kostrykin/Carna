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

#include <LibCarna/LibCarna.h>

namespace Carna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// ParallaxStageIntegrationTest
// ----------------------------------------------------------------------------------

/** \brief
  * Integration-tests of the \ref Carna::presets::ParallaxStage class.
  *
  * \author Leonid Kostrykin
  */
class ParallaxStageIntegrationTest : public QObject
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

    void test_aside();

    void test_interleaved();

 // ---------------------------------------------------------------------------------

private:

    const static unsigned int GEOMETRY_TYPE_OPAQUE = 1;

    std::unique_ptr< QGLContextHolder > qglContextHolder;
    std::unique_ptr< TestFramebuffer > testFramebuffer;
    std::unique_ptr< TestScene > scene;
    std::unique_ptr< base::FrameRenderer > renderer;
    
    presets::ParallaxStage* parallax;

}; // ParallaxStageIntegrationTest



}  // namespace testing

}  // namespace Carna
