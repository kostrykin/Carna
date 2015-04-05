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

#include "ColorTest.h"
#include <Carna/base/Color.h>

namespace Carna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// ColorTest
// ----------------------------------------------------------------------------------

void ColorTest::initTestCase()
{
}


void ColorTest::cleanupTestCase()
{
}


void ColorTest::init()
{
}


void ColorTest::cleanup()
{
}


void ColorTest::test_RED()
{
    QCOMPARE( base::Color::RED.r, static_cast< unsigned char >( 255 ) );
    QCOMPARE( base::Color::RED.g, static_cast< unsigned char >(   0 ) );
    QCOMPARE( base::Color::RED.b, static_cast< unsigned char >(   0 ) );
    QCOMPARE( base::Color::RED.a, static_cast< unsigned char >( 255 ) );
}


void ColorTest::test_copy_ctor()
{
    const base::Color c( base::Color::RED );
    QCOMPARE( c.r, static_cast< unsigned char >( 255 ) );
    QCOMPARE( c.g, static_cast< unsigned char >(   0 ) );
    QCOMPARE( c.b, static_cast< unsigned char >(   0 ) );
    QCOMPARE( c.a, static_cast< unsigned char >( 255 ) );
}


void ColorTest::test_assignment_operator()
{
    base::Color c( 0, 0, 0, 255 );
    c = base::Color::RED;
    
    QCOMPARE( c.r, static_cast< unsigned char >( 255 ) );
    QCOMPARE( c.g, static_cast< unsigned char >(   0 ) );
    QCOMPARE( c.b, static_cast< unsigned char >(   0 ) );
    QCOMPARE( c.a, static_cast< unsigned char >( 255 ) );
}



}  // namespace Carna :: testing

}  // namespace Carna
