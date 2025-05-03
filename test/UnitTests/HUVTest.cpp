/*
 *  Copyright (C) 2021 Leonid Kostrykin
 *
 */

#include "HUVTest.h"
#include <LibCarna/base/HUV.h>

namespace Carna
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



}  // namespace Carna :: testing

}  // namespace Carna
