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
#include <Carna/base/Log.h>
#include <map>
#include <memory>
#include <cmath>

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

    const base::math::Vector3ui originalResolution;

    std::unique_ptr< base::HUVolumeGrid< HUVolumeSegmentVolume > > myGrid;

    mutable std::map< const typename base::HUVolumeGrid< HUVolumeSegmentVolume >::HUVolumeSegment*, base::Texture3D* > textures;

    base::Spatial* createSpatial( const base::GLContext& glc, unsigned int geometryType, unsigned int volumeTextureRole ) const;

    static base::math::Vector3ui computeMaxSegmentSize( const base::math::Vector3ui& resolution, std::size_t maxSegmentBytesize );

public:

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    const static std::size_t DEFAULT_MAX_SEGMENT_BYTESIZE = 2 * 300 * 300 * 300;

    const static unsigned int DEFAULT_VOLUME_TEXTURE_ROLE = 0;

    HUVolumeGridHelper( const base::math::Vector3ui& resolution, std::size_t maxSegmentBytesize = DEFAULT_MAX_SEGMENT_BYTESIZE );

    /** \brief
      * Checks whether \ref invalidateTextures has been invoked before, if required.
      * An error is logged occasionally.
      */
    ~HUVolumeGridHelper();

    const base::math::Vector3ui resolution;

    const std::size_t maxSegmentBytesize;

    const base::math::Vector3ui maxSegmentSize;

    /** \brief
      * Alters the volume data. You must also call \ref invalidateTextures if
      * \ref createNode was invoked previously, in order for succeeding invocations
      * to \ref createNode to reflect the changes made to the volume data.
      */
    template< typename UnaryVector3uiToHUVFunction >
    void loadData( const UnaryVector3uiToHUVFunction& );

    /** \brief
      * Releases all previously acquired textures.
      *
      * \warning
      * Always invoke this method at least once before deleting
      * \c %HUVolumeGridHelper instances, if \ref createNode might have been earlier!
      *
      * You might also want to invoke this method when the volume data changes, e.g.
      * through \ref loadData. This will allow video resources, that were occupied by
      * earlier calls to \ref createNode, to be freed sooner.
      *
      * \attention
      * Note however, that if you call this method between two invocations of
      * \ref createNode, without the volume data being altered, same textures will
      * get uploaded twice to video memory, i.e. video resources will be wasted.
      */
    void invalidateTextures( const base::GLContext& glc );

    base::HUVolumeGrid< HUVolumeSegmentVolume >& grid() const;

    struct Spacing
    {
        explicit Spacing( const base::math::Vector3f& millimeters );

        base::math::Vector3f millimeters;
    };

    struct Dimensions
    {
        explicit Dimensions( const base::math::Vector3f& millimeters );

        base::math::Vector3f millimeters;
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
base::math::Vector3ui HUVolumeGridHelper< HUVolumeSegmentVolume >::computeMaxSegmentSize
    ( const base::math::Vector3ui& resolution, std::size_t maxSegmentBytesize )
{
    const float maxSideLengthF = std::pow
        ( maxSegmentBytesize / static_cast< float >( sizeof( HUVolumeSegmentVolume::VoxelType ) ), 1.f / 3 );
    const unsigned int maxSideLength = base::math::makeEven( base::math::round_ui( maxSideLengthF ), -1 );
    return base::math::Vector3ui( maxSideLength, maxSideLength, maxSideLength );
}


template< typename HUVolumeSegmentVolume >
HUVolumeGridHelper< HUVolumeSegmentVolume >::HUVolumeGridHelper
        ( const base::math::Vector3ui& originalResolution
        , std::size_t maxSegmentBytesize )
    : originalResolution( originalResolution )
    , resolution( base::math::makeEven( originalResolution, +1 ) )
    , maxSegmentBytesize( maxSegmentBytesize )
    , maxSegmentSize( computeMaxSegmentSize( resolution, maxSegmentBytesize ) )
{
    const base::math::Vector3ui tails
        ( resolution.x() % maxSegmentSize.x()
        , resolution.y() % maxSegmentSize.y()
        , resolution.z() % maxSegmentSize.z() );

    const base::math::Vector3ui segmentCounts
        ( resolution.x() / maxSegmentSize.x() + ( tails.x() > 0 ? 1 : 0 )
        , resolution.y() / maxSegmentSize.y() + ( tails.y() > 0 ? 1 : 0 )
        , resolution.z() / maxSegmentSize.z() + ( tails.z() > 0 ? 1 : 0 ) );
    myGrid.reset( new base::HUVolumeGrid< HUVolumeSegmentVolume >( maxSegmentSize, segmentCounts ) );

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
        myGrid->segmentAt( segmentCoord.x(), segmentCoord.y(), segmentCoord.z() ).setVolume
            ( new base::Composition< HUVolumeSegmentVolume >( volume ) );
    }
}


template< typename HUVolumeSegmentVolume >
HUVolumeGridHelper< HUVolumeSegmentVolume >::~HUVolumeGridHelper()
{
    if( !textures.empty() )
    {
        base::Log::instance().record
            ( base::Log::error
            , "Leaking video memory! Invoke 'invalidateTextures' before deleting 'HUVolumeGridHelper' to solve." );
    }
}


