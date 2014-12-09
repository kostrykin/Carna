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

#include <Carna/base/model/SceneDetails.h>
#include <Carna/base/model/Object3D.h>
#include <Carna/base/model/Object3DEvent.h>
#include <Carna/base/model/Volume.h>
#include <Carna/base/CarnaException.h>
#include <Carna/base/Vector3.h>
#include <QTimer>
#include <QDebug>
#include <mmintrin.h>

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// Scene :: Details
// ----------------------------------------------------------------------------------

const base::Flags< Object3DEvent::Feature > Scene::Details::nullScheduledObject3DEvents
    = base::Flags< Object3DEvent::Feature >();


Scene::Details::Details( Scene& self, Association< Volume >* volume, float spacingX, float spacingY, float spacingZ )
    : self( self )
    , volume( volume )
    , spacingX( spacingX )
    , spacingY( spacingY )
    , spacingZ( spacingZ )
    , windowingLevel( 4096 / 2 - 1024 )
    , windowingWidth( 4096 / 2 )
    , recommendedVoidThreshold( 0 )
{
}


void Scene::Details::scheduleObject3DEvent( const Object3DEvent& event )
{
    if( scheduledObject3DEvents == nullScheduledObject3DEvents )
    {
        QTimer::singleShot( 0, this, SLOT( postScheduledObject3DEvents() ) );
    }

    scheduledObject3DEvents |= event.flags();
}


void Scene::Details::postScheduledObject3DEvents()
{
    emit self.objectsChanged( Object3DEvent( scheduledObject3DEvents ) );
    scheduledObject3DEvents = nullScheduledObject3DEvents;
}



// ----------------------------------------------------------------------------------
// Scene
// ----------------------------------------------------------------------------------

Scene::Scene( Association< Volume >* volume
            , float spacingX
            , float spacingY
            , float spacingZ )
    : pimpl( new Details( *this, volume, spacingX, spacingY, spacingZ ) )
{
    CARNA_ASSERT( volume != nullptr && volume->get() != nullptr );

    CARNA_ASSERT( ( *volume )->size.x >= 2 );
    CARNA_ASSERT( ( *volume )->size.y >= 2 );
    CARNA_ASSERT( ( *volume )->size.z >= 2 );

    CARNA_ASSERT( spacingX > 0 );
    CARNA_ASSERT( spacingY > 0 );
    CARNA_ASSERT( spacingZ > 0 );

 // early warning for bug #26:

    CARNA_ASSERT_EX( ( *volume )->size.x % 2 == 0, "The volume's width must be even. " \
        "This is a known bug that will be solved as soon as the volume concept is refactored." );

 // registers the meta type for the 'Object3DEvent' class (required by the 'objectsChanged' signal)

    qRegisterMetaType< Carna::base::model::Object3DEvent >( "const Carna::base::model::Object3DEvent" );
}


Scene::~Scene()
{
    while( !pimpl->objects.empty() )
    {
        delete pimpl->objects.front();
    }
}


void Scene::setRecommendedVoidThreshold( signed int huv )
{
    CARNA_ASSERT( huv >= -1024 );
    CARNA_ASSERT( huv <=  3071 );

    if( huv != pimpl->recommendedVoidThreshold )
    {
        pimpl->recommendedVoidThreshold = huv;

        emit recommendedVoidThresholdChanged();
    }
}


void Scene::setRecommendedWindowingLevel( signed int level )
{
    CARNA_ASSERT( level >= -1024 );
    CARNA_ASSERT( level <=  3071 );

    if( level != pimpl->windowingLevel )
    {
        pimpl->windowingLevel = level;

        emit recommendedWindowingChanged();
    }
}


void Scene::setRecommendedWindowingWidth( signed int width )
{
    CARNA_ASSERT( width >= -1024 );
    CARNA_ASSERT( width <=  3071 );

    if( width != pimpl->windowingWidth )
    {
        pimpl->windowingWidth = width;

        emit recommendedWindowingChanged();
    }
}


Object3D& Scene::objectByIndex( unsigned int index )
{
    CARNA_ASSERT( index < objectsCount() );

    return static_cast< Object3D& >( *pimpl->objects[ index ] );
}


const Volume& Scene::volume() const
{
    return *pimpl->volume->get();
}


signed int Scene::recommendedVoidThreshold() const
{
    CARNA_ASSERT( pimpl->recommendedVoidThreshold >= -1024 );
    CARNA_ASSERT( pimpl->recommendedVoidThreshold <=  3071 );

    return pimpl->recommendedVoidThreshold;
}


float Scene::spacingX() const
{
    CARNA_ASSERT( pimpl->spacingX > 0 );

    return pimpl->spacingX;
}


float Scene::spacingY() const
{
    CARNA_ASSERT( pimpl->spacingY > 0 );

    return pimpl->spacingY;
}


float Scene::spacingZ() const
{
    CARNA_ASSERT( pimpl->spacingZ > 0 );

    return pimpl->spacingZ;
}


signed int Scene::recommendedWindowingLevel() const
{
    CARNA_ASSERT( pimpl->windowingLevel >= -1024 );
    CARNA_ASSERT( pimpl->windowingLevel <=  3071 );

    return pimpl->windowingLevel;
}


signed int Scene::recommendedWindowingWidth() const
{
    CARNA_ASSERT( pimpl->windowingWidth >= -1024 );
    CARNA_ASSERT( pimpl->windowingWidth <=  3071 );

    return pimpl->windowingWidth;
}


unsigned int Scene::objectsCount() const
{
    return pimpl->objects.size();
}


void Scene::setVolumeMask( BufferedMaskAdapter* mask )
{
    pimpl->mask.reset( mask );
    emit volumeMaskExchanged();
}


