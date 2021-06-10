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

namespace Carna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// BufferedHUVolumeTest
// ----------------------------------------------------------------------------------

/** \brief
  * Unit-tests of the \ref Carna::base::BufferedHUVolume class.
  *
  * \author Leonid Kostrykin
  * \date   22.2.14 - 4.4.15
  */
class BufferedHUVolumeTest : public QObject
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

    void test_bufferValueToHUV();

    void test_HUVToBufferValue();

    void test_instantiation();

#if 0

    void test_parenthesisOperator();

    void test_setVoxel();

#endif

 // ---------------------------------------------------------------------------------

private:

    unsigned int indexByPosition( const base::math::Vector3ui& ) const;

    base::HUV huvByIndex( unsigned int index ) const;

    base::HUVolumeUInt16::Buffer* bufferPtr;

    std::unique_ptr< base::HUVolumeUInt16 > volume;

}; // BufferedHUVolumeTest



}  // namespace testing

}  // namespace Carna
