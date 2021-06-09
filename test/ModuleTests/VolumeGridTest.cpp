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

#include "VolumeGridTest.h"
#include <Carna/base/VolumeGrid.h>



// ----------------------------------------------------------------------------------
// VolumeGridTest
// ----------------------------------------------------------------------------------

void VolumeGridTest::initTestCase()
{
}


void VolumeGridTest::cleanupTestCase()
{
}


void VolumeGridTest::init()
{
}


void VolumeGridTest::cleanup()
{
    grid.reset();
}


void VolumeGridTest::test_instantiation()
{
    grid.reset( new base::VolumeGrid< base::IntensityVolumeUInt16, void >
        ( base::math::Vector3ui( 10, 10, 10 ), base::math::Vector3ui( 2, 2, 1 ) ) );
}


void VolumeGridTest::test_parenthesisOperator()
{
    test_instantiation();

    /* Define mapping from coordinate to intensity value.
     */
    const base::math::Vector3ui totalSize = grid->maxSegmentSize.cwiseProduct( grid->segmentCounts );
    const auto coord2float = [&totalSize]( const base::math::Vector3ui& totalCoord )->float
    {
        const unsigned int index = totalCoord.x() + totalCoord.y() * totalSize.x() + totalCoord.z() * totalSize.x() * totalSize.y();
        const unsigned int maxIndex = ( 1 << 16 ) - 1;
        const float value = ( index % maxIndex ) / float( maxIndex );
        return value;
    };

    /* Initialize segments with data.
     */
    base::math::Vector3ui segCoord;
    for( segCoord.z() = 0; segCoord.z() < grid->segmentCounts.z(); ++segCoord.z() )
    for( segCoord.y() = 0; segCoord.y() < grid->segmentCounts.y(); ++segCoord.y() )
    for( segCoord.x() = 0; segCoord.x() < grid->segmentCounts.x(); ++segCoord.x() )
    {
        base::VolumeSegment< base::IntensityVolumeUInt16, void >& segment = grid->segmentAt( segCoord.x(), segCoord.y(), segCoord.z() );
        base::IntensityVolumeUInt16* const volume = new base::IntensityVolumeUInt16( grid->maxSegmentSize );
        segment.setIntensityVolume( new base::Composition< base::IntensityVolumeUInt16 >( volume ) );

        /* Load segment volume data.
        */
        base::math::Vector3ui localCoord;
        for( localCoord.z() = 0; localCoord.z() < volume->size.z(); ++localCoord.z() )
        for( localCoord.y() = 0; localCoord.y() < volume->size.y(); ++localCoord.y() )
        for( localCoord.x() = 0; localCoord.x() < volume->size.x(); ++localCoord.x() )
        {
            const base::math::Vector3ui totalCoord = localCoord + segCoord.cwiseProduct( grid->maxSegmentSize );
            const float value = coord2float( totalCoord );
            volume->setVoxel( localCoord, value );
        }
    }

    /* Test whether the grid is set up properly.
     */
    base::math::Vector3ui totalCoord;
    for( totalCoord.z() = 0; totalCoord.z() < totalSize.z(); ++totalCoord.z() )
    for( totalCoord.y() = 0; totalCoord.y() < totalSize.y(); ++totalCoord.y() )
    for( totalCoord.x() = 0; totalCoord.x() < totalSize.x(); ++totalCoord.x() )
    {
        const float expected = coord2float( totalCoord );
        const float actual   = grid->getVoxel< base::VolumeGrid< base::IntensityVolumeUInt16, void >::IntensitySelector >( totalCoord );
        QCOMPARE( actual, expected );
    }
}
