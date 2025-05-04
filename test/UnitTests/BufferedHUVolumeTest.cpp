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

#include "BufferedHUVolumeTest.hpp"
#include <iostream>



// ----------------------------------------------------------------------------------
// BufferedHUVolumeTest
// ----------------------------------------------------------------------------------

void BufferedHUVolumeTest::initTestCase()
{
}


void BufferedHUVolumeTest::cleanupTestCase()
{
}


void BufferedHUVolumeTest::init()
{
    static_assert( !std::numeric_limits< base::HUVolumeUInt16::Voxel >::is_signed, "Type conflict detected." );
}


void BufferedHUVolumeTest::cleanup()
{
    volume.reset();
}


void BufferedHUVolumeTest::test_instantiation()
{
    const base::math::Vector3ui size( 3, 3, 3 );
    bufferPtr = new base::HUVolumeUInt16::Buffer( size.x() * size.y() * size.z() );
    volume.reset( new base::HUVolumeUInt16( size, new base::Composition< base::HUVolumeUInt16::Buffer >( bufferPtr ) ) );
}


unsigned int BufferedHUVolumeTest::indexByPosition( const LibCarna::base::math::Vector3ui& pos ) const
{
    return pos.x() + pos.y() * volume->size.x() + pos.z() * volume->size.x() * volume->size.y();
}


LibCarna::base::HUV BufferedHUVolumeTest::huvByIndex( unsigned int index ) const
{
    const unsigned int maxIndex = volume->size.x() * volume->size.y() * volume->size.z() - 1;
    const float linear = static_cast< float >( index ) / maxIndex;
    const base::HUV huv = base::HUV::abs( -1024 + 4095 * linear );
    return huv;
}


void BufferedHUVolumeTest::test_bufferValueToHUV()
{
    const base::HUV actual   = base::HUVolumeUInt16::bufferValueToHUV( 2048 * 16 );
    const base::HUV expected = base::HUV::abs( 1024 );
    QCOMPARE( actual, expected );
}


void BufferedHUVolumeTest::test_HUVToBufferValue()
{
    const base::HUVolumeUInt16::Voxel actual   = base::HUVolumeUInt16::HUVToBufferValue( base::HUV::abs( 1024 ) );
    const base::HUVolumeUInt16::Voxel expected = 2048 * 16;
    QCOMPARE( actual, expected );
}


#if 0
void BufferedHUVolumeTest::test_parenthesisOperator()
{
    test_instantiation();

    /* Prepare volume buffer.
     */
    base::HUVolumeUInt16::Buffer& buffer = *bufferPtr;
    base::math::Vector3ui pos;
    for( pos.z() = 0; pos.z() < volume->size.z(); ++pos.z() )
    for( pos.y() = 0; pos.y() < volume->size.y(); ++pos.y() )
    for( pos.x() = 0; pos.x() < volume->size.x(); ++pos.x() )
    {
        const unsigned int index = indexByPosition( pos );
        const base::HUV huv = huvByIndex( index );
        const base::HUVolumeUInt16::Voxel bufferValue = 16 * static_cast< base::HUVolumeUInt16::Voxel >( 1024 + huv );
        buffer[ index ] = bufferValue;
    }

    /* Validate result from parenthesis operator.
     */
    for( pos.z() = 0; pos.z() < volume->size.z(); ++pos.z() )
    for( pos.y() = 0; pos.y() < volume->size.y(); ++pos.y() )
    for( pos.x() = 0; pos.x() < volume->size.x(); ++pos.x() )
    {
        const unsigned int index = indexByPosition( pos );
        const base::HUV expected = huvByIndex( index );
        const base::HUV   actual = ( *volume )( pos );
        QVERIFY( actual, expected );
    }
}


void BufferedHUVolumeTest::test_setVoxel()
{
    test_instantiation();

    /* Invoke 'setVoxel' method.
     */
    base::math::Vector3ui pos;
    for( pos.z() = 0; pos.z() < volume->size.z(); ++pos.z() )
    for( pos.y() = 0; pos.y() < volume->size.y(); ++pos.y() )
    for( pos.x() = 0; pos.x() < volume->size.x(); ++pos.x() )
    {
        const unsigned int index = indexByPosition( pos );
        const    base::HUV   huv = huvByIndex( index );
        volume->setVoxel( pos, huv );
    }

    /* Validate volume buffer.
     */
    base::HUVolumeUInt16::Buffer& buffer = *bufferPtr;
    for( pos.z() = 0; pos.z() < volume->size.z(); ++pos.z() )
    for( pos.y() = 0; pos.y() < volume->size.y(); ++pos.y() )
    for( pos.x() = 0; pos.x() < volume->size.x(); ++pos.x() )
    {
        const unsigned int index = indexByPosition( pos );
        const base::HUV expected = huvByIndex( index );
        const base::HUVolumeUInt16::Voxel bufferValue = buffer[ index ];
        const base::HUV actual = static_cast< const base::HUV >( bufferValue / 16 ) - 1024;
        QCOMPARE( actual, expected );
    }
}
#endif
