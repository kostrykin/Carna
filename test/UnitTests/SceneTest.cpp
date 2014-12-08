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

#include "SceneTest.h"
#include <Carna/base/model/Scene.h>
#include <Carna/base/Vector3.h>
#include <Carna/base/model/Object3D.h>
#include <Carna/base/Association.h>
#include <MockSceneFactory.h>



// ----------------------------------------------------------------------------------
// NullAggregation
// ----------------------------------------------------------------------------------

template< typename AssociatedObjectType >
class NullAggregation : public Carna::base::Association< AssociatedObjectType >
{

public:

    explicit NullAggregation()
        : Association< AssociatedObjectType >( nullptr )
    {
    }

}; // NullAggregation



// ----------------------------------------------------------------------------------
// MockObject3D
// ----------------------------------------------------------------------------------

class MockObject3D : public Carna::base::model::Object3D
{

public:

    explicit MockObject3D( Carna::base::model::Scene& );


    virtual void paint( const Carna::base::view::Renderer& ) const override;

    virtual void paintFalseColor( const Carna::base::view::Renderer&, const Carna::base::Vector3ui& color ) const override;

}; // MockObject3D


MockObject3D::MockObject3D( Carna::base::model::Scene& scene )
    : Carna::base::model::Object3D( scene )
{
}


void MockObject3D::paint( const Carna::base::view::Renderer& ) const
{
}


void MockObject3D::paintFalseColor( const Carna::base::view::Renderer&, const Carna::base::Vector3ui& color ) const
{
}



// ----------------------------------------------------------------------------------
// SceneTest
// ----------------------------------------------------------------------------------

void SceneTest::initTestCase()
{
    disableQDebug();
}


void SceneTest::cleanupTestCase()
{
}


void SceneTest::init()
{
}


void SceneTest::cleanup()
{
}


void SceneTest::instantiation()
{
    QFETCH( float, spacingX );
    QFETCH( float, spacingY );
    QFETCH( float, spacingZ );

    QFETCH( unsigned int, volumeSizeX );
    QFETCH( unsigned int, volumeSizeY );
    QFETCH( unsigned int, volumeSizeZ );

    const bool preconditions_violated =
        spacingX <= 0 ||
        spacingY <= 0 ||
        spacingZ <= 0 ||
        volumeSizeX < 2 ||
        volumeSizeY < 2 ||
        volumeSizeZ < 2;

    const Carna::base::Vector3ui size( volumeSizeX, volumeSizeY, volumeSizeZ );
    const MockSceneFactory sceneFactory( size, spacingX, spacingY, spacingZ );

    if( preconditions_violated )
    {
        EXPECT_ASSERTION_FAILURE( std::unique_ptr< Carna::base::model::Scene >( sceneFactory.createMockScene() ) );
    }
    else
    {
        const std::unique_ptr< Carna::base::model::Scene > scene( sceneFactory.createMockScene() );

        QCOMPARE( scene->spacingX(), spacingX );
        QCOMPARE( scene->spacingY(), spacingY );
        QCOMPARE( scene->spacingZ(), spacingZ );

        QCOMPARE( scene->volume().size.x, volumeSizeX );
        QCOMPARE( scene->volume().size.y, volumeSizeY );
        QCOMPARE( scene->volume().size.z, volumeSizeZ );
    }
}


