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


void HUVTest::test_HUV_rel()
{
    QCOMPARE( int( base::HUV::rel(  100 ).value ),  100 );
    QCOMPARE( int( base::HUV::rel( -100 ).value ), -100 );
    QCOMPARE(      base::HUV::rel(  100 ).relIntensity(), 100 / 4095.f );
}


void HUVTest::test_HUV_abs()
{
    QCOMPARE( int( base::HUV::abs(  100 ).value ),  100 );
    QCOMPARE( int( base::HUV::abs( -100 ).value ), -100 );
    QCOMPARE(      base::HUV::abs( -100 ).absIntensity(), ( -100 + 1024 ) / 4095.f );
}



}  // namespace LibCarna :: testing

}  // namespace LibCarna
