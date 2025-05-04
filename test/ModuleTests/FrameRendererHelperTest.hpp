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
// FrameRendererHelperTest
// ----------------------------------------------------------------------------------

/** \brief
  * Module-tests of the \ref LibCarna::helpers::FrameRendererHelper class.
  *
  * \author Leonid Kostrykin
  */
class FrameRendererHelperTest : public QObject
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

    void test_emptyCommit();

    void test_complete();

 // ---------------------------------------------------------------------------------

private:

    std::unique_ptr< QGLContextHolder > qglContextHolder;
    std::unique_ptr< base::FrameRenderer > renderer;

}; // FrameRendererHelperTest



}  // namespace testing

}  // namespace LibCarna
