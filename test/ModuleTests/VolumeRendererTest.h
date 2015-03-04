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

#pragma once

#include <Carna/Carna.h>

namespace Carna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// VolumeRendererTest
// ----------------------------------------------------------------------------------

class VolumeRendererTest : public QObject
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

 // ----------------------------------------------------------------------------------

    void testDRR();

    void testMIP();

    void testDVR();

 // ----------------------------------------------------------------------------------

private:

 /* The attributes below are initialized once per test case.
  */

    std::unique_ptr< Carna::base::Scene > scene;
    std::unique_ptr< Carna::base::SceneProvider > provider;
    std::unique_ptr< Carna::base::OffscreenVisualizationEnvironment > environment;

 // ----------------------------------------------------------------------------------

 /* The attributes below are initialized at least once per test function.
  */

    std::unique_ptr< Carna::base::OffscreenDisplay > display;

    Carna::VolumeRenderings::VolumeVisualization* module;
    
}; // VolumeRendererTest



}  // namespace testing

}  // namespace Carna
