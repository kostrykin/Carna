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

/** \brief
  * Initializes, holds and manages an \ref base::HUVolumeGrid object.
  * Creates representative scene nodes in particular.
  *
  * This class needs to distinguish between three kinds of resolutions. The grid's
  * volume textures are \em not disjoint, but must maintain redundant voxels along
  * common segment faces. Hence, the resolution of the data uploaded to GPU from all
  * segments, that we will call the \em total resolution therefore, will usually be
  * greater than the resolution of the actually \ref loadData "loaded data". We will
  * refer to the latter as the \em native resolution.
  *
  * Furthermore, the \em effective resolution, that is the one covered by the grid
  * and available for payload, might still be greater than the native resolution,
  * namely because the segments' resolution is rounded to even numbers. The
  * additional voxels arising from this are \em not queried from the data source, but
  * automatically padded with \f$-1024\f$, s.t. the data source doesn't have to pay
  * attention to this circumstance.
  *
  * \author Leonid Kostrykin
  * \date   8.3.15 - 10.3.15
  */
template< typename HUVolumeSegmentVolume >
class HUVolumeGridHelper
{

    NON_COPYABLE

    /** \brief
      * Holds the original resolution of the loaded data.
      */
    const base::math::Vector3ui nativeResolution;

    /** \brief
      * Holds the wrapped \ref base::HUVolumeGrid object.
      */
    std::unique_ptr< base::HUVolumeGrid< HUVolumeSegmentVolume > > myGrid;

    /** \brief
      * Caches volume textures created from \ref myGrid.
      */
    mutable std::map< const typename base::HUVolumeGrid< HUVolumeSegmentVolume >::HUVolumeSegment*, base::Texture3D* > textures;

public:

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    /** \brief
      * Default maximum memory size of a single segment volume, 50 megabytes
      * approximately. This determines the segments partitioning.
      */
    const static std::size_t DEFAULT_MAX_SEGMENT_BYTESIZE = 2 * 300 * 300 * 300;

    const static unsigned int DEFAULT_VOLUME_TEXTURE_ROLE = 0;

    /** \brief
      * Creates new \ref base::HUVolumeGrid object. Initializes its' segments s.t.
      * the totally covered resolution is \a resolution at least. It may be chosen
      * larger if the buffers' resolution need to be rounded to even numbers.
      *
      * \param nativeResolution
      *     The resolution the grid is to be prepared for. This is the resolution
      *     that will be expected from the \ref loadData "data source".
      *
      * \param maxSegmentBytesize
      *     Maximum memory size of a single segment volume. The segments partitioning
      *     is chosen according to this value.
      */
    HUVolumeGridHelper( const base::math::Vector3ui& nativeResolution, std::size_t maxSegmentBytesize = DEFAULT_MAX_SEGMENT_BYTESIZE );

    /** \brief
      * Checks whether \ref invalidateTextures has been invoked before, if required.
      * An error is logged occasionally.
      */
    ~HUVolumeGridHelper();

    /** \brief
      * Holds the effective resolution, i.e. the resolution covered by the grid.
      */
    const base::math::Vector3ui resolution;

    /** \brief
      * Maximum memory size of a single segment volume.
      */
    const std::size_t maxSegmentBytesize;

    /** \brief
      * The maximum effective resolution of a single grid segment.
      */
    const base::math::Vector3ui maxSegmentSize;

    /** \brief
      * The effective resolution of each grid segment that is not the last along an
      * arbitrary axis.
      */
    const base::math::Vector3ui regularSegmentSize;

    /** \brief
      * Alters the volume data. You must also call \ref invalidateTextures if
      * \ref createNode was invoked previously, in order for succeeding invocations
      * to \ref createNode to reflect the changes made to the volume data.
      *
      * \param data
      *     Unary function that maps \ref base::math::Vector3ui to \ref base::HUV. It
      *     will be queried for all values up to \ref nativeResolution.
      */
    template< typename UnaryVector3uiToHUVFunction >
    void loadData( const UnaryVector3uiToHUVFunction& data );

    /** \brief
      * Releases all previously acquired textures.
      *
      * \warning
      *     Always invoke this method at least once before deleting
      *     \c %HUVolumeGridHelper instances, if \ref createNode might have been
      *     called on the instance earlier!
      *
      * You might also want to invoke this method when the volume data changes, e.g.
      * through \ref loadData. This will allow video resources, that were occupied by
      * earlier calls to \ref createNode, to be freed sooner.
      *
      * \attention
      *     Note however, that if you call this method between two invocations of
      *     \ref createNode, without the volume data being altered, same textures
      *     will get uploaded twice to video memory, i.e. video resources will be
      *     wasted.
      */
    void invalidateTextures( const base::GLContext& glc );

