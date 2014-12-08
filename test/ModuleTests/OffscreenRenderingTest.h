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
// OffscreenRenderingTest
// ----------------------------------------------------------------------------------

class OffscreenRenderingTest : public QObject
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

    /** \brief  Tests the reference returned by the mutable and immutable \c module methods.
      */
    void testModule();
    
    /** \brief  Tests the width of the offscreen display.
      */
    void testWidth();
    
    /** \brief  Tests the height of the offscreen display.
      */
    void testHeight();

    /** \brief  Tests whether the initial \c OffscreenDisplay state is \em invalidated,
      *         but turns to \em validated by invoking the painting routine.
      */
    void testIsInvalidated();

    /** \brief  Tests whether the GL object ID returned by \c textureID references a valid GL texture object.
      */
    void testTextureID();

    /** \brief  Tests whether the \c OffscreenDisplay object properly transits from
      *         \em validated into \em invalidated state by an \c invalidate invocation.
      */
    void testInvalidate();

    /** \brief  Tests the result of an invocation to \c paint.
      */
    void testPaint();
    
    /** \brief  Tests the result of sequential invocations to \c paint on multiple \c OffscreenDisplay instances.
      */
    void testMultipleInstances();

    /** \brief  Tests whether the the \c OffscreenDisplay object repaints as rarely as possible, yet as
      *         often as necessary, when using the implicit rendering logic of the \c framebuffer method.
      */
    void testMinimalRepainting();

    /** \brief  Tests whether any given reference to the object returned by \c framebuffer stays valid
      *         even when the image is repainted or the framebuffer gets resized.
      */
    void testFramebufferImageAddressConsistency();

    /** \brief  Tests the \c resize method according to its documentation.
      */
    void testResize();

 // ----------------------------------------------------------------------------------

private:

 /* The attributes below are initialized once per test case.
  */

    std::unique_ptr< Carna::base::model::Scene > scene;
    std::unique_ptr< Carna::base::view::SceneProvider > provider;
    std::unique_ptr< Carna::base::OffscreenVisualizationEnvironment > environment;

 // ----------------------------------------------------------------------------------

 /* The attributes below are initialized at least once per test function.
  */

    std::unique_ptr< Carna::base::OffscreenDisplay > display;

    Carna::base::Visualization* module;
    
    /** \brief  Holds the width of the offscreen display and it's framebuffer that is supposed to be correct.
      */
    int displayWidth;

    /** \brief  Holds the height of the offscreen display and it's framebuffer that is supposed to be correct.
      */
    int displayHeight;

 // ----------------------------------------------------------------------------------

 /* Helper functions are declared below.
  */

    /** \brief  Verifies the width and height of the image returned by \c framebuffer.
      */
    void verifyFramebufferSize();

    static void OffscreenRenderingTest::verifyAnotherScene
        ( unsigned int scene_width
        , unsigned int scene_height
        , unsigned int scene_depth
        , double voxels_per_mm
        , unsigned int display_width
        , unsigned int display_height
        , const std::string& expected_filename );

    static void OffscreenRenderingTest::verifyAnotherScene
        ( const std::string& hugz_filename
        , unsigned int display_width
        , unsigned int display_height
        , const std::string& expected_filename );

    static void OffscreenRenderingTest::verifyAnotherScene
        ( const Carna::base::model::Scene* scene_ptr
        , unsigned int display_width
        , unsigned int display_height
        , const std::string& expected_filename );
    
}; // OffscreenRenderingTest



}  // namespace testing

}  // namespace Carna
