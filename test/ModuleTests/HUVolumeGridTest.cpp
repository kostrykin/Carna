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
#include <Carna/base/HUVolumeGrid.h>



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
}


void HUVolumeGridTest::test_instantiation()
{
    base::HUVolumeGrid< base::UInt16HUVolume > grid( base::math::Vector3ui( 50, 50, 50 ), base::math::Vector3ui( 3, 3, 1 ) );

    /* Initialize segments with data.
     */
    for( unsigned int segmentZ = 0; segmentZ < grid.segmentCounts.z(); ++segmentZ )
    for( unsigned int segmentY = 0; segmentY < grid.segmentCounts.y(); ++segmentY )
    for( unsigned int segmentX = 0; segmentX < grid.segmentCounts.x(); ++segmentX )
    {
        base::HUVolumeGrid< base::UInt16HUVolume >::HUVolumeSegment& segment = grid.segmentAt( segmentX, segmentY, segmentZ );
    }
}
