/*
 *  Copyright (C) 2021 - 2025 Leonid Kostrykin
 * 
 */

#include "ColorMapTest.hpp"
#include <LibCarna/base/ColorMap.hpp>

namespace LibCarna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// ColorMapTest
// ----------------------------------------------------------------------------------

void ColorMapTest::initTestCase()
{
}


void ColorMapTest::cleanupTestCase()
{
}


void ColorMapTest::init()
{
}


void ColorMapTest::cleanup()
{
}


void ColorMapTest::test_setMinimumIntensity()
{
    base::ColorMap colorMap;
    colorMap.setMaximumIntensity( 0.5f );
    for( float f = 0; f <= 1; f += 0.1f )
    {
        colorMap.setMinimumIntensity( f );
        QCOMPARE( colorMap.minimumIntensity(), f );
        if( f <= 0.5f )
        {
            QCOMPARE( colorMap.maximumIntensity(), 0.5f );
        }
        else
        {
            QCOMPARE( colorMap.maximumIntensity(), f );
        }
    }
}


void ColorMapTest::test_setMaximumIntensity()
{
    base::ColorMap colorMap;
    colorMap.setMinimumIntensity( 0.5f );
    for( float f = 1; f >= 0; f -= 0.1f )
    {
        colorMap.setMaximumIntensity( f );
        QCOMPARE( colorMap.maximumIntensity(), f );
        if( f >= 0.5f )
        {
            QCOMPARE( colorMap.minimumIntensity(), 0.5f );
        }
        else
        {
            QCOMPARE( colorMap.minimumIntensity(), f );
        }
    }
}



}  // namespace LibCarna :: testing

}  // namespace LibCarna
