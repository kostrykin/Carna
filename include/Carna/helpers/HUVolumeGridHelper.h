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

    const base::math::Vector3ui regularSegmentSize;

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

private:

    base::Node* createNode
        ( const base::GLContext& glc
        , unsigned int geometryType
        , const Spacing& spacing
        , const Dimensions& dimensions
        , unsigned int volumeTextureRole ) const;

    static base::math::Vector3ui computeMaxSegmentSize( const base::math::Vector3ui& resolution, std::size_t maxSegmentBytesize );

}; // HUVolumeGridHelper


template< typename HUVolumeSegmentVolume >
base::math::Vector3ui HUVolumeGridHelper< HUVolumeSegmentVolume >::computeMaxSegmentSize
    ( const base::math::Vector3ui& resolution, std::size_t maxSegmentBytesize )
{
    const float maxSideLengthF = std::pow
        ( maxSegmentBytesize / static_cast< float >( sizeof( HUVolumeSegmentVolume::VoxelType ) ), 1.f / 3 );
    const unsigned int maxSideLength = base::math::makeEven( base::math::round_ui( maxSideLengthF ), -1 );

    /* We subtract the redundant texels from effective segment size.
     * Note that this causes the effective maximum segment size to be odd.
     */
    return base::math::Vector3ui( maxSideLength - 1, maxSideLength - 1, maxSideLength - 1 );
}


template< typename HUVolumeSegmentVolume >
HUVolumeGridHelper< HUVolumeSegmentVolume >::HUVolumeGridHelper
        ( const base::math::Vector3ui& originalResolution
        , std::size_t maxSegmentBytesize )
    : originalResolution( originalResolution )
    , resolution( base::math::makeEven( originalResolution, +1 ) )
    , maxSegmentBytesize( maxSegmentBytesize )
    , maxSegmentSize( computeMaxSegmentSize( resolution, maxSegmentBytesize ) )
    , regularSegmentSize( maxSegmentSize.cwiseMin( resolution ) )
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

    CARNA_FOR_VECTOR3UI( segmentCoord, myGrid->segmentCounts )
    {
        /* Here we add the redundant texels to the buffer size considerations. This
         * is fine because 'regularSegmentSize' contains the effective segment size.
         */
        const base::math::Vector3ui volumeSize
            ( segmentCoord.x() + 1 == myGrid->segmentCounts.x() ? tails.x() : regularSegmentSize.x() + 1
            , segmentCoord.y() + 1 == myGrid->segmentCounts.y() ? tails.y() : regularSegmentSize.y() + 1
            , segmentCoord.z() + 1 == myGrid->segmentCounts.z() ? tails.z() : regularSegmentSize.z() + 1 );

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
    CARNA_FOR_VECTOR3UI( coord, resolution )
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
base::Node* HUVolumeGridHelper< HUVolumeSegmentVolume >::createNode
    ( const base::GLContext& glc
    , unsigned int geometryType
    , const Spacing& spacing
    , const Dimensions& dimensions
    , unsigned int volumeTextureRole ) const
{
    /* Compute dimensions of a regular grid segment,
     * taking the redundant texels into account.
     */
    const base::math::Vector3f regularSegmentDimensions = spacing.millimeters.cwiseProduct( regularSegmentSize.cast< float >() );
    
    /* Create pivot node that shifts it's children to a corner.
     */
    base::Node* const pivot = new base::Node();
    pivot->localTransform = base::math::translation4f( ( regularSegmentDimensions - dimensions.millimeters ) / 2 );
    pivot->setMovable( false );

    /* Create geometry nodes for all grid segments.
     */
    glc.makeActive();
    CARNA_FOR_VECTOR3UI( segmentCoord, myGrid->segmentCounts )
    {
        const base::HUVolumeGrid< HUVolumeSegmentVolume >::HUVolumeSegment& segment = myGrid->segmentAt( segmentCoord );

        /* Check whether the texture already is available or needs to be uploaded.
         */
        auto textureItr = textures.find( &segment );
        base::Texture3D* texture;
        if( textureItr == textures.end() )
        {
            /* Upload the texture to video memory.
             */
            texture = &base::BufferedHUVolumeTexture< HUVolumeSegmentVolume >::create( segment.volume() );
            textures[ &segment ] = texture;
        }
        else
        {
            texture = textureItr->second;
        }

        /* Compute dimensions of particular grid segment.
         */
        const bool isTail =
               segmentCoord.x() + 1 == myGrid->segmentCounts.x()
            || segmentCoord.y() + 1 == myGrid->segmentCounts.y()
            || segmentCoord.z() + 1 == myGrid->segmentCounts.z();
        const base::math::Vector3f dimensions = !isTail ? regularSegmentDimensions
            : ( ( segment.volume().size.cast< int >() - base::math::Vector3i( 1, 1, 1 ) )
                .cast< float >().cwiseProduct( spacing.millimeters ) );

        /* Create geometry node for particular grid segment.
         */
        base::Geometry* const geom = new  base::Geometry( geometryType );
        pivot->attachChild( geom );
        geom->putFeature( volumeTextureRole, *texture );
        geom->setMovable( false );
        geom->localTransform
            = base::math::translation4f( segmentCoord.cast< float >().cwiseProduct( regularSegmentDimensions )
                - ( !isTail
                    ? base::math::Vector3i( 0, 0, 0 )
                    : regularSegmentDimensions.cast< int >() - dimensions.cast< int >() ).cast< float >() / 2 )
            * base::math::scaling4f( dimensions );
    }

    /* We're done.
     */
    return pivot;
}


template< typename HUVolumeSegmentVolume >
base::Node* HUVolumeGridHelper< HUVolumeSegmentVolume >::createNode
    ( const base::GLContext& glc, unsigned int geometryType, const Spacing& spacing, unsigned int volumeTextureRole ) const
{
    const base::math::Vector3f dimensions
        = ( resolution.cast< int >() - base::math::Vector3i( 1, 1, 1 ) ).cast< float >().cwiseProduct( spacing.millimeters );
    return createNode( glc, geometryType, spacing, Dimensions( dimensions ), volumeTextureRole );
}


template< typename HUVolumeSegmentVolume >
base::Node* HUVolumeGridHelper< HUVolumeSegmentVolume >::createNode
    ( const base::GLContext& glc, unsigned int geometryType, const Dimensions& dimensions, unsigned int volumeTextureRole ) const
{
    const base::math::Vector3f spacing
        = dimensions.millimeters.cast< float >().cwiseQuotient( ( dimensions.cast< int >() - base::math::Vector3i( 1, 1, 1 ) ) );
    return createNode( glc, geometryType, Spacing( spacing ), dimensions, volumeTextureRole );
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