    /** \brief
      * References the underlying grid.
      */
    base::HUVolumeGrid< HUVolumeSegmentVolume >& grid() const;

    /** \brief
      * Specifies the spacing between two succeeding voxel centers in millimeters.
      */
    struct Spacing
    {
        explicit Spacing( const base::math::Vector3f& millimeters );

        base::math::Vector3f millimeters;
    };
    
    /** \brief
      * Specifies the dimensions of the whole dataset in millimeters.
      */
    struct Dimensions
    {
        explicit Dimensions( const base::math::Vector3f& millimeters );

        base::math::Vector3f millimeters;
    };

    /** \brief
      * Creates renderable representation of the underlying grid, that can be put
      * anywhere in the scene graph. The volume is centered in the node.
      *
      * \warning
      *     Only change the returned node's \ref base::Spatial::localTransform
      *     "localTransform" attribute when you know what you're doing! Put it into
      *     another node otherwise.
      *
      * Also make sure you have called \ref invalidateTextures before, in case the
      * volume data has been altered, e.g. through \ref loadData,
      * since \c %createNode was called the last time.
      *
      * \param glc
      *     References the GL context wrapper that will be activated before uploading
      *     textures to video memory.
      *
      * \param geometryType
      *     Will be used for \ref base::Geometry instantiation.
      *
      * \param spacing
      *     Specifies the spacing between two succeeding voxel centers in
      *     millimeters.
      *
      * \param volumeTextureRole
      *     Will be used to \ref base::Geometry::putFeature
      *     "put the volume textures on the Geometry objects".
      */
    base::Node* createNode
        ( const base::GLContext& glc
        , unsigned int geometryType
        , const Spacing& spacing
        , unsigned int volumeTextureRole = DEFAULT_VOLUME_TEXTURE_ROLE ) const;
    
    /** \brief
      * Creates renderable representation of the underlying grid, that can be put
      * anywhere in the scene graph. The volume is centered in the node.
      *
      * \warning
      *     Only change the returned node's \ref base::Spatial::localTransform
      *     "localTransform" attribute when you know what you're doing! Put it into
      *     another node otherwise.
      *
      * Also make sure you have called \ref invalidateTextures before, in case the
      * volume data has been altered, e.g. through \ref loadData, since
      * \c %createNode was called the last time.
      *
      * \param glc
      *     References the GL context wrapper that will be activated before uploading
      *     textures to video memory.
      *
      * \param geometryType
      *     Will be used for \ref base::Geometry instantiation.
      *
      * \param dimensions
      *     Specifies the dimensions of the whole dataset in millimeters.
      *
      * \param volumeTextureRole
      *     Will be used to \ref base::Geometry::putFeature
      *     "put the volume textures on the Geometry objects".
      */
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
        ( maxSegmentBytesize / static_cast< float >( sizeof( typename HUVolumeSegmentVolume::Voxel ) ), 1.f / 3 );
    const unsigned int maxSideLength = base::math::makeEven( base::math::round_ui( maxSideLengthF ), -1 );

    /* We subtract the redundant texels from effective segment size.
     * Note that this causes the effective maximum segment size to be odd.
     */
    return base::math::Vector3ui( maxSideLength - 1, maxSideLength - 1, maxSideLength - 1 );
}


template< typename HUVolumeSegmentVolume >
HUVolumeGridHelper< HUVolumeSegmentVolume >::HUVolumeGridHelper
        ( const base::math::Vector3ui& nativeResolution
        , std::size_t maxSegmentBytesize )
    : nativeResolution( nativeResolution )
    , resolution( base::math::makeEven( nativeResolution, +1 ) )
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
            =  coord.x() >= nativeResolution.x()
            || coord.y() >= nativeResolution.y()
            || coord.z() >= nativeResolution.z();
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
        const typename base::HUVolumeGrid< HUVolumeSegmentVolume >::HUVolumeSegment& segment = myGrid->segmentAt( segmentCoord );

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
        const base::math::Vector3ui& volumeSize = segment.volume().size;
        const base::math::Vector3f dimensions = !isTail ? regularSegmentDimensions
            : ( ( volumeSize.cast< int >() - base::math::Vector3i( 1, 1, 1 ) )
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
    const base::math::Vector3f& mmDimensions = dimensions.millimeters;
    const base::math::Vector3f spacing
        = mmDimensions.cast< float >().cwiseQuotient( ( resolution.cast< int >() - base::math::Vector3i( 1, 1, 1 ) ) );
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
