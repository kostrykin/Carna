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

#include "BufferedNormalMap3DTest.h"
#include <Carna/base/BufferedNormalMap3D.h>



// ----------------------------------------------------------------------------------
// BufferedNormalMap3DTest
// ----------------------------------------------------------------------------------

void BufferedNormalMap3DTest::initTestCase()
{
}


void BufferedNormalMap3DTest::cleanupTestCase()
{
}


void BufferedNormalMap3DTest::init()
{
    static_assert( std::numeric_limits< base::NormalMap3DInt8::BufferedVectorComponent >::is_signed, "Type conflict detected." );
    static_assert( sizeof( base::NormalMap3DInt8::BufferedVectorComponent ) == 1, "Type conflict detected." );
}


void BufferedNormalMap3DTest::cleanup()
{
    normals.reset();
}


void BufferedNormalMap3DTest::test_instantiation()
{
    const base::math::Vector3ui size( 3, 3, 3 );
    normals.reset( new base::NormalMap3DInt8( size ) );
}


void BufferedNormalMap3DTest::test_bufferComponentToNormalComponent()
{
    QVERIFY( base::NormalMap3DInt8::decodeComponent( -1 ) < 0 );
    QVERIFY( base::NormalMap3DInt8::decodeComponent(  0 ) > 0 );
    
    QCOMPARE( base::NormalMap3DInt8::decodeComponent( -128 ), -1.f );
    QCOMPARE( base::NormalMap3DInt8::decodeComponent( +127 ), +1.f );
}


void BufferedNormalMap3DTest::test_normalComponentToBufferComponent()
{
    QCOMPARE( static_cast< int >( base::NormalMap3DInt8::encodeComponent(  0 ) ),   -1 );
    QCOMPARE( static_cast< int >( base::NormalMap3DInt8::encodeComponent( -1 ) ), -128 );
    QCOMPARE( static_cast< int >( base::NormalMap3DInt8::encodeComponent( +1 ) ), +127 );
}
