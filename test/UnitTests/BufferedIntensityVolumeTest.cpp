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

#include "BufferedIntensityVolumeTest.hpp"



// ----------------------------------------------------------------------------------
// BufferedIntensityVolumeTest
// ----------------------------------------------------------------------------------

void BufferedIntensityVolumeTest::initTestCase()
{
}


void BufferedIntensityVolumeTest::cleanupTestCase()
{
}


void BufferedIntensityVolumeTest::init()
{
    static_assert( !std::numeric_limits< base::IntensityVolumeUInt16::Voxel >::is_signed, "Type conflict detected." );
}


void BufferedIntensityVolumeTest::cleanup()
{
    volume.reset();
}


void BufferedIntensityVolumeTest::test_instantiation()
{
    const base::math::Vector3ui size( 3, 3, 3 );
    bufferPtr = new base::IntensityVolumeUInt16::Buffer( size.x() * size.y() * size.z() );
    volume.reset( new base::IntensityVolumeUInt16( size, new base::Composition< base::IntensityVolumeUInt16::Buffer >( bufferPtr ) ) );
}


unsigned int BufferedIntensityVolumeTest::indexByPosition( const LibCarna::base::math::Vector3ui& pos ) const
{
    return pos.x() + pos.y() * volume->size.x() + pos.z() * volume->size.x() * volume->size.y();
}


float BufferedIntensityVolumeTest::intensityByIndex( unsigned int index ) const
{
    const unsigned int maxIndex = volume->size.x() * volume->size.y() * volume->size.z() - 1;
    return static_cast< float >( index ) / maxIndex;
}


void BufferedIntensityVolumeTest::test_bufferValueToIntensity()
{
    const float actual   = base::IntensityVolumeUInt16::bufferValueToIntensity( 1 << 15 );
    const float expected = ( 1 << 15 ) / float( ( 1 << 16 ) - 1 );
    QCOMPARE( actual, expected );
    QVERIFY( std::abs( actual - expected ) <= 1e-4 );
}


void BufferedIntensityVolumeTest::test_intensityToBufferValue()
{
    const base::IntensityVolumeUInt16::Voxel actual   = base::IntensityVolumeUInt16::intensityToBufferValue( 0.5 );
    const base::IntensityVolumeUInt16::Voxel expected = static_cast< base::IntensityVolumeUInt16::Voxel >( 0.5 * ( ( 1 << 16 ) - 1 ) + 0.5 );
    QCOMPARE( actual, expected );
    QVERIFY( std::abs( actual - ( 1 << 15 ) ) <= 1e-4 );
}


void BufferedIntensityVolumeTest::test_parenthesisOperator()
{
    test_instantiation();

    /* Prepare volume buffer.
     */
    base::IntensityVolumeUInt16::Buffer& buffer = *bufferPtr;
    base::math::Vector3ui pos;
    for( pos.z() = 0; pos.z() < volume->size.z(); ++pos.z() )
    for( pos.y() = 0; pos.y() < volume->size.y(); ++pos.y() )
    for( pos.x() = 0; pos.x() < volume->size.x(); ++pos.x() )
    {
        const unsigned int index = indexByPosition( pos );
        const auto valueMax = ( 1 << 16 ) - 1;
        float value = intensityByIndex( index ) * valueMax + 0.5f;
        if( value > valueMax ) value = valueMax;
        const base::IntensityVolumeUInt16::Voxel bufferValue = static_cast< base::IntensityVolumeUInt16::Voxel >( value );
        buffer[ index ] = bufferValue;
    }

    /* Validate result from parenthesis operator.
     */
    for( pos.z() = 0; pos.z() < volume->size.z(); ++pos.z() )
    for( pos.y() = 0; pos.y() < volume->size.y(); ++pos.y() )
    for( pos.x() = 0; pos.x() < volume->size.x(); ++pos.x() )
    {
        const unsigned int index = indexByPosition( pos );
        const auto expected = intensityByIndex( index );
        const auto   actual = ( *volume )( pos );
        QVERIFY( std::abs( actual - expected ) <= 1e-4 );
    }
}


void BufferedIntensityVolumeTest::test_setVoxel()
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
        const float value = intensityByIndex( index );
        volume->setVoxel( pos, value );
    }

    /* Validate volume buffer.
     */
    base::IntensityVolumeUInt16::Buffer& buffer = *bufferPtr;
    for( pos.z() = 0; pos.z() < volume->size.z(); ++pos.z() )
    for( pos.y() = 0; pos.y() < volume->size.y(); ++pos.y() )
    for( pos.x() = 0; pos.x() < volume->size.x(); ++pos.x() )
    {
        const unsigned int index = indexByPosition( pos );
        const float expected = intensityByIndex( index );
        const base::IntensityVolumeUInt16::Voxel bufferValue = buffer[ index ];
        const float actual = bufferValue / float( ( 1 << 16 ) - 1 );
        QVERIFY( std::abs( actual - expected ) <= 1e-4 );
    }
}
