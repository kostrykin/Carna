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

#ifndef VOLUMEGRIDHELPER_H_6014714286
#define VOLUMEGRIDHELPER_H_6014714286

#include <Carna/helpers/VolumeGridHelperDetails.h>
#include <Carna/Carna.h>
#include <Carna/base/math.h>
#include <Carna/base/VolumeGrid.h>
#include <Carna/base/VolumeSegment.h>
#include <Carna/base/Geometry.h>
#include <Carna/base/BoundingBox.h>
#include <memory>
#include <cmath>

/** \file   VolumeGridHelper.h
  * \brief  Defines \ref Carna::helpers::VolumeGridHelper.
  */

namespace Carna
{

namespace helpers
{



// ----------------------------------------------------------------------------------
// VolumeGridHelper
// ----------------------------------------------------------------------------------

/** \brief
  * Computes the \ref VolumePartitioning "partitioning of volumetric data" and the
  * corresponding normal map. Also creates \ref SceneGraph "scene nodes" that insert
  * the volumetric data into a scene.
  *
  * \param SegmentHUVolumeType is the \ref base::BufferedHUVolume compatible type to
  *     use for storing the HU volume of a single partition.
  *
  * \param SegmentNormalsVolumeType is the \ref base::BufferedNormalMap3D compatible
  *     type to use for storing the normal map of a single partition. Set to `void`
  *     if the normal map is not required.
  *
  * \section VolumeGridHelperNormals Normal Map Computation
  *
  * The \ref loadData kicks off the computation of the normals automatically. If you
  * alter the volume data differently, it is within your responsibility to do this by
  * calling `computeNormals` on this object. Note that the `computeNormals` method is
  * only available if \a SegmentNormalsVolumeType is not `void`.
  *
  * \section VolumeGridHelperResolutions Resolutions
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
  * \date   8.3.15 - 29.3.15
  */
template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
class VolumeGridHelper
    : public details::VolumeGridHelper::HUComponent     < SegmentHUVolumeType, SegmentNormalsVolumeType >
    , public details::VolumeGridHelper::NormalsComponent< SegmentHUVolumeType, SegmentNormalsVolumeType >
{

    NON_COPYABLE

    typedef details::VolumeGridHelper::HUComponent     < SegmentHUVolumeType, SegmentNormalsVolumeType > HUComponent;
    typedef details::VolumeGridHelper::NormalsComponent< SegmentHUVolumeType, SegmentNormalsVolumeType > NormalsComponent;

    /** \brief
      * Holds the wrapped \ref base::VolumeGrid object.
      */
    std::unique_ptr< base::VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType > > myGrid;

public:

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    /** \brief
      * Default maximum memory size of a single segment volume, 50 megabytes
      * approximately. This determines the segments partitioning.
      */
    const static std::size_t DEFAULT_MAX_SEGMENT_BYTESIZE = 2 * 300 * 300 * 300;

    /** \brief
      * Creates new \ref base::VolumeGrid object. Initializes its' segments s.t. the
      * totally covered resolution is \a resolution at least. It may be chosen larger
      * if the buffers' resolution need to be rounded to even numbers.
      *
      * \param nativeResolution
      *     The resolution the grid is to be prepared for. This is the resolution
      *     that will be expected from the \ref loadData "data source".
      *
      * \param maxSegmentBytesize
      *     Maximum memory size of a single segment volume. The segments partitioning
      *     is chosen according to this value.
      */
    VolumeGridHelper( const base::math::Vector3ui& nativeResolution, std::size_t maxSegmentBytesize = DEFAULT_MAX_SEGMENT_BYTESIZE );

    /** \brief
      * Holds the original resolution of the loaded data.
      */
    const base::math::Vector3ui nativeResolution;

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
      * Alters the volume data.
      *
      * \param data
      *     Unary function that maps \ref base::math::Vector3ui to \ref base::HUV. It
      *     will be queried for all values up to \ref nativeResolution.
      *
      * The normal map is re-computed if `SegmentNormalsVolumeType` is not `void`.
      */
    template< typename UnaryVector3uiToHUVFunction >
    void loadData( const UnaryVector3uiToHUVFunction& data );
    
    /** \brief
    * Releases all previously acquired textures. Invoke this method when the volume
    * data changes, \ref loadData already does takes care of that.
    *
    * If this method is not invoked after an update of the volume data, succeeding
    * calls to \ref createNode will not reflect the new data. Note however, that if
    * you call this method between two invocations of \ref createNode without the
    * volume data been altered, same textures will get uploaded twice to video
    * memory, i.e. video resources will be wasted.
      */
    void releaseGeometryFeatures();

    /** \brief
      * References the underlying grid.
      */
    base::VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >& grid() const;

    /** \brief
      * Specifies the spacing between two succeeding voxel centers in millimeters.
      */
    struct Spacing
    {
        /** \brief
          * Instantiates.
          */
        explicit Spacing( const base::math::Vector3f& millimeters );

        /** \brief
          * Holds the spacing between two succeeding voxel centers in millimeters.
          */
        base::math::Vector3f millimeters;
    };
    
    /** \brief
      * Specifies the dimensions of the whole dataset in millimeters.
      */
    struct Dimensions
    {
        /** \brief
          * Instantiates.
          */
        explicit Dimensions( const base::math::Vector3f& millimeters );

        /** \brief
          * Holds the dimensions of the whole dataset in millimeters.
          */
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
      * \param geometryType
      *     Will be used for \ref base::Geometry instantiation.
      *
      * \param spacing
      *     Specifies the spacing between two succeeding voxel centers in
      *     millimeters.
      */
    base::Node* createNode( unsigned int geometryType, const Spacing& spacing ) const;
    
    /** \brief
      * Creates renderable representation of the underlying grid, that can be put
      * anywhere in the scene graph. The volume is centered in the node.
      *
      * \warning
      *     Only change the returned node's \ref base::Spatial::localTransform
      *     "localTransform" attribute when you know what you're doing! Put it into
      *     another node otherwise.
      *
      * \param geometryType
      *     Will be used for \ref base::Geometry instantiation.
      *
      * \param dimensions
      *     Specifies the dimensions of the whole dataset in millimeters.
      */
    base::Node* createNode( unsigned int geometryType, const Dimensions& dimensions ) const;
    
protected:

    virtual base::math::Vector3ui gridResolution() const override;

private:

    base::Node* createNode
        ( unsigned int geometryType
        , const Spacing& spacing
        , const Dimensions& dimensions ) const;

    static base::math::Vector3ui computeMaxSegmentSize( const base::math::Vector3ui& resolution, std::size_t maxSegmentBytesize );

}; // VolumeGridHelper


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
base::math::Vector3ui VolumeGridHelper< SegmentHUVolumeType, SegmentNormalsVolumeType >::computeMaxSegmentSize
    ( const base::math::Vector3ui& resolution, std::size_t maxSegmentBytesize )
{
    const float maxSideLengthF = std::pow
        ( maxSegmentBytesize / static_cast< float >( sizeof( typename SegmentHUVolumeType::Voxel ) ), 1.f / 3 );
    const unsigned int maxSideLength = base::math::makeEven( base::math::round_ui( maxSideLengthF ), -1 );

    /* We subtract the redundant texels from effective segment size.
     * Note that this causes the effective maximum segment size to be odd.
     */
    return base::math::Vector3ui( maxSideLength - 1, maxSideLength - 1, maxSideLength - 1 );
}


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
VolumeGridHelper< SegmentHUVolumeType, SegmentNormalsVolumeType >::VolumeGridHelper
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
    myGrid.reset( new base::VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >( maxSegmentSize, segmentCounts ) );
    NormalsComponent::setGrid( *myGrid );

    CARNA_FOR_VECTOR3UI( segmentCoord, myGrid->segmentCounts )
    {
        /* Here we add the redundant texels to the buffer size considerations. This
         * is fine because 'regularSegmentSize' contains the effective segment size.
         */
        const base::math::Vector3ui segmentSize
            ( segmentCoord.x() + 1 == myGrid->segmentCounts.x() ? tails.x() : regularSegmentSize.x() + 1
            , segmentCoord.y() + 1 == myGrid->segmentCounts.y() ? tails.y() : regularSegmentSize.y() + 1
            , segmentCoord.z() + 1 == myGrid->segmentCounts.z() ? tails.z() : regularSegmentSize.z() + 1 );

        HUComponent     ::initializeSegment( myGrid->segmentAt( segmentCoord ), segmentSize );
        NormalsComponent::initializeSegment( myGrid->segmentAt( segmentCoord ), segmentSize );
    }
}


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
void VolumeGridHelper< SegmentHUVolumeType, SegmentNormalsVolumeType >::releaseGeometryFeatures()
{
    HUComponent     ::releaseGeometryFeatures();
    NormalsComponent::releaseGeometryFeatures();
}


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
template< typename UnaryVector3uiToHUVFunction >
void VolumeGridHelper< SegmentHUVolumeType, SegmentNormalsVolumeType >::loadData
    ( const UnaryVector3uiToHUVFunction& data )
{
    releaseGeometryFeatures();
    CARNA_FOR_VECTOR3UI( coord, resolution )
    {
        const bool outOfNativeBounds
            =  coord.x() >= nativeResolution.x()
            || coord.y() >= nativeResolution.y()
            || coord.z() >= nativeResolution.z();
        const base::HUV huv = outOfNativeBounds ? -1024 : data( coord );
        myGrid->template setVoxel< typename base::VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >::HUVSelector >( coord, huv );
    }
    NormalsComponent::computeNormals();
}


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
base::VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >&
    VolumeGridHelper< SegmentHUVolumeType, SegmentNormalsVolumeType >::grid() const
{
    return *myGrid;
}


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
base::Node* VolumeGridHelper< SegmentHUVolumeType, SegmentNormalsVolumeType >::createNode
    ( unsigned int geometryType, const Spacing& spacing, const Dimensions& dimensions ) const
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
    CARNA_FOR_VECTOR3UI( segmentCoord, myGrid->segmentCounts )
    {
        const base::VolumeSegment< SegmentHUVolumeType, SegmentNormalsVolumeType >& segment = myGrid->segmentAt( segmentCoord );

        /* Compute dimensions of particular grid segment.
         */
        const bool isTail =
               segmentCoord.x() + 1 == myGrid->segmentCounts.x()
            || segmentCoord.y() + 1 == myGrid->segmentCounts.y()
            || segmentCoord.z() + 1 == myGrid->segmentCounts.z();
        const base::math::Vector3ui& volumeSize = segment.huVolume().size;
        const base::math::Vector3f dimensions = !isTail ? regularSegmentDimensions
            : ( ( volumeSize.cast< int >() - base::math::Vector3i( 1, 1, 1 ) )
                .cast< float >().cwiseProduct( spacing.millimeters ) );

        /* Create geometry node for particular grid segment.
         */
        base::Geometry* const geom = new base::Geometry( geometryType );
        pivot->attachChild( geom );
        HUComponent     ::attachTexture( *geom, segment );
        NormalsComponent::attachTexture( *geom, segment );
        geom->setMovable( false );
        geom->setBoundingVolume( new base::BoundingBox( 1, 1, 1 ) );
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


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
base::Node* VolumeGridHelper< SegmentHUVolumeType, SegmentNormalsVolumeType >::createNode
    ( unsigned int geometryType, const Spacing& spacing ) const
{
    const base::math::Vector3f dimensions
        = ( resolution.cast< int >() - base::math::Vector3i( 1, 1, 1 ) ).cast< float >().cwiseProduct( spacing.millimeters );
    return createNode( geometryType, spacing, Dimensions( dimensions ) );
}


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
base::Node* VolumeGridHelper< SegmentHUVolumeType, SegmentNormalsVolumeType >::createNode
    ( unsigned int geometryType, const Dimensions& dimensions ) const
{
    const base::math::Vector3f& mmDimensions = dimensions.millimeters;
    const base::math::Vector3f spacing
        = mmDimensions.cast< float >().cwiseQuotient( ( resolution.cast< int >() - base::math::Vector3i( 1, 1, 1 ) ) );
    return createNode( geometryType, Spacing( spacing ), dimensions );
}


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
base::math::Vector3ui VolumeGridHelper< SegmentHUVolumeType, SegmentNormalsVolumeType >::gridResolution() const
{
    return resolution;
}


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
VolumeGridHelper< SegmentHUVolumeType, SegmentNormalsVolumeType >::Spacing::Spacing( const base::math::Vector3f& millimeters )
    : millimeters( millimeters )
{
}


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
VolumeGridHelper< SegmentHUVolumeType, SegmentNormalsVolumeType >::Dimensions::Dimensions( const base::math::Vector3f& millimeters )
    : millimeters( millimeters )
{
}



}  // namespace Carna :: helpers

}  // namespace Carna

#endif // VOLUMEGRIDHELPER_H_6014714286
