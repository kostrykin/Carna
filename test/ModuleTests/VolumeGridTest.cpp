/*
 *  Copyright (C) 2010 - 2016 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 * 
 * 
 *  Copyright (C) 2021 - 2025 Leonid Kostrykin
 * 
 */

#include "VolumeGridTest.hpp"
#include <LibCarna/base/VolumeGrid.hpp>



// ----------------------------------------------------------------------------------
// VolumeGridTestUInt16
// ----------------------------------------------------------------------------------

void VolumeGridTestUInt16::initTestCase()
{
}


void VolumeGridTestUInt16::cleanupTestCase()
{
}


void VolumeGridTestUInt16::init()
{
}


void VolumeGridTestUInt16::cleanup()
{
    grid.reset();
}


void VolumeGridTestUInt16::test_instantiation()
{
    grid.reset( new base::VolumeGrid< base::IntensityVolumeUInt16, void >
        ( base::math::Vector3ui( 10, 10, 10 ), base::math::Vector3ui( 2, 2, 1 ) ) );
}


void VolumeGridTestUInt16::test_parenthesisOperator()
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
        segment.setIntensities( new base::Composition< base::IntensityVolumeUInt16 >( volume ) );

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



// ----------------------------------------------------------------------------------
// VolumeGridTestUInt8
// ----------------------------------------------------------------------------------

void VolumeGridTestUInt8::initTestCase()
{
}


void VolumeGridTestUInt8::cleanupTestCase()
{
}


void VolumeGridTestUInt8::init()
{
}


void VolumeGridTestUInt8::cleanup()
{
    grid.reset();
}


void VolumeGridTestUInt8::test_instantiation()
{
    grid.reset( new base::VolumeGrid< base::IntensityVolumeUInt8, void >
        ( base::math::Vector3ui( 10, 10, 10 ), base::math::Vector3ui( 2, 2, 1 ) ) );
}


void VolumeGridTestUInt8::test_parenthesisOperator1()
{
    test_instantiation();

    /* Define mapping from coordinate to intensity value.
     */
    const base::math::Vector3ui totalSize = grid->maxSegmentSize.cwiseProduct( grid->segmentCounts );
    const auto coord2float = [&totalSize]( const base::math::Vector3ui& totalCoord )->float
    {
        const unsigned int index = totalCoord.x() + totalCoord.y() * totalSize.x() + totalCoord.z() * totalSize.x() * totalSize.y();
        const unsigned int maxIndex = ( 1 << 8 ) - 1;
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
        base::VolumeSegment< base::IntensityVolumeUInt8, void >& segment = grid->segmentAt( segCoord.x(), segCoord.y(), segCoord.z() );
        base::IntensityVolumeUInt8* const volume = new base::IntensityVolumeUInt8( grid->maxSegmentSize );
        segment.setIntensities( new base::Composition< base::IntensityVolumeUInt8 >( volume ) );

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
        const float actual   = grid->getVoxel< base::VolumeGrid< base::IntensityVolumeUInt8, void >::IntensitySelector >( totalCoord );
        QCOMPARE( actual, expected );
    }
}


void VolumeGridTestUInt8::test_parenthesisOperator2()
{
    grid.reset( new base::VolumeGrid< base::IntensityVolumeUInt8, void >
        ( base::math::Vector3ui( 10, 10, 10 ), base::math::Vector3ui( 1, 1, 1 ) ) );

    /* Define mapping from coordinate to intensity value.
     */
    const base::math::Vector3ui totalSize = grid->maxSegmentSize.cwiseProduct( grid->segmentCounts );
    const auto coord2float = [&totalSize]( const base::math::Vector3ui& totalCoord )->float
    {
        const unsigned int index = totalCoord.x() + totalCoord.y() * totalSize.x() + totalCoord.z() * totalSize.x() * totalSize.y();
        const unsigned int maxIndex = ( 1 << 8 ) - 1;
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
        base::VolumeSegment< base::IntensityVolumeUInt8, void >& segment = grid->segmentAt( segCoord.x(), segCoord.y(), segCoord.z() );
        base::IntensityVolumeUInt8* const volume = new base::IntensityVolumeUInt8( grid->maxSegmentSize );
        segment.setIntensities( new base::Composition< base::IntensityVolumeUInt8 >( volume ) );

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
        const float actual   = grid->getVoxel< base::VolumeGrid< base::IntensityVolumeUInt8, void >::IntensitySelector >( totalCoord );
        QCOMPARE( actual, expected );
    }
}
