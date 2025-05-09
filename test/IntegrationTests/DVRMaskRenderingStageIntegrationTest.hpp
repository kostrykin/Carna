/*
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
// DVRMaskRenderingStageIntegrationTest
// ----------------------------------------------------------------------------------

/** \brief
  * Integration-tests of the \ref LibCarna::presets::DVRStage and the \ref LibCarna::presets::MaskRenderingStage
  * classes.
  *
  * \author Leonid Kostrykin
  */
class DVRMaskRenderingStageIntegrationTest : public QObject
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

    void test_after1Frame();

    void test_after2Frames();

    void test_after3Frames();

// ---------------------------------------------------------------------------------

private:

    typedef helpers::VolumeGridHelper< base::IntensityVolumeUInt8 > MaskGridHelper;

    const static unsigned int GEOMETRY_TYPE_VOLUMETRIC = 0;
    const static unsigned int GEOMETRY_TYPE_MASK       = 1;

    std::unique_ptr< QGLContextHolder > qglContextHolder;
    std::unique_ptr< TestFramebuffer > testFramebuffer;
    std::unique_ptr< TestScene > scene;
    std::unique_ptr< base::FrameRenderer > renderer;
    std::unique_ptr< base::IntensityVolumeUInt8 > mask;
    std::unique_ptr< MaskGridHelper > maskGridHelper;

    presets::DVRStage* dvr;
    presets::MaskRenderingStage* mr;

    void renderFrames( unsigned int n );

}; // DVRMaskRenderingStageIntegrationTest



}  // namespace testing

}  // namespace LibCarna
