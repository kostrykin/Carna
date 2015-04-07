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
#include <Carna/base/math.h>
#include <Carna/presets/OpaqueRenderingStage.h>

namespace Carna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// MeshColorCodingStageTest
// ----------------------------------------------------------------------------------

/** \brief
  * Module-tests of the \ref Carna::presets::MeshColorCodingStage class.
  *
  * \author Leonid Kostrykin
  * \date   22.2.14 - 4.4.15
  */
class MeshColorCodingStageTest : public QObject
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

    void test_fromFront();

    void test_fromBack();

    void test_fromSide();

    void test_atInvalidFrameLocations();

 // ---------------------------------------------------------------------------------

private:

    const static unsigned int GEOMETRY_TYPE_OPAQUE   = 1;

    std::unique_ptr< QGLContextHolder > qglContextHolder;
    std::unique_ptr< TestFramebuffer > testFramebuffer;
    std::unique_ptr< TestScene > scene;
    std::unique_ptr< base::FrameRenderer > renderer;

    base::Geometry* objRed;
    base::Geometry* objGreen;

    presets::OpaqueRenderingStage* opaque;    
    presets::MeshColorCodingStage* mccs;

    base::math::Vector2ui computeFrameLocation( const base::Spatial& ) const;

}; // MeshColorCodingStageTest



}  // namespace testing

}  // namespace Carna
