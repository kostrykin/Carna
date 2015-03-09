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

    void test_parenthesisOperator();

    void test_setVoxel();

 // ---------------------------------------------------------------------------------

private:

    unsigned int indexByPosition( const base::math::Vector3ui& ) const;

    base::HUV huvByIndex( unsigned int index ) const;

    base::UInt16HUVolume::BufferType* bufferPtr;

    std::unique_ptr< base::UInt16HUVolume > volume;

}; // BufferedHUVolumeTest



}  // namespace testing

}  // namespace Carna
