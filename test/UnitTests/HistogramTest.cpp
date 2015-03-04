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

#include "HistogramTest.h"
#include <Carna/base/Histogram.h>
#include <numeric>
#include <algorithm>
#include <map>



// ----------------------------------------------------------------------------------
// ConcreteCounterHistogramTest
// ----------------------------------------------------------------------------------

template< typename ConcreteCounter, unsigned int scalar_field_dimension >
struct ConcreteCounterHistogramTest
{

    typedef typename ConcreteCounter::KeyType Codomain;

    struct ScalarField
    {

        typedef Carna::base::Histogram< ConcreteCounter, ScalarField, scalar_field_dimension > Histogram;

        typedef typename Histogram::Vector Vector;

        ScalarField( const std::map< Codomain, unsigned int >& frequencies, Codomain padding_value );

        Codomain operator()( const Vector& position ) const;

        const Vector size;

    private:

        std::vector< Codomain > data;

    };  // ScalarField

    typedef typename ScalarField::Histogram Histogram;

    void run() const;

    void run( const std::map< Codomain, unsigned int >& frequencies, Codomain padding_value ) const;

private:

    static void verifyHistogram( const Histogram& histogram, const std::map< Codomain, unsigned int >& frequencies, Codomain padding_value );

}; // ConcreteCounterHistogramTest


template< typename ConcreteCounter, unsigned int scalar_field_dimension >
void ConcreteCounterHistogramTest< ConcreteCounter, scalar_field_dimension >::run() const
{
    std::map< Codomain, unsigned int > frequencies;
    std::map< Codomain, unsigned int > peaks;

    peaks[ std::numeric_limits< Codomain >::min() ] = 100;
    peaks[ std::numeric_limits< Codomain >::max() ] =  80;

    const static auto process_key = [&]( Codomain key )
    {
        typedef Codomain Codomain;
        const unsigned int max_peak_impact = std::accumulate( peaks.begin(), peaks.end(), 0u, [&]( unsigned int least_max_peak_impact, const std::map< Codomain, unsigned int >::value_type& peak )->unsigned int
            {
                return std::max( least_max_peak_impact, static_cast< unsigned int >( std::max( 0, static_cast< Codomain >( peak.second ) - std::abs( peak.first - key ) ) ) );
            }
        );
        frequencies[ key ] = max_peak_impact;
    };

    process_key( std::numeric_limits< Codomain >::max() );
    for( Codomain key = std::numeric_limits< Codomain >::min(); key < std::numeric_limits< Codomain >::max(); ++key )
    {
        process_key( key );
    }

    run( frequencies, std::numeric_limits< Codomain >::min() );
    run( frequencies, 0 );
    run( frequencies, std::numeric_limits< Codomain >::max() );
}


template< typename ConcreteCounter, unsigned int scalar_field_dimension >
void ConcreteCounterHistogramTest< ConcreteCounter, scalar_field_dimension >::run( const std::map< Codomain, unsigned int >& frequencies, Codomain padding_value ) const
{
    const ScalarField field( frequencies, padding_value );
    Histogram histogram( field, field.size );

    QCOMPARE( &histogram.field, &field );

 // check the histogram computation results

    histogram.updateCounter();
    verifyHistogram( histogram, frequencies, padding_value );

 // verify that a second invocation has no impact

    histogram.updateCounter();
    verifyHistogram( histogram, frequencies, padding_value );
}


