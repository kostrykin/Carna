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

#include "HUVolumeGridTest.h"
#include <Carna/base/VolumeGrid.h>



// ----------------------------------------------------------------------------------
// HUVolumeGridTest
// ----------------------------------------------------------------------------------

void HUVolumeGridTest::initTestCase()
{
}


void HUVolumeGridTest::cleanupTestCase()
{
}


void HUVolumeGridTest::init()
{
}


void HUVolumeGridTest::cleanup()
{
    grid.reset();
}


void HUVolumeGridTest::test_instantiation()
{
    grid.reset( new base::VolumeGrid< base::HUVolumeUInt16, void >
        ( base::math::Vector3ui( 10, 10, 10 ), base::math::Vector3ui( 2, 2, 1 ) ) );
}


void HUVolumeGridTest::test_parenthesisOperator()
{
    test_instantiation();

    /* Define mapping from coordinate to HU value.
     */
    const base::math::Vector3ui totalSize = grid->maxSegmentSize.cwiseProduct( grid->segmentCounts );
    const auto coord2HUV = [&totalSize]( const base::math::Vector3ui& totalCoord )->base::HUV
    {
        const unsigned int index = totalCoord.x() + totalCoord.y() * totalSize.x() + totalCoord.z() * totalSize.x() * totalSize.y();
        const base::HUV huv = static_cast< base::HUV >( index % 4096 - 1024 );
        return huv;
    };

    /* Initialize segments with data.
     */
    base::math::Vector3ui segCoord;
    for( segCoord.z() = 0; segCoord.z() < grid->segmentCounts.z(); ++segCoord.z() )
    for( segCoord.y() = 0; segCoord.y() < grid->segmentCounts.y(); ++segCoord.y() )
    for( segCoord.x() = 0; segCoord.x() < grid->segmentCounts.x(); ++segCoord.x() )
    {
        base::VolumeSegment< base::HUVolumeUInt16, void >& segment = grid->segmentAt( segCoord.x(), segCoord.y(), segCoord.z() );
        base::HUVolumeUInt16* const volume = new base::HUVolumeUInt16( grid->maxSegmentSize );
        segment.setHUVolume( new base::Composition< base::HUVolumeUInt16 >( volume ) );

        /* Load segment volume data.
        */
        base::math::Vector3ui localCoord;
        for( localCoord.z() = 0; localCoord.z() < volume->size.z(); ++localCoord.z() )
        for( localCoord.y() = 0; localCoord.y() < volume->size.y(); ++localCoord.y() )
        for( localCoord.x() = 0; localCoord.x() < volume->size.x(); ++localCoord.x() )
        {
            const base::math::Vector3ui totalCoord = localCoord + segCoord.cwiseProduct( grid->maxSegmentSize );
            const base::HUV huv = coord2HUV( totalCoord );
            volume->setVoxel( localCoord, huv );
        }
    }

    /* Test whether the grid is set up properly.
     */
    base::math::Vector3ui totalCoord;
    for( totalCoord.z() = 0; totalCoord.z() < totalSize.z(); ++totalCoord.z() )
    for( totalCoord.y() = 0; totalCoord.y() < totalSize.y(); ++totalCoord.y() )
    for( totalCoord.x() = 0; totalCoord.x() < totalSize.x(); ++totalCoord.x() )
    {
        const base::HUV expected = coord2HUV( totalCoord );
        const base::HUV actual   = grid->getVoxel< base::VolumeGrid< base::HUVolumeUInt16, void >::HUVSelector >( totalCoord );
        QCOMPARE( actual, expected );
    }
}
