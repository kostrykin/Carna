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

#include "BufferedHUVolumeTest.h"
#include <Carna/base/UInt16HUVolume.h>



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
    static_assert( !std::numeric_limits< base::UInt16HUVolume::Voxel >::is_signed, "Type conflict detected." );
}


void BufferedHUVolumeTest::cleanup()
{
    volume.reset();
}


void BufferedHUVolumeTest::test_instantiation()
{
    const base::math::Vector3ui size( 3, 3, 3 );
    bufferPtr = new base::UInt16HUVolume::Buffer( size.x() * size.y() * size.z() );
    volume.reset( new base::UInt16HUVolume( size, new base::Composition< base::UInt16HUVolume::Buffer >( bufferPtr ) ) );
}


unsigned int BufferedHUVolumeTest::indexByPosition( const Carna::base::math::Vector3ui& pos ) const
{
    return pos.x() + pos.y() * volume->size.x() + pos.z() * volume->size.x() * volume->size.y();
}


Carna::base::HUV BufferedHUVolumeTest::huvByIndex( unsigned int index ) const
{
    const unsigned int maxIndex = volume->size.x() * volume->size.y() * volume->size.z() - 1;
    const float linear = static_cast< float >( index ) / maxIndex;
    const base::HUV huv = static_cast< base::HUV >( -1024 + 4095 * linear );
    return huv;
}


void BufferedHUVolumeTest::test_bufferValueToHUV()
{
    const base::HUV actual   = base::UInt16HUVolume::bufferValueToHUV( 2048 * 16 );
    const base::HUV expected = 1024;
    QCOMPARE( actual, expected );
}


void BufferedHUVolumeTest::test_HUVToBufferValue()
{
    const base::UInt16HUVolume::Voxel actual   = base::UInt16HUVolume::HUVToBufferValue( 1024 );
    const base::UInt16HUVolume::Voxel expected = 2048 * 16;
    QCOMPARE( actual, expected );
}


void BufferedHUVolumeTest::test_parenthesisOperator()
{
    test_instantiation();

    /* Prepare volume buffer.
     */
    base::UInt16HUVolume::Buffer& buffer = *bufferPtr;
    base::math::Vector3ui pos;
    for( pos.z() = 0; pos.z() < volume->size.z(); ++pos.z() )
    for( pos.y() = 0; pos.y() < volume->size.y(); ++pos.y() )
    for( pos.x() = 0; pos.x() < volume->size.x(); ++pos.x() )
    {
        const unsigned int index = indexByPosition( pos );
        const base::HUV huv = huvByIndex( index );
        const base::UInt16HUVolume::Voxel bufferValue = 16 * static_cast< base::UInt16HUVolume::Voxel >( 1024 + huv );
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
        QCOMPARE( actual, expected );
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
    base::UInt16HUVolume::Buffer& buffer = *bufferPtr;
    for( pos.z() = 0; pos.z() < volume->size.z(); ++pos.z() )
    for( pos.y() = 0; pos.y() < volume->size.y(); ++pos.y() )
    for( pos.x() = 0; pos.x() < volume->size.x(); ++pos.x() )
    {
        const unsigned int index = indexByPosition( pos );
        const base::HUV expected = huvByIndex( index );
        const base::UInt16HUVolume::Voxel bufferValue = buffer[ index ];
        const base::HUV actual = static_cast< const base::HUV >( bufferValue / 16 ) - 1024;
        QCOMPARE( actual, expected );
    }
}