template< typename ConcreteCounter, unsigned int scalar_field_dimension >
void ConcreteCounterHistogramTest< ConcreteCounter, scalar_field_dimension >::verifyHistogram( const Histogram& histogram, const std::map< Codomain, unsigned int >& frequencies, Codomain padding_value )
{
    if( padding_value != std::numeric_limits< Codomain >::max() )
    {
        QCOMPARE( histogram.counter[ std::numeric_limits< Codomain >::max() ], frequencies.at( std::numeric_limits< Codomain >::max() ) );
    }
    else
    {
        QVERIFY( histogram.counter[ padding_value ] >= frequencies.at( padding_value ) );
    }

    for( Codomain voxel_value = std::numeric_limits< Codomain >::min(); voxel_value < std::numeric_limits< Codomain >::max(); ++voxel_value )
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


// ----------------------------------------------------------------------------------
// ConcreteCounterHistogramTest :: ScalarField
// ----------------------------------------------------------------------------------

template< typename ConcreteCounter, unsigned int scalar_field_dimension >
ConcreteCounterHistogramTest< ConcreteCounter, scalar_field_dimension >::ScalarField::ScalarField
    ( const std::map< Codomain, unsigned int >& frequencies
    , Codomain padding_value )

    : size( [&]()->Vector
        {
            using std::accumulate;
            const unsigned int minimum_voxel_count = accumulate
                ( frequencies.begin()
                , frequencies.end()
                , 0u
                , []( unsigned int total, const std::map< Codomain, unsigned int >::value_type& entry )->unsigned int
            {
                return total + entry.second;
            }
            );

            const unsigned int side_length = static_cast< unsigned int >( std::ceil( std::pow( minimum_voxel_count, 1. / scalar_field_dimension ) ) );
            Vector size;
            for( unsigned int d = 0; d < scalar_field_dimension; ++d )
            {
                size[ d ] = side_length;
            }
            return size;
        }() )
    , data( [&]()->std::size_t
        {
            std::size_t data_size = 1;
            for( unsigned int d = 0; d < scalar_field_dimension; ++d )
            {
                data_size *= size[ d ];
            }
            return data_size;
        }()
        , padding_value )
{
    unsigned int voxel_offset = 0;
    for( auto frequencies_itr = frequencies.begin(); frequencies_itr != frequencies.end(); ++frequencies_itr )
    {
        const Codomain voxel_value = frequencies_itr->first;
        const unsigned int value_frequency = frequencies_itr->second;

        for( unsigned int voxel_index = 0; voxel_index < value_frequency; ++voxel_index )
        {
            data[ voxel_offset + voxel_index ] = voxel_value;
        }

        voxel_offset += value_frequency;
    }
}


template< typename ConcreteCounter, unsigned int scalar_field_dimension >
typename ConcreteCounterHistogramTest< ConcreteCounter, scalar_field_dimension >::Codomain ConcreteCounterHistogramTest< ConcreteCounter, scalar_field_dimension >::ScalarField::operator()( const Vector& position ) const
{
    std::size_t index = position[ 0 ];
    for( unsigned int summand_dimension = 1; summand_dimension < scalar_field_dimension; ++summand_dimension )
    {
        std::size_t summand = position[ summand_dimension ];
        for( unsigned int factor_dimension = 0; factor_dimension != summand_dimension; ++factor_dimension )
        {
            summand *= size[ factor_dimension ];
        }
        index += summand;
    }
    return data.at( index );
}



// ----------------------------------------------------------------------------------
// HistogramTest
// ----------------------------------------------------------------------------------

void HistogramTest::initTestCase()
{
    disableQDebug();
}


void HistogramTest::cleanupTestCase()
{
}


void HistogramTest::init()
{
}


void HistogramTest::cleanup()
{
}


void HistogramTest::testWithIndexedCounter()
{
    try
    {
        ConcreteCounterHistogramTest< Carna::base::IndexedCounter< signed char >, 1u >().run();
        ConcreteCounterHistogramTest< Carna::base::IndexedCounter< signed char >, 2u >().run();
        ConcreteCounterHistogramTest< Carna::base::IndexedCounter< signed char >, 3u >().run();

        ConcreteCounterHistogramTest< Carna::base::IndexedCounter< unsigned char >, 1u >().run();
        ConcreteCounterHistogramTest< Carna::base::IndexedCounter< unsigned char >, 2u >().run();
        ConcreteCounterHistogramTest< Carna::base::IndexedCounter< unsigned char >, 3u >().run();
    }
    catch( const std::out_of_range& ex )
    {
        QFAIL( ex.what() );
    }
}


void HistogramTest::testWithAssociativeCounter()
{
    try
    {
        ConcreteCounterHistogramTest< Carna::base::AssociativeCounter< signed char >, 1u >().run();
        ConcreteCounterHistogramTest< Carna::base::AssociativeCounter< signed char >, 2u >().run();
        ConcreteCounterHistogramTest< Carna::base::AssociativeCounter< signed char >, 3u >().run();

        ConcreteCounterHistogramTest< Carna::base::AssociativeCounter< unsigned char >, 1u >().run();
        ConcreteCounterHistogramTest< Carna::base::AssociativeCounter< unsigned char >, 2u >().run();
        ConcreteCounterHistogramTest< Carna::base::AssociativeCounter< unsigned char >, 3u >().run();
    }
    catch( const std::out_of_range& ex )
    {
        QFAIL( ex.what() );
    }
}
