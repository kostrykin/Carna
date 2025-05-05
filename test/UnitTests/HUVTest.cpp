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

#include "HUVTest.hpp"
#include <LibCarna/base/HUV.hpp>

namespace LibCarna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// HUVTest
// ----------------------------------------------------------------------------------

void HUVTest::initTestCase()
{
}


void HUVTest::cleanupTestCase()
{
}


void HUVTest::init()
{
}


void HUVTest::cleanup()
{
}


void HUVTest::test_HUVOffset()
{
    /* Test HUV constructor.
     */
    QCOMPARE( int( base::HUVOffset(  100 ).value ),  100 );
    QCOMPARE( int( base::HUVOffset( -100 ).value ), -100 );
    QCOMPARE( int( base::HUVOffset(  5000 ).value ), +4095 );
    QCOMPARE( int( base::HUVOffset( -5000 ).value ), -4095 );

    /* Test intensity constructor.
     */
    QCOMPARE( int( base::HUVOffset( +0.5f ).value ), +4095 / 2 );
    QCOMPARE( int( base::HUVOffset( -0.5f ).value ), -4095 / 2 );
    QCOMPARE( int( base::HUVOffset( +1.5f ).value ), +4095 );
    QCOMPARE( int( base::HUVOffset( -1.5f ).value ), -4095 );

    /* Test `.intensity()` method.
     */
    QCOMPARE( base::HUVOffset(  100 ).intensity(), 100 / 4095.f );
}


void HUVTest::test_HUV()
{
    /* Test HUV constructor.
     */
    QCOMPARE( int( base::HUV(  100 ).value ),  100 );
    QCOMPARE( int( base::HUV( -100 ).value ), -100 );
    QCOMPARE( int( base::HUV( +4000 ).value ), +3071 );
    QCOMPARE( int( base::HUV( -2000 ).value ), -1024 );

    /* Test intensity constructor.
     */
    QCOMPARE( int( base::HUV( 0.75f ).value ), 1023 * 3 );
    QCOMPARE( int( base::HUV( 0.25f ).value ), 1023 );
    QCOMPARE( int( base::HUV( +1.5f ).value ), +3071 );
    QCOMPARE( int( base::HUV( -0.5f ).value ), -1024 );

    /* Test `.intensity()` method.
     */
    QCOMPARE( base::HUV( -100 ).intensity(), ( -100 + 1024 ) / 4095.f );
}



}  // namespace LibCarna :: testing

}  // namespace LibCarna
