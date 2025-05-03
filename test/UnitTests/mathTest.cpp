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

#include "mathTest.h"
#include <LibCarna/base/math.h>

namespace Carna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// mathTest
// ----------------------------------------------------------------------------------

void mathTest::initTestCase()
{
}


void mathTest::cleanupTestCase()
{
}


void mathTest::init()
{
}


void mathTest::cleanup()
{
}


void mathTest::test_epsilon()
{
    using namespace Carna::base;

    QCOMPARE( math::epsilon< int >(), 0 );
    QCOMPARE( math::epsilon< unsigned int >(), 0u );

    QVERIFY ( math::epsilon< float >() >= 0 );
    QVERIFY ( math::epsilon< float >() <= 1e-4f );

    QVERIFY ( math::epsilon< double >() >= 0 );
    QVERIFY ( math::epsilon< double >() <= 1e-6 );
}


void mathTest::test_length2()
{
    using namespace Carna::base;

    QCOMPARE( math::length2< signed int >(  0 ), 0 );
    QCOMPARE( math::length2< signed int >( -1 ), 1 );
    QCOMPARE( math::length2< signed int >( -2 ), 4 );
    QCOMPARE( math::length2< signed int >( +2 ), 4 );

    QVERIFY ( math::isEqual< float >( math::length2< float >( -2.5f ), 2.5f * 2.5f ) );

    /* TODO: Below is not working, check why.
     */
    //static_assert( std::is_same< math::length2( math::Vector2f( 0, 0 ) ), float >::value, "Type mismatch." );

    const float test = math::length2( math::Vector2f( 0, 0 ) );
    QVERIFY ( math::isEqual< float >( math::length2( math::Vector2f( 0, 0 ) ), 0 ) );
    QVERIFY ( math::isEqual< float >( math::length2( math::Vector2f( 2, 2 ) ), 8 ) );
}


void mathTest::test_isEqual()
{
    using namespace Carna::base;

    QVERIFY(  math::isEqual(  true,  true ) );
    QVERIFY( !math::isEqual(  true, false ) );
    QVERIFY( !math::isEqual( false,  true ) );
    QVERIFY(  math::isEqual( false, false ) );

    QVERIFY(  math::isEqual( 2.f, 2.f - math::epsilon< float >() * 0.9f ) );
    QVERIFY(  math::isEqual( 2.f, 2.f + math::epsilon< float >() * 0.9f ) );

    QVERIFY( !math::isEqual( math::Vector2f( 3, 3 ), math::Vector2f( -3, -3 ) ) );
    QVERIFY( !math::isEqual( math::Vector2f( 3, 3 ), math::Vector2f(  3,  2 ) ) );
    QVERIFY(  math::isEqual
        ( math::Vector2f( 3, 3 )
        , math::Vector2f( 3 - math::epsilon< float >() * 0.9f, 3 + math::epsilon< float >() * 0.9f ) ) );
}


void mathTest::test_identity4f()
{
    using namespace Carna::base;

    const math::Matrix4f m( math::identity4f() );
    for( int i = 0; i < m.rows(); ++i )
    for( int j = 0; j < m.cols(); ++j )
    {
        QCOMPARE( m( i, j ), i == j ? 1.f : 0.f );
    }
}


void mathTest::test_zeros()
{
    using namespace Carna::base;

    const math::Matrix4f m( math::zeros< math::Matrix4f >() );
    for( int i = 0; i < m.rows(); ++i )
    for( int j = 0; j < m.cols(); ++j )
    {
        QCOMPARE( m( i, j ), 0.f );
    }
}


void mathTest::test_basis4f()
{
    using namespace Carna::base;
    const float pi = 4 * std::atan( 1.f );

    /* Create directional vectors:
     *  'bx' pointing to 45 deg
     *  'by' pointing to 45 + 90 deg
     *  'bz' pointing upwards or downwards
     */
    math::Vector4f bx( std::cos(     pi / 4 ), std::sin(     pi / 4 ), 0, 0 );
    math::Vector4f by( std::cos( 3 * pi / 4 ), std::sin( 3 * pi / 4 ), 0, 0 );
    math::Vector4f bz( math::vector4< float, 3 >( -math::vector3( bx ).cross( math::vector3( by ) ), 0 ) );
    math::Matrix4f m ( math::basis4f( bx, by, bz ) );

    math::Vector4f ux( 1, 0, 0, 0 );
    math::Vector4f uy( 0, 1, 0, 0 );
    math::Vector4f uz( 0, 0, 1, 0 );

    QVERIFY( math::isEqual< math::Vector4f >( m * ux, bx ) );
    QVERIFY( math::isEqual< math::Vector4f >( m * uy, by ) );
    QVERIFY( math::isEqual< math::Vector4f >( m * uz, bz ) );

    /* Add a translation to 'm'.
     * This should not affect the result.
     */
    const math::Vector4f bt( 1, 2, 3, 1 );
    m = math::basis4f( bx, by, bz, bt );

    QVERIFY( math::isEqual< math::Vector4f >( m * ux, bx ) );
    QVERIFY( math::isEqual< math::Vector4f >( m * uy, by ) );
    QVERIFY( math::isEqual< math::Vector4f >( m * uz, bz ) );

    /* Now repeat with positional vectors.
     */
    bx.w() = by.w() = bz.w() = 1;
    ux.w() = uy.w() = uz.w() = 1;

    QVERIFY( math::isEqual< math::Vector3f >( math::vector3< float, 4 >( m * ux ), math::vector3< float, 4 >( bx + bt ) ) );
    QVERIFY( math::isEqual< math::Vector3f >( math::vector3< float, 4 >( m * uy ), math::vector3< float, 4 >( by + bt ) ) );
    QVERIFY( math::isEqual< math::Vector3f >( math::vector3< float, 4 >( m * uz ), math::vector3< float, 4 >( bz + bt ) ) );
}