void SceneTest::instantiation_data()
{
    QTest::addColumn< float >( "spacingX" );
    QTest::addColumn< float >( "spacingY" );
    QTest::addColumn< float >( "spacingZ" );

    QTest::addColumn< unsigned int >( "volumeSizeX" );
    QTest::addColumn< unsigned int >( "volumeSizeY" );
    QTest::addColumn< unsigned int >( "volumeSizeZ" );

    const static float EPSILON = Carna::base::Math::epsilon< float >();

 // ----------------------------------------------------------------------------------
 // Positive Tests
 // ----------------------------------------------------------------------------------

    QTest::newRow( "Positive Test" ) << 0.1f << 0.2f << 0.3f << 30u << 20u << 10u;

 // extreme values

    QTest::newRow( "Extreme: volumeSizeX = 2" ) << 1.f << 1.f << 1.f <<  2u << 10u << 10u;
    QTest::newRow( "Extreme: volumeSizeY = 2" ) << 1.f << 1.f << 1.f << 10u <<  2u << 10u;
    QTest::newRow( "Extreme: volumeSizeZ = 2" ) << 1.f << 1.f << 1.f << 10u << 10u <<  2u;

    QTest::newRow( "Extreme: spacingX = EPSILON" ) << EPSILON << 1.f << 1.f << 10u << 10u << 10u;
    QTest::newRow( "Extreme: spacingY = EPSILON" ) << 1.f << EPSILON << 1.f << 10u << 10u << 10u;
    QTest::newRow( "Extreme: spacingZ = EPSILON" ) << 1.f << 1.f << EPSILON << 10u << 10u << 10u;

 // ----------------------------------------------------------------------------------
 // Defect Tests
 // ----------------------------------------------------------------------------------

 // too small volume sizes

    QTest::newRow( "Defect: volumeSizeX = 0" ) << 1.f << 1.f << 1.f <<  0u << 10u << 10u;
    QTest::newRow( "Defect: volumeSizeY = 0" ) << 1.f << 1.f << 1.f << 10u <<  0u << 10u;
    QTest::newRow( "Defect: volumeSizeZ = 0" ) << 1.f << 1.f << 1.f << 10u << 10u <<  0u;

 // zero spacings

    QTest::newRow( "Defect: spacingX = 0" ) << 0.f << 1.f << 1.f << 10u << 10u << 10u;
    QTest::newRow( "Defect: spacingY = 0" ) << 1.f << 0.f << 1.f << 10u << 10u << 10u;
    QTest::newRow( "Defect: spacingZ = 0" ) << 1.f << 1.f << 0.f << 10u << 10u << 10u;

 // negative spacings

    QTest::newRow( "Defect: spacingX < 0" ) << -1.f <<  1.f <<  1.f << 10u << 10u << 10u;
    QTest::newRow( "Defect: spacingY < 0" ) <<  1.f << -1.f <<  1.f << 10u << 10u << 10u;
    QTest::newRow( "Defect: spacingZ < 0" ) <<  1.f <<  1.f << -1.f << 10u << 10u << 10u;
}


void SceneTest::instantiation_bug26()
{
    EXPECT_ASSERTION_FAILURE
    (
        Carna::base::model::Scene
            ( new Carna::base::Composition< Carna::base::model::Volume >( new Carna::base::model::UInt16Volume( Carna::base::Vector3ui
                ( 5
                , 8
                , 8
                ) ) )
            , 1.f, 1.f, 1.f );
    );
}


void SceneTest::instantiation_null_volume()
{
    EXPECT_ASSERTION_FAILURE( Carna::base::model::Scene( nullptr, 1.f, 1.f, 1.f ) );
    EXPECT_ASSERTION_FAILURE( Carna::base::model::Scene( new NullAggregation< Carna::base::model::Volume >(), 1.f, 1.f, 1.f ) );
}


Carna::base::model::Scene* SceneTest::createScene()
{
    const Carna::base::Vector3ui size( 30, 20, 10 );
    return MockSceneFactory( size, 0.1f, 0.2f, 0.3f ).createMockScene();
}


void SceneTest::createHuvTestData()
{
    QTest::addColumn< int >( "huv" );

 // ----------------------------------------------------------------------------------
 // Positive Tests
 // ----------------------------------------------------------------------------------

    QTest::newRow( "huv = -321" )   << -321;
    QTest::newRow( "huv = 1234" )   << 1234;
    QTest::newRow( "huv = 0" )      <<    0;

 // extreme values

    QTest::newRow( "huv = -1024" )  << -1024;
    QTest::newRow( "huv = 3071" )   <<  3071;

 // ----------------------------------------------------------------------------------
 // Defect Tests
 // ----------------------------------------------------------------------------------
    
    QTest::newRow( "huv = -2000" )  << 2000;
    QTest::newRow( "huv = 4000" )   << 4000;

 // extreme values

    QTest::newRow( "huv = -1025" )  << -1025;
    QTest::newRow( "huv = 3071" )   <<  3071;
}