bool Scene::hasVolumeMask() const
{
    return pimpl->mask.get() != nullptr;
}


const BufferedMaskAdapter& Scene::volumeMask() const
{
    CARNA_ASSERT( hasVolumeMask() );

    return *pimpl->mask;
}


double Scene::intensityAt( const Position& position ) const
{
    const double x = position.toVolumeUnits().x() * ( volume().size.x - 1 );
    const double y = position.toVolumeUnits().y() * ( volume().size.y - 1 );
    const double z = position.toVolumeUnits().z() * ( volume().size.z - 1 );

    const auto sampleAt = [&]( int x, int y, int z )->signed short
    {
        if( x < 0 || y < 0 || z < 0 ||
            x >= static_cast< signed >( volume().size.x ) ||
            y >= static_cast< signed >( volume().size.y ) ||
            z >= static_cast< signed >( volume().size.z ) )
        {
            return -1024;
        }
        else
        {
            return volume()( static_cast< unsigned int >( x )
                           , static_cast< unsigned int >( y )
                           , static_cast< unsigned int >( z ) );
        }
    };

    const auto signum = []( int x )->int
    {
        return 1 + ( ( x < 0 ) * -2 );
    };

    const int x0 = static_cast< int >( x );
    const int y0 = static_cast< int >( y );
    const int z0 = static_cast< int >( z );

    const int x1 = ( std::abs( x0 ) + 1 ) * signum( x0 );
    const int y1 = ( std::abs( y0 ) + 1 ) * signum( y0 );
    const int z1 = ( std::abs( z0 ) + 1 ) * signum( z0 );

    const float dx = static_cast< float >( std::abs( x - x0 ) );
    const float dy = static_cast< float >( std::abs( y - y0 ) );
    const float dz = static_cast< float >( std::abs( z - z0 ) );

 // sample lattice points (sorted primarily by x and secondarily by y for less paging effort)

    __declspec( align( 16 ) ) float i0_buffer[ 4 ];
    __declspec( align( 16 ) ) float i1_buffer[ 4 ];

    i0_buffer[ 0 ] = static_cast< float >( sampleAt( x0, y0, z0 ) );    // 0, 0, 0
    i1_buffer[ 0 ] = static_cast< float >( sampleAt( x1, y0, z0 ) );    // 1, 0, 0

    i0_buffer[ 2 ] = static_cast< float >( sampleAt( x0, y1, z0 ) );    // 0, 1, 0
    i1_buffer[ 2 ] = static_cast< float >( sampleAt( x1, y1, z0 ) );    // 1, 0, 1

    i0_buffer[ 1 ] = static_cast< float >( sampleAt( x0, y0, z1 ) );    // 0, 0, 1
    i1_buffer[ 1 ] = static_cast< float >( sampleAt( x1, y0, z1 ) );    // 1, 0, 1

    i0_buffer[ 3 ] = static_cast< float >( sampleAt( x0, y1, z1 ) );    // 0, 1, 1
    i1_buffer[ 3 ] = static_cast< float >( sampleAt( x1, y1, z1 ) );    // 1, 1, 1

 // interpolate along x

    const float inverse_dx_buffer = 1 - dx;
    __m128 sse_inverse_dx = _mm_load1_ps( &inverse_dx_buffer );
    __m128 sse_dx = _mm_load1_ps( &dx );

    __m128 sse_i0 = _mm_load_ps( i0_buffer );
    __m128 sse_i1 = _mm_load_ps( i1_buffer );

    __m128 sse_ix_a = _mm_mul_ps( sse_inverse_dx, sse_i0 );
    __m128 sse_ix_b = _mm_mul_ps( sse_dx, sse_i1 );
    __m128 sse_ix   = _mm_add_ps( sse_ix_a, sse_ix_b );

 // interpolate along y

    __declspec( align( 16 ) ) float ix00_weight_buffer[ 4 ];
    ix00_weight_buffer[ 0 ] = ix00_weight_buffer[ 1 ] = 1 - dy;
    ix00_weight_buffer[ 2 ] = ix00_weight_buffer[ 3 ] = dy;

    __m128 sse_ix00_weight = _mm_load_ps( ix00_weight_buffer );
    __m128 sse_ixy = _mm_mul_ps( sse_ix, sse_ix00_weight );

    __declspec( align( 16 ) ) float ixy_buffer[ 4 ];
    _mm_store_ps( ixy_buffer, sse_ixy );

    const float ixy0 = ixy_buffer[ 0 ] + ixy_buffer[ 2 ];
    const float ixy1 = ixy_buffer[ 1 ] + ixy_buffer[ 3 ];

 // interpolate along z

    const float ixyz = ( 1 - dz ) * ixy0 + dz * ixy1;

 // finish

    return ixyz;
}



// ----------------------------------------------------------------------------------
// Scene::BaseObject3D
// ----------------------------------------------------------------------------------

Scene::BaseObject3D::BaseObject3D( Scene& model )
    : model( model )
{
    model.pimpl->objects.push_back( this );

    model.pimpl->scheduleObject3DEvent( Object3DEvent( Object3DEvent::existence ) );
}


Scene::BaseObject3D::~BaseObject3D()
{
    auto it = std::find( model.pimpl->objects.begin()
                       , model.pimpl->objects.end()
                       , this );

    CARNA_ASSERT_EX( it != model.pimpl->objects.end(), "unknown 3D object" );

    model.pimpl->objects.erase( it );

    model.pimpl->scheduleObject3DEvent( Object3DEvent( Object3DEvent::existence ) );
}


void Scene::BaseObject3D::invalidateObjects3D( Object3DEvent& ev )
{
    model.pimpl->scheduleObject3DEvent( ev );
}


void Scene::removeVolumeMask()
{
    setVolumeMask( nullptr );
}



}  // namespace Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna
