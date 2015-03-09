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

#ifndef HUVOLUMEGRIDHELPER_H_6014714286
#define HUVOLUMEGRIDHELPER_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/math.h>
#include <Carna/base/HUVolumeGrid.h>
#include <Carna/base/HUVolumeSegment.h>
#include <Carna/base/BufferedHUVolumeTexture.h>
#include <Carna/base/Geometry.h>

/** \file   HUVolumeGridHelper.h
  * \brief  Defines \ref Carna::helpers::HUVolumeGridHelper.
  */

namespace Carna
{

namespace helpers
{



// ----------------------------------------------------------------------------------
// HUVolumeGridHelper
// ----------------------------------------------------------------------------------

template< typename HUVolumeSegmentVolume >
class HUVolumeGridHelper
{

    NON_COPYABLE

    std::unique_ptr< base::HUVolumeGrid > myGrid;

    base::Spatial* createNode( const base::GLContext& glc, unsigned int geometryType, unsigned int volumeTextureRole ) const;

public:

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    const static std::size_t DEFAULT_MAX_SEGMENT_BYTESIZE = 2 * 300 * 300 * 300;

    const static unsigned int DEFAULT_VOLUME_TEXTURE_ROLE = 0;

    HUVolumeGridHelper( const base::math::Vector3ui& resolution, std::size_t maxSegmentBytesize = DEFAULT_MAX_SEGMENT_BYTESIZE );

    const base::math::Vector3ui resolution;

    const std::size_t maxSegmentBytesize;

    const base::math::Vector3ui maxSegmentSize;

    void loadData( const std::function< base::HUV( const base::math::Vector3ui& ) >& );

    base::HUVolumeGrid& grid() const;

    struct Spacing
    {
        explicit Spacing( const base::math::Vector3f& millimters );

        const base::math::Vector3f& millimters;
    };

    struct Dimensions
    {
        explicit Dimensions( const base::math::Vector3f& millimters );

        const base::math::Vector3f& millimters;
    };

    base::Node* createNode
        ( const base::GLContext& glc
        , unsigned int geometryType
        , const Spacing& spacing
        , unsigned int volumeTextureRole = DEFAULT_VOLUME_TEXTURE_ROLE ) const;

    base::Node* createNode
        ( const base::GLContext& glc
        , unsigned int geometryType
        , const Dimensions& dimensions
        , unsigned int volumeTextureRole = DEFAULT_VOLUME_TEXTURE_ROLE ) const;

}; // HUVolumeGridHelper


template< typename HUVolumeSegmentVolume >
HUVolumeGridHelper< HUVolumeSegmentVolume >::HUVolumeGridHelper( const base::math::Vector3ui& resolution, std::size_t maxSegmentBytesize )
    : resolution( resolution )
    , maxSegmentBytesize( maxSegmentBytesize )
    , maxSegmentSize( [&resolution, maxSegmentBytesize]()->base::math::Vector3ui
            {
                const float maxSideLength = std::pow< float >( maxSegmentBytesize / 2.f, 1.3f / 3 );
                return base::math::round_ui( maxSideLength );
            }()
        )
{
    const base::math::Vector3ui tails
        ( resolution.x() % maxSegmentSize.x()
        , resolution.y() % maxSegmentSize.y()
        , resolution.z() % maxSegmentSize.z() );

    const base::math::Vector3ui segmentCounts
        ( resolution.x() / maxSegmentSize.x() + ( tails.x() > 0 ? 1 : 0 )
        , resolution.y() / maxSegmentSize.y() + ( tails.y() > 0 ? 1 : 0 )
        , resolution.z() / maxSegmentSize.z() + ( tails.z() > 0 ? 1 : 0 ) );
    myGrid.reset( new HUVolumeGrid< HUVolumeSegmentVolume >( maxSegmentSize, segmentCounts ) )

    base::math::Vector3ui segmentCoord;
    for( segmentCoord.z() = 0; segmentCoord.z() < myGrid->segmentCounts.z(); ++segmentCoord.z() )
    for( segmentCoord.y() = 0; segmentCoord.y() < myGrid->segmentCounts.y(); ++segmentCoord.y() )
    for( segmentCoord.x() = 0; segmentCoord.x() < myGrid->segmentCounts.x(); ++segmentCoord.x() )
    {
        const base::math::Vector3ui volumeSize
            ( segmentCoord.x() + 1 == myGrid->segmentCounts.x() ? tails.x() : maxSegmentSize.x()
            , segmentCoord.y() + 1 == myGrid->segmentCounts.y() ? tails.y() : maxSegmentSize.y()
            , segmentCoord.z() + 1 == myGrid->segmentCounts.z() ? tails.z() : maxSegmentSize.z() );

        HUVolumeSegmentVolume* const volume = new HUVolumeSegmentVolume( volumeSize );
        myGrid->segmentAt( segmentCoord ).setVolume( volume );
    }
}


template< typename HUVolumeSegmentVolume >
void HUVolumeGridHelper< HUVolumeSegmentVolume >::loadData
    ( const std::function< base::HUV( const base::math::Vector3ui& ) >& data )
{
    base::math::Vector3ui coord;
    for( coord.z() = 0; coord.z() < resolution.z(); ++coord.z() )
    for( coord.y() = 0; coord.y() < resolution.y(); ++coord.y() )
    for( coord.x() = 0; coord.x() < resolution.x(); ++coord.x() )
    {
        const base::HUV huv = data( coord );
        myGrid->setVoxel( coord, huv );
    }
}


template< typename HUVolumeSegmentVolume >
base::HUVolumeGrid& HUVolumeGridHelper< HUVolumeSegmentVolume >::grid() const
{
    return *myGrid;
}


template< typename HUVolumeSegmentVolume >
base::Spatial* HUVolumeGridHelper< HUVolumeSegmentVolume >::createNode
    ( const base::GLContext& glc, unsigned int geometryType, unsigned int volumeTextureRole ) const
{
    base::Node* const pivot = new base::Node();
    const base::math::Matrix4f baseTranslation( base::math::translation4f
        ( -static_cast< float >( myGrid->segmentCounts.x() - 1 ) / 2
        , -static_cast< float >( myGrid->segmentCounts.y() - 1 ) / 2
        , -static_cast< float >( myGrid->segmentCounts.z() - 1 ) / 2 ) );

    glc.makeActive();
    base::math::Vector3ui segmentCoord;
    for( segmentCoord.z() = 0; segmentCoord.z() < myGrid->segmentCounts.z(); ++segmentCoord.z() )
    for( segmentCoord.y() = 0; segmentCoord.y() < myGrid->segmentCounts.y(); ++segmentCoord.y() )
    for( segmentCoord.x() = 0; segmentCoord.x() < myGrid->segmentCounts.x(); ++segmentCoord.x() )
    {
        const HUVolumeGrid< HUVolumeSegmentVolume >::HUVolumeSegment& segment = myGrid->segmentAt( segmentCoord );
        typedef base::BufferedHUVolumeTexture< HUVolumeSegmentVolume > Texture;
        Texture& texture = Texture::create( segment.volume() );

        base::Geometry* const geom = new  base::Geometry( geometryType );
        geom->putFeature( volumeTextureRole, texture );
        geom->localTransform = baseTranslation * base::math::translation4f( segmentCoord );
        pivot->attachChild( geom );
    }

    return pivot;
}


template< typename HUVolumeSegmentVolume >
base::Node* HUVolumeGridHelper< HUVolumeSegmentVolume >::createNode
    ( const base::GLContext& glc, unsigned int geometryType, const Spacing& spacing, unsigned int volumeTextureRole ) const
{
    return createNode( glc, geometryType, Dimensions( ( resolution - 1 ) * spacing.millimeters, volumeTextureRole );
}


template< typename HUVolumeSegmentVolume >
base::Node* HUVolumeGridHelper< HUVolumeSegmentVolume >::createNode
    ( const base::GLContext& glc, unsigned int geometryType, const Dimensions& dimensions, unsigned int volumeTextureRole ) const
{
    base::Node* const gridNode = createNode( glc, geometryType, volumeTextureRole );
    base::Node* const pivot = new base::Node();
    pivot->localTransform = base::math::scaling4f( dimensions );
    pivot->attachChild( gridNode );
    return pivot;
}


template< typename HUVolumeSegmentVolume >
HUVolumeGridHelper< HUVolumeSegmentVolume >::Spacing::Spacing( const base::math::Vector3f& millimters )
    : millimters( millimters )
{
}


template< typename HUVolumeSegmentVolume >
HUVolumeGridHelper< HUVolumeSegmentVolume >::Dimensions::Dimensions( const base::math::Vector3f& millimters )
    : millimters( millimters )
{
}



}  // namespace Carna :: helpers

}  // namespace Carna

#endif // HUVOLUMEGRIDHELPER_H_6014714286