template< typename HUVolumeSegmentVolume >
void HUVolumeGridHelper< HUVolumeSegmentVolume >::invalidateTextures( const base::GLContext& glc )
{
    glc.makeActive();
    for( auto itr = textures.begin(); itr != textures.end(); ++itr )
    {
        base::Texture3D& texture = *itr->second;
        texture.release();
    }
    textures.clear();
}


template< typename HUVolumeSegmentVolume >
template< typename UnaryVector3uiToHUVFunction >
void HUVolumeGridHelper< HUVolumeSegmentVolume >::loadData
    ( const UnaryVector3uiToHUVFunction& data )
{
    base::math::Vector3ui coord;
    for( coord.z() = 0; coord.z() < resolution.z(); ++coord.z() )
    for( coord.y() = 0; coord.y() < resolution.y(); ++coord.y() )
    for( coord.x() = 0; coord.x() < resolution.x(); ++coord.x() )
    {
        const bool outOfOriginalBounds
            =  coord.x() >= originalResolution.x()
            || coord.y() >= originalResolution.y()
            || coord.z() >= originalResolution.z();
        const base::HUV huv = outOfOriginalBounds ? -1024 : data( coord );
        myGrid->setVoxel( coord, huv );
    }
}


template< typename HUVolumeSegmentVolume >
base::HUVolumeGrid< HUVolumeSegmentVolume >& HUVolumeGridHelper< HUVolumeSegmentVolume >::grid() const
{
    return *myGrid;
}


template< typename HUVolumeSegmentVolume >
base::Spatial* HUVolumeGridHelper< HUVolumeSegmentVolume >::createSpatial
    ( const base::GLContext& glc, unsigned int geometryType, unsigned int volumeTextureRole ) const
{
    base::Node* const pivot = new base::Node();
    const base::math::Matrix4f baseTransform
        = base::math::scaling4f
            ( 1.f / myGrid->segmentCounts.x()
            , 1.f / myGrid->segmentCounts.y()
            , 1.f / myGrid->segmentCounts.z() )
        * base::math::translation4f
            ( -static_cast< float >( myGrid->segmentCounts.x() - 1 ) / 2
            , -static_cast< float >( myGrid->segmentCounts.y() - 1 ) / 2
            , -static_cast< float >( myGrid->segmentCounts.z() - 1 ) / 2 );

    glc.makeActive();
    base::math::Vector3ui segmentCoord;
    for( segmentCoord.z() = 0; segmentCoord.z() < myGrid->segmentCounts.z(); ++segmentCoord.z() )
    for( segmentCoord.y() = 0; segmentCoord.y() < myGrid->segmentCounts.y(); ++segmentCoord.y() )
    for( segmentCoord.x() = 0; segmentCoord.x() < myGrid->segmentCounts.x(); ++segmentCoord.x() )
    {
        const base::HUVolumeGrid< HUVolumeSegmentVolume >::HUVolumeSegment& segment
            = myGrid->segmentAt( segmentCoord.x(), segmentCoord.y(), segmentCoord.z() );
        auto textureItr = textures.find( &segment );
        base::Texture3D* texture;
        if( textureItr == textures.end() )
        {
            texture = &base::BufferedHUVolumeTexture< HUVolumeSegmentVolume >::create( segment.volume() );
            textures[ &segment ] = texture;
        }
        else
        {
            texture = textureItr->second;
        }

        base::Geometry* const geom = new  base::Geometry( geometryType );
        geom->putFeature( volumeTextureRole, *texture );
        geom->localTransform = baseTransform * base::math::translation4f( segmentCoord );
        geom->setMovable( false );
        pivot->attachChild( geom );
    }

    pivot->setMovable( false );
    return pivot;
}


template< typename HUVolumeSegmentVolume >
base::Node* HUVolumeGridHelper< HUVolumeSegmentVolume >::createNode
    ( const base::GLContext& glc, unsigned int geometryType, const Spacing& spacing, unsigned int volumeTextureRole ) const
{
    const base::math::Vector3f dimensions
        ( ( resolution.x() - 1 ) * spacing.millimeters.x()
        , ( resolution.y() - 1 ) * spacing.millimeters.y()
        , ( resolution.z() - 1 ) * spacing.millimeters.z() );
    return createNode( glc, geometryType, Dimensions( dimensions ), volumeTextureRole );
}


template< typename HUVolumeSegmentVolume >
base::Node* HUVolumeGridHelper< HUVolumeSegmentVolume >::createNode
    ( const base::GLContext& glc, unsigned int geometryType, const Dimensions& dimensions, unsigned int volumeTextureRole ) const
{
    base::Spatial* const grid = createSpatial( glc, geometryType, volumeTextureRole );
    base::Node* const pivot = new base::Node();
    pivot->localTransform = base::math::scaling4f( dimensions.millimeters );
    pivot->attachChild( grid );
    return pivot;
}


template< typename HUVolumeSegmentVolume >
HUVolumeGridHelper< HUVolumeSegmentVolume >::Spacing::Spacing( const base::math::Vector3f& millimeters )
    : millimeters( millimeters )
{
}


template< typename HUVolumeSegmentVolume >
HUVolumeGridHelper< HUVolumeSegmentVolume >::Dimensions::Dimensions( const base::math::Vector3f& millimeters )
    : millimeters( millimeters )
{
}



}  // namespace Carna :: helpers

}  // namespace Carna

#endif // HUVOLUMEGRIDHELPER_H_6014714286
