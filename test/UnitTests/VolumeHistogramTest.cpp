/*
 *  Copyright (C) 2010 - 2014 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#include "VolumeHistogramTest.h"
#include <Carna/base/Histogram.h>
#include <Carna/base/Volume.h>
#include <numeric>
#include <map>



// ----------------------------------------------------------------------------------
// ConcreteCounterVolumeHistogramTest
// ----------------------------------------------------------------------------------

template< typename ConcreteCounter >
struct ConcreteCounterVolumeHistogramTest
{

    typedef typename Carna::base::VolumeHistogram< ConcreteCounter > VolumeHistogram;

    void run() const;

    void run( const std::map< signed short, unsigned int >& frequencies, signed short padding_value ) const;

private:

    static Carna::base::UInt16Volume* createVolume( const std::map< signed short, unsigned int >& frequencies, signed short padding_value );

    static void verifyHistogram( const VolumeHistogram& histogram, const std::map< signed short, unsigned int >& frequencies, signed short padding_value );

}; // ConcreteCounterHistogramTest


template< typename ConcreteCounter >
void ConcreteCounterVolumeHistogramTest< ConcreteCounter >::run() const
{
    std::map< signed short, unsigned int > frequencies;
    std::map< signed short, unsigned int > peaks;

    peaks[ -800 ] = 400;
    peaks[    0 ] = 100;
    peaks[  400 ] = 200;

    for( signed short key = -1024; key <= 3071; ++key )
    {
        const unsigned int max_peak_impact = std::accumulate( peaks.begin(), peaks.end(), 0u, [&]( unsigned int least_max_peak_impact, const std::map< signed short, unsigned int >::value_type& peak )->unsigned int
            {
                return std::max( least_max_peak_impact, static_cast< unsigned int >( std::max( 0, static_cast< signed short >( peak.second ) - std::abs( peak.first - key ) ) ) );
            }
        );
        frequencies[ key ] = max_peak_impact;
    }

    run( frequencies, -1024 );
    run( frequencies,     0 );
    run( frequencies,  3071 );
}


template< typename ConcreteCounter >
void ConcreteCounterVolumeHistogramTest< ConcreteCounter >::run( const std::map< signed short, unsigned int >& frequencies, signed short padding_value ) const
{
    const std::unique_ptr< Carna::base::UInt16Volume > volume( createVolume( frequencies, padding_value ) );
    VolumeHistogram histogram( *volume, volume->size );

    QCOMPARE( &histogram.field.volume, &*volume );

    // check the histogram computation results

    histogram.updateCounter();
    verifyHistogram( histogram, frequencies, padding_value );

    // verify that a second invocation has no impact

    histogram.updateCounter();
    verifyHistogram( histogram, frequencies, padding_value );
}


template< typename ConcreteCounter >
void ConcreteCounterVolumeHistogramTest< ConcreteCounter >::verifyHistogram( const VolumeHistogram& histogram, const std::map< signed short, unsigned int >& frequencies, signed short padding_value )
{
    for( signed short voxel_value = -1024; voxel_value <= 3071; ++voxel_value )
    {
        if( voxel_value != padding_value )
        {
            QCOMPARE( histogram.counter[ voxel_value ], frequencies.at( voxel_value ) );
        }
        else
        {
            QVERIFY( histogram.counter[ voxel_value ] >= frequencies.at( voxel_value ) );
        }
    }
}


template< typename ConcreteCounter >
Carna::base::UInt16Volume* ConcreteCounterVolumeHistogramTest< ConcreteCounter >::createVolume( const std::map< signed short, unsigned int >& frequencies, signed short padding_value )
{
    const unsigned int minimum_voxel_count = std::accumulate
        ( frequencies.begin()
        , frequencies.end()
        , 0u
        , []( unsigned int total, const std::map< Carna::base::UInt16Volume::VoxelType, unsigned int >::value_type& entry )->unsigned int
        {
            return total + entry.second;
        }
    );

    const unsigned int cubic_volume_side_length = static_cast< unsigned int >( std::ceil( std::pow( minimum_voxel_count, 1./3 ) ) );
    const Carna::base::Vector3ui volume_size( cubic_volume_side_length, cubic_volume_side_length, cubic_volume_side_length );
    Carna::base::UInt16Volume::BufferType* const buffer = new Carna::base::UInt16Volume::BufferType( volume_size.x * volume_size.y * volume_size.z, Carna::base::UInt16Volume::HUVToBufferValue( padding_value ) );
    Carna::base::UInt16Volume* const volume = new Carna::base::UInt16Volume( volume_size, new Carna::base::Composition< Carna::base::UInt16Volume::BufferType >( buffer ) );

    unsigned int voxel_offset = 0;
    for( auto frequencies_itr = frequencies.begin(); frequencies_itr != frequencies.end(); ++frequencies_itr )
    {
        const Carna::base::UInt16Volume::VoxelType voxel_value = Carna::base::UInt16Volume::HUVToBufferValue( frequencies_itr->first );
        const unsigned int value_frequency = frequencies_itr->second;

        for( unsigned int voxel_index = 0; voxel_index < value_frequency; ++voxel_index )
        {
            ( *buffer )[ voxel_offset + voxel_index ] = voxel_value;
        }

        voxel_offset += value_frequency;
    }

    return volume;
}



// ----------------------------------------------------------------------------------
// VolumeHistogramTest
// ----------------------------------------------------------------------------------

void VolumeHistogramTest::initTestCase()
{
    disableQDebug();
}


void VolumeHistogramTest::cleanupTestCase()
{
}


void VolumeHistogramTest::init()
{
}


void VolumeHistogramTest::cleanup()
{
}


void VolumeHistogramTest::testWithIndexedCounter()
{
    try
    {
        ConcreteCounterVolumeHistogramTest< Carna::base::IndexedCounter< signed short > >().run();
    }
    catch( const std::out_of_range& ex )
    {
        QFAIL( ex.what() );
    }
}


void VolumeHistogramTest::testWithAssociativeCounter()
{
    try
    {
        ConcreteCounterVolumeHistogramTest< Carna::base::AssociativeCounter< signed short > >().run();
    }
    catch( const std::out_of_range& ex )
    {
        QFAIL( ex.what() );
    }
}
