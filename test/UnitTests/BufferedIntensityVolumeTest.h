/*
 *  Copyright (C) 2021 Leonid Kostrykin
 *
 */

#pragma once

#include <LibCarna/LibCarna.h>
#include <LibCarna/base/BufferedIntensityVolume.h>

namespace Carna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// BufferedIntensityVolumeTest
// ----------------------------------------------------------------------------------

/** \brief
  * Unit-tests of the \ref Carna::base::BufferedIntensityVolume class.
  *
  * \author Leonid Kostrykin
  * \date   June 2021
  */
class BufferedIntensityVolumeTest : public QObject
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

    void test_bufferValueToIntensity();

    void test_intensityToBufferValue();

    void test_instantiation();

    void test_parenthesisOperator();

    void test_setVoxel();

 // ---------------------------------------------------------------------------------

private:

    unsigned int indexByPosition( const base::math::Vector3ui& ) const;

    float intensityByIndex( unsigned int index ) const;

    base::IntensityVolumeUInt16::Buffer* bufferPtr;

    std::unique_ptr< base::IntensityVolumeUInt16 > volume;

}; // BufferedIntensityVolumeTest



}  // namespace testing

}  // namespace Carna