void mathTest::test_translation4f()
{
    using namespace Carna::base;
    const math::Matrix4f m( math::translation4f( -1, 2, -3 ) );

    QVERIFY( math::isEqual< math::Vector4f >( m * math::Vector4f( 0, 0, 0, 1 ), math::Vector4f( -1,  2, -3, 1 ) ) );
    QVERIFY( math::isEqual< math::Vector4f >( m * math::Vector4f( 1, 1, 1, 0 ), math::Vector4f(  1,  1,  1, 0 ) ) );
}


void mathTest::test_scaling4f()
{
    using namespace Carna::base;

    QVERIFY( math::isEqual< math::Vector4f >
        ( math::scaling4f( 0 ) * math::Vector4f( 1, 2, 3, 1 )
        , math::Vector4f( 0, 0, 0, 1 ) ) );

    QVERIFY( math::isEqual< math::Vector4f >
        ( math::scaling4f( 1, -2, 3 ) * math::Vector4f( -1, -1, -1, 1 )
        , math::Vector4f( -1, 2, -3, 1 ) ) );
}


void mathTest::test_orthogonal3f()
{
    using namespace Carna::base;
    const auto dotOrtho = []( const math::Vector3f& v )->float
    {
        using namespace Carna::base;
        const math::Vector3f u = math::orthogonal3f( v );
        return u.dot( v );
    };

    QCOMPARE( dotOrtho( math::Vector3f( 1, 0, 0 ) ), 0.f );
    QCOMPARE( dotOrtho( math::Vector3f( 1, 1, 0 ) ), 0.f );
    QCOMPARE( dotOrtho( math::Vector3f( 0, 1, 0 ) ), 0.f );
    QCOMPARE( dotOrtho( math::Vector3f( 0, 1, 1 ) ), 0.f );
    QCOMPARE( dotOrtho( math::Vector3f( 0, 0, 1 ) ), 0.f );
    QCOMPARE( dotOrtho( math::Vector3f( 1, 0, 1 ) ), 0.f );
    QCOMPARE( dotOrtho( math::Vector3f( 1, 1, 1 ) ), 0.f );

    QCOMPARE( dotOrtho( math::Vector3f( -1,  0,  0 ) ), 0.f );
    QCOMPARE( dotOrtho( math::Vector3f( -1,  1,  0 ) ), 0.f );
    QCOMPARE( dotOrtho( math::Vector3f(  0, -1,  0 ) ), 0.f );
    QCOMPARE( dotOrtho( math::Vector3f(  0, -1, -1 ) ), 0.f );
    QCOMPARE( dotOrtho( math::Vector3f(  0,  0, -1 ) ), 0.f );
    QCOMPARE( dotOrtho( math::Vector3f( -1,  0, -1 ) ), 0.f );
    QCOMPARE( dotOrtho( math::Vector3f( -1, -1, -1 ) ), 0.f );
}


void mathTest::test_plane4f()
{
    QWARN( "not implemented yet" );
}


void mathTest::test_round_ui()
{
    QWARN( "not implemented yet" );
}


void mathTest::test_makeEven()
{
    QWARN( "not implemented yet" );
}


void mathTest::test_mix()
{
    QWARN( "not implemented yet" );
}


void mathTest::test_LIBCARNA_FOR_VECTOR3UI()
{
    //! [example_LIBCARNA_FOR_VECTOR3UI]
    using namespace Carna::base;
    HUVolumeUInt16 data( math::Vector3ui( 100, 100, 30 ) );
  
    /* Initialize all 'data' voxels with '-1024'.
     */
    const HUV expected = HUV::abs( -1024 );
    LIBCARNA_FOR_VECTOR3UI( p, data.size )
    {
        data.setVoxel( p, expected );
    }
  
    /* Verify the result.
     */
    for( std::size_t i = 0; i < data.buffer().size(); ++i )
    {
        QCOMPARE( data.buffer()[ i ], HUVolumeUInt16::HUVToBufferValue( expected ) );
    }
    //! [example_LIBCARNA_FOR_VECTOR3UI]
};



}  // namespace Carna :: testing

}  // namespace Carna
