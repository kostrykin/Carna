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

#include "CounterTest.h"
#include <Carna/base/Histogram.h>
#include <numeric>
#include <cstdint>


// ----------------------------------------------------------------------------------
// ConcreteCounterTest
// ----------------------------------------------------------------------------------

template< typename ConcreteCounter >
struct ConcreteCounterTest
{

    static typename ConcreteCounter::KeyType MIN_COUNTER_KEY();
    static typename ConcreteCounter::KeyType MAX_COUNTER_KEY();

    static void run();

private:

    static bool isCounterEmpty( const ConcreteCounter& counter );
    static bool areCountersEqual( const ConcreteCounter& counter1, const ConcreteCounter& counter2 );
    static unsigned int expectedKeyCount( typename ConcreteCounter::ConstReferencedKeyType key );

}; // ConcreteCounterTest


template< typename ConcreteCounter >
bool ConcreteCounterTest< ConcreteCounter >::isCounterEmpty( const ConcreteCounter& counter )
{
    if( counter[ MAX_COUNTER_KEY() ] > 0 )
    {
        return false;
    }

    for( ConcreteCounter::KeyType key = MIN_COUNTER_KEY(); key < MAX_COUNTER_KEY(); ++key )
    {
        if( counter[ key ] > 0 )
        {
            return false;
        }
    }

    return true;
}


template< typename ConcreteCounter >
bool ConcreteCounterTest< ConcreteCounter >::areCountersEqual( const ConcreteCounter& counter1, const ConcreteCounter& counter2 )
{
    if( counter1[ MAX_COUNTER_KEY() ] != counter2[ MAX_COUNTER_KEY() ] )
    {
        return false;
    }

    for( ConcreteCounter::KeyType key = MIN_COUNTER_KEY(); key < MAX_COUNTER_KEY(); ++key )
    {
        if( counter1[ key ] != counter2[ key ] )
        {
            return false;
        }
    }

    return true;
}


template< typename ConcreteCounter >
typename ConcreteCounter::KeyType ConcreteCounterTest< ConcreteCounter >::MIN_COUNTER_KEY()
{
    return std::numeric_limits< typename ConcreteCounter::KeyType >::min();
}


template< typename ConcreteCounter >
typename ConcreteCounter::KeyType ConcreteCounterTest< ConcreteCounter >::MAX_COUNTER_KEY()
{
    return std::numeric_limits< typename ConcreteCounter::KeyType >::max();
}


template< typename ConcreteCounter >
unsigned int ConcreteCounterTest< ConcreteCounter >::expectedKeyCount( typename ConcreteCounter::ConstReferencedKeyType key )
{
    return ( MAX_COUNTER_KEY() - key ) + 1;
}


template< typename ConcreteCounter >
void ConcreteCounterTest< ConcreteCounter >::run()
{
    const static ConcreteCounter::KeyType min = MIN_COUNTER_KEY();
    const static ConcreteCounter::KeyType max = MAX_COUNTER_KEY();

    ConcreteCounter counter;

 // check counter's initial state

    QVERIFY( isCounterEmpty( counter ) );

 // check copy constructor of blank counter

    ConcreteCounter blank_counter_copy( counter );
    QVERIFY( areCountersEqual( counter, blank_counter_copy ) );

 // manipulate counter's state

    counter[ max ] = expectedKeyCount( max );
    for( ConcreteCounter::KeyType voxel_value = min; voxel_value < max; ++voxel_value )
    {
        counter[ voxel_value ] = expectedKeyCount( voxel_value );
    }

 // check counter's manipulated state

    QCOMPARE( counter[ max ], expectedKeyCount( max ) );
    for( ConcreteCounter::KeyType voxel_value = min; voxel_value < max; ++voxel_value )
    {
        QCOMPARE( counter[ voxel_value ], expectedKeyCount( voxel_value ) );
    }

 // check copy constructor of manipulated counter

    ConcreteCounter manipulated_counter_copy( counter );
    QVERIFY( areCountersEqual( counter, manipulated_counter_copy ) );

 // check counter's 'reset' method

    counter.reset();
    QVERIFY( isCounterEmpty( counter ) );

 // check counter's 'reset' method on an already reset counter

    counter.reset();
    QVERIFY( isCounterEmpty( counter ) );

 // check counter's assignment operator for blank counter

    ConcreteCounter assigned_counter;
    assigned_counter = manipulated_counter_copy;
    QVERIFY( areCountersEqual( assigned_counter, manipulated_counter_copy ) );

 // check counter's assignment operator for non-blank counter

    manipulated_counter_copy = blank_counter_copy;
    QVERIFY( isCounterEmpty( manipulated_counter_copy ) );
}



// ----------------------------------------------------------------------------------
// CounterTest
// ----------------------------------------------------------------------------------

void CounterTest::initTestCase()
{
    disableQDebug();
}


void CounterTest::cleanupTestCase()
{
}


void CounterTest::init()
{
}


void CounterTest::cleanup()
{
}


void CounterTest::testIndexedCounter()
{
    try
    {
        ConcreteCounterTest< Carna::base::IndexedCounter< int8_t > >::run();
    }
    catch( const std::exception& ex )
    {
        QFAIL( ex.what() );
    }
}


void CounterTest::testAssociativeCounter()
{
    try
    {
        ConcreteCounterTest< Carna::base::AssociativeCounter< int8_t > >::run();
    }
    catch( const std::exception& ex )
    {
        QFAIL( ex.what() );
    }
}