void SceneTest::runHuvPropertyTest( const HuvSetter& set, const HuvGetter& get, const char* signal )
{
    const std::unique_ptr< Carna::base::model::Scene > scene( createScene() );

    QFETCH( int, huv );

    if( huv >= -1024 && huv <= 3071 )
    {
        const int original_huv = get( *scene );
        const std::unique_ptr< QSignalSpy > spy( signal != nullptr
            ? new QSignalSpy( scene.get(), signal )
            : nullptr );

        set( *scene, huv );
        QCOMPARE( get( *scene ), huv );

        if( spy.get() )
        {
            QCOMPARE( spy->count(), original_huv != huv ? 1 : 0 );
        }
    }
    else
    {
        set( *scene, 123 );
        EXPECT_ASSERTION_FAILURE( set( *scene, huv ) );
        QCOMPARE( get( *scene ), 123 );
    }
}


void SceneTest::recommendedWindowingLevel()
{
    runHuvPropertyTest(
        []( Carna::base::model::Scene& scene, int huv )
            {
                scene.setRecommendedWindowingLevel( huv );
            },
        []( const Carna::base::model::Scene& scene )->int
            {
                return scene.recommendedWindowingLevel();
            },
        SIGNAL( recommendedWindowingChanged() )
    );
}


void SceneTest::recommendedWindowingLevel_data()
{
    createHuvTestData();
}


void SceneTest::recommendedWindowingWidth()
{
    runHuvPropertyTest(
        []( Carna::base::model::Scene& scene, int huv )
            {
                scene.setRecommendedWindowingWidth( huv );
            },
        []( const Carna::base::model::Scene& scene )->int
            {
                return scene.recommendedWindowingWidth();
            },
        SIGNAL( recommendedWindowingChanged() )
    );
}


void SceneTest::recommendedWindowingWidth_data()
{
    createHuvTestData();
}


void SceneTest::recommendedVoidThreshold()
{
    runHuvPropertyTest(
        []( Carna::base::model::Scene& scene, int huv )
            {
                scene.setRecommendedVoidThreshold( huv );
            },
        []( const Carna::base::model::Scene& scene )->int
            {
                return scene.recommendedVoidThreshold();
            },
        SIGNAL( recommendedVoidThresholdChanged() )
    );
}


void SceneTest::recommendedVoidThreshold_data()
{
    createHuvTestData();
}


void SceneTest::objects3D()
{
    const std::unique_ptr< Carna::base::model::Scene > scene( createScene() );

    const unsigned int max_count = 10;

    QSignalSpy spy( scene.get(), SIGNAL( objectsChanged( const Carna::base::model::Object3DEvent& ) ) );

    QCOMPARE( scene->objectsCount(), 0u );

 // create objects

    std::vector< MockObject3D* > objects( max_count );
    for( unsigned int i = 0; i < max_count; ++i )
    {
        objects[ i ] = new MockObject3D( *scene );

        QCOMPARE( scene->objectsCount(), i + 1 );
    }

    QApplication::processEvents();
    QCOMPARE( spy.count(), 1 );

 // test object references

    for( unsigned int i = 0; i < max_count; ++i )
    {
        QCOMPARE( &scene->objectByIndex( i ), objects[ i ] );
        QCOMPARE( &static_cast< const Carna::base::model::Scene& >( *scene ).objectByIndex( i ), objects[ i ] );
    }

 // remove objects

    spy.clear();

    for( unsigned int i = 0; i < max_count; ++i )
    {
        delete objects[ i ];
        objects[ i ] = nullptr;

        QCOMPARE( scene->objectsCount(), max_count - ( i + 1 ) );
    }

    QApplication::processEvents();
    QCOMPARE( spy.count(), 1 );
}
