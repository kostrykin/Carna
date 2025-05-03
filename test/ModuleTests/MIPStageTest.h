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

#include <LibCarna/LibCarna.h>

namespace Carna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// MIPStageTest
// ----------------------------------------------------------------------------------

/** \brief
  * Module-tests of the \ref Carna::presets::MIPStage class.
  *
  * \author Leonid Kostrykin
  * \date   22.2.14 - 4.4.15
  */
class MIPStageTest : public QObject
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

    void test_writeLinearSegment();

    void test_writeLinearSpline();

 // ---------------------------------------------------------------------------------

private:

    std::unique_ptr< QGLContextHolder > qglContextHolder;
    std::unique_ptr< TestFramebuffer > testFramebuffer;
    std::unique_ptr< TestScene > scene;
    std::unique_ptr< base::FrameRenderer > renderer;
    
    presets::MIPStage* mip;

}; // MIPStageTest



}  // namespace testing

}  // namespace Carna
