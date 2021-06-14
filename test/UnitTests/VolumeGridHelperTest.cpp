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

#include "VolumeGridHelperTest.h"
#include <Carna/base/BufferedIntensityVolume.h>
#include <Carna/helpers/VolumeGridHelper.h>



// ----------------------------------------------------------------------------------
// verifyPartitioning
// ----------------------------------------------------------------------------------

template< typename TestedHelperType >
void verifyPartitioning( const TestedHelperType& helper )
{
    CARNA_FOR_VECTOR3UI( coord, helper.grid().segmentCounts )
    {
        const Carna::base::math::Vector3ui& size = helper.grid().segmentAt( coord ).intensities().size;
        QCOMPARE( size.x() % 2, 0u );
        QCOMPARE( size.y() % 2, 0u );
        QCOMPARE( size.z() % 2, 0u );
    }
}



// ----------------------------------------------------------------------------------
// VolumeGridHelperTest
// ----------------------------------------------------------------------------------

void VolumeGridHelperTest::initTestCase()
{
}


void VolumeGridHelperTest::cleanupTestCase()
{
}


void VolumeGridHelperTest::init()
{
}


void VolumeGridHelperTest::cleanup()
{
}


void VolumeGridHelperTest::test_uint16_512x512x71()
{
    const static std::size_t TESTED_MAX_SEGMENT_BYTESIZE = 2 * 300 * 300 * 300;
    typedef helpers::VolumeGridHelper< base::IntensityVolumeUInt16 > TestedHelperType;
    TestedHelperType instance( base::math::Vector3ui( 512, 512, 71 ), TESTED_MAX_SEGMENT_BYTESIZE );
    verifyPartitioning( instance );
}


void VolumeGridHelperTest::test_uint16_173x511x16()
{
    const static std::size_t TESTED_MAX_SEGMENT_BYTESIZE = 2 * 300 * 300 * 300;
    typedef helpers::VolumeGridHelper< base::IntensityVolumeUInt16 > TestedHelperType;
    TestedHelperType instance( base::math::Vector3ui( 173, 511, 16 ), TESTED_MAX_SEGMENT_BYTESIZE );
    verifyPartitioning( instance );
}


void VolumeGridHelperTest::test_uint8_512x512x71()
{
    const static std::size_t TESTED_MAX_SEGMENT_BYTESIZE = 2 * 300 * 300 * 300;
    typedef helpers::VolumeGridHelper< base::IntensityVolumeUInt8 > TestedHelperType;
    TestedHelperType instance( base::math::Vector3ui( 512, 512, 71 ), TESTED_MAX_SEGMENT_BYTESIZE );
    verifyPartitioning( instance );
}


void VolumeGridHelperTest::test_uint8_173x511x16()
{
    const static std::size_t TESTED_MAX_SEGMENT_BYTESIZE = 2 * 300 * 300 * 300;
    typedef helpers::VolumeGridHelper< base::IntensityVolumeUInt8 > TestedHelperType;
    TestedHelperType instance( base::math::Vector3ui( 173, 511, 16 ), TESTED_MAX_SEGMENT_BYTESIZE );
    verifyPartitioning( instance );
}
