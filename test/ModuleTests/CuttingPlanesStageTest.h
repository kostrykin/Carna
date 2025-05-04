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

namespace LibCarna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// CuttingPlanesStageTest
// ----------------------------------------------------------------------------------

/** \brief
  * Module-tests of the \ref LibCarna::presets::CuttingPlanesStage class.
  *
  * \author Leonid Kostrykin
  */
class CuttingPlanesStageTest : public QObject
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

    void test_noPlanes();

    void test_threePlanes();

 // ---------------------------------------------------------------------------------

private:

    const static unsigned int GEOMETRY_TYPE_CUTTING_PLANE = 1;

    std::unique_ptr< QGLContextHolder > qglContextHolder;
    std::unique_ptr< TestFramebuffer > testFramebuffer;
    std::unique_ptr< TestScene > scene;
    std::unique_ptr< base::FrameRenderer > renderer;
    
    presets::CuttingPlanesStage* planes;

}; // CuttingPlanesStageTest



}  // namespace testing

}  // namespace LibCarna
