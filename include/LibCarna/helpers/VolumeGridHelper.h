/*
 *  Copyright (C) 2010 - 2016 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 * 
 * 
 *  Copyright (C) 2021 - 2025 Leonid Kostrykin
 * 
 */

#ifndef VOLUMEGRIDHELPER_H_6014714286
#define VOLUMEGRIDHELPER_H_6014714286

#include <LibCarna/helpers/VolumeGridHelperDetails.h>
#include <LibCarna/LibCarna.h>
#include <LibCarna/base/Node.h>
#include <LibCarna/base/math.h>
#include <LibCarna/base/VolumeGrid.h>
#include <LibCarna/base/VolumeSegment.h>
#include <LibCarna/base/Geometry.h>
#include <LibCarna/base/BoundingBox.h>
#include <memory>
#include <cmath>

/** \file   VolumeGridHelper.h
  * \brief  Defines \ref LibCarna::helpers::VolumeGridHelper.
  */

namespace LibCarna
{

namespace helpers
{



// ----------------------------------------------------------------------------------
// VolumeGridHelperBase
// ----------------------------------------------------------------------------------

/** \brief
  * Defines type-parameters-independent \ref VolumeGridHelper base interface.
  *
  * \author Leonid Kostrykin
  */
class LIBCARNA VolumeGridHelperBase
{

public:

    /** \brief
      * Default maximum memory size of a single segment volume, 50 megabytes
      * approximately. This determines the segments partitioning.
      *
      * \since \ref v_3_0_1
      */
    const static std::size_t DEFAULT_MAX_SEGMENT_BYTESIZE = 2 * 300 * 300 * 300;

    /** \brief
      * Instantiates.
      *
      * \param nativeResolution is the resolution the grid is to be prepared for.
      *
      * \since \ref v_3_0_1
      */
    VolumeGridHelperBase( const base::math::Vector3ui& nativeResolution );

    /** \brief
      * Holds the original resolution of the loaded data.
      */
    const base::math::Vector3ui nativeResolution;

    /** \brief
      * Does nothing.
      */
    virtual ~VolumeGridHelperBase();

    /** \brief
     * Releases all previously acquired textures. Invoke this method when the volume
     * data changes.
     *
     * \copydetails VolumeGridHelper::releaseGeometryFeatures
     */
    virtual void releaseGeometryFeatures() = 0;

    /** \brief
      * Specifies the spacing between two succeeding voxel centers.
      */
    struct LIBCARNA Spacing
    {
        /** \brief
          * Instantiates.
          */
        explicit Spacing( const base::math::Vector3f& units );

        /** \brief
          * Holds the spacing between two succeeding voxel centers.
          */
        base::math::Vector3f units;
    };
    
    /** \brief
      * Specifies the extent of the whole dataset.
      */
    struct LIBCARNA Extent
    {
        /** \brief
          * Instantiates.
          */
        explicit Extent( const base::math::Vector3f& units );

        /** \brief
          * Holds the extent of the whole dataset in units.
          */
        base::math::Vector3f units;
    };

    /** \brief
      * Creates renderable representation of the underlying grid, that can be put anywhere in the scene graph. The
      * volume is centered in the node.
      *
      * \warning
      * Only change the returned node's \ref base::Spatial::localTransform "localTransform" attribute when you know
      * what you're doing! Put it into another node otherwise.
      *
      * \param geometryType
      * Will be used for \ref base::Geometry instantiation.
      *
      * \param spacing
      * Specifies the spacing between two succeeding voxel centers.
      */
    virtual base::Node* createNode( unsigned int geometryType, const Spacing& spacing ) const = 0;
    
    /** \brief
      * Creates renderable representation of the underlying grid, that can be put anywhere in the scene graph. The
      * volume is centered in the node.
      *
      * \warning
      * Only change the returned node's \ref base::Spatial::localTransform "localTransform" attribute when you know
      * what you're doing! Put it into another node otherwise.
      *
      * \param geometryType
      * Will be used for \ref base::Geometry instantiation.
      *
      * \param extent
      * Specifies the extent of the whole dataset.
      */
    virtual base::Node* createNode( unsigned int geometryType, const Extent& extent ) const = 0;
    
    /** \brief
      * Updates the data of the volume grid.
      *
      * The \a intensityData must be scaled to \f$\left[0, 1\right]\f$.
      */
    virtual void loadIntensities( const std::function< float( const base::math::Vector3ui& ) >& intensityData ) = 0;

    /** \overload
      */
    template< typename LoadIntensitiesFunction >
    void loadIntensities( const LoadIntensitiesFunction& intensityData );

    /** \brief
      * Updates the data of the volume grid.
      *
      * The \a huData must be scaled to \f$\left[-1024, 3071\right]\f$.
      */
    void loadHUData( const std::function< base::HUV( const base::math::Vector3ui& ) >& huData );

    /** \overload
      */
    template< typename LoadHUDataFunction >
    void loadHUData( const LoadHUDataFunction& huData );

}; // VolumeGridHelperBase


template< typename LoadIntensitiesFunction >
void VolumeGridHelperBase::loadIntensities( const LoadIntensitiesFunction& intensityData )
{
    loadIntensities( static_cast< const std::function< float( const base::math::Vector3ui& ) >& >
        (
            [&intensityData]( const base::math::Vector3ui& loc ) -> float
            {
                return intensityData( loc );
            }
        )
    );
}


template< typename LoadHUDataFunction >
void VolumeGridHelperBase::loadHUData( const LoadHUDataFunction& huData )
{
    loadHUData( static_cast< const std::function< base::HUV( const base::math::Vector3ui& ) >& >
        (
            [&huData]( const base::math::Vector3ui& loc )
            {
                return huData( loc );
            }
        )
    );
}



// ----------------------------------------------------------------------------------
// VolumeGridHelper
// ----------------------------------------------------------------------------------

/** \brief
  * Computes the \ref VolumePartitioning "partitioning of volumetric data" and the corresponding normal map. Also
  * creates \ref SceneGraph "scene nodes" that insert the volumetric data into a scene.
  *
  * \param SegmentIntensityVolumeType
  * is the \ref base::BufferedIntensityVolume compatible type to use for storing the intensity volume of a single
  * partition.
  *
  * \param SegmentNormalsVolumeType
  * is the \ref base::BufferedNormalMap3D compatible type to use for storing the normal map of a single partition. Set
  * to `void` if a normal map is not required.
  *
  * \section VolumeGridHelperNormals Normal Map Computation
  *
  * The \ref loadIntensities method performs the computation of the normals automatically. If the intensities are
  * changed in a different way, it is within your responsibility to perform the computation of the normals by calling
  * `computeNormals` on this object. Note that the `computeNormals` method is only available if
  * \a SegmentNormalsVolumeType is not `void`.
  *
  * \section VolumeGridHelperResolutions Resolutions
  *
  * This class needs to distinguish between three kinds of resolutions. The grid's volume textures are \em not disjoint,
  * but must maintain redundant voxels along the segment faces that they have in common. Hence, the resolution of the
  * data uploaded to the GPU from all segments, that is therefore called the \em total resolution, will usually be
  * greater than the \em native resolution of the data passed to \ref loadIntensities.
  *
  * Furthermore, the \em effective resolution, that is the one covered by the grid and available for payload, might
  * still be greater than the native resolution, because the resolution of the segments is rounded to even numbers. The
  * additional voxels arising from this are \em not queried from the data source, but padded automatically.
  */
template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
class VolumeGridHelper
    : public VolumeGridHelperBase
    , public details::VolumeGridHelper::IntensityComponent< SegmentIntensityVolumeType, SegmentNormalsVolumeType >
    , public details::VolumeGridHelper::  NormalsComponent< SegmentIntensityVolumeType, SegmentNormalsVolumeType >
{

    NON_COPYABLE

    typedef details::VolumeGridHelper::IntensityComponent< SegmentIntensityVolumeType, SegmentNormalsVolumeType > IntensityComponent;
    typedef details::VolumeGridHelper::  NormalsComponent< SegmentIntensityVolumeType, SegmentNormalsVolumeType >   NormalsComponent;

    /** \brief
      * Holds the wrapped \ref base::VolumeGrid object.
      */
    std::unique_ptr< base::VolumeGrid< SegmentIntensityVolumeType, SegmentNormalsVolumeType > > myGrid;

public:

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    /** \brief
      * Creates a new \ref base::VolumeGrid object. Initializes the segments of the grid so that the totally covered
      * resolution is \a resolution or larger. It will be larger if the resolution of the buffers need to be rounded
      * to even numbers.
      *
      * \param nativeResolution
      * The resolution the grid is to be prepared for. This is the resolution that will be expected from the
      * \ref loadIntensities "data source".
      *
      * \param maxSegmentBytesize
      * Maximum memory size of a single segment volume. The segments partitioning is chosen according to this value.
      */
    VolumeGridHelper( const base::math::Vector3ui& nativeResolution, std::size_t maxSegmentBytesize = DEFAULT_MAX_SEGMENT_BYTESIZE );

    /** \brief
      * Maximum memory size of a single segment volume.
      */
    const std::size_t maxSegmentBytesize;

    /** \brief
      * The maximum effective resolution of a single grid segment.
      */
    const base::math::Vector3ui maxSegmentSize;

    /** \brief
      * Describes the partitioning along the x-axis.
      */
    const details::VolumeGridHelper::Partionining partitioningX;
    
    /** \brief
      * Describes the partitioning along the y-axis.
      */
    const details::VolumeGridHelper::Partionining partitioningY;
    
    /** \brief
      * Describes the partitioning along the z-axis.
      */
    const details::VolumeGridHelper::Partionining partitioningZ;

    /** \brief
      * Holds the effective resolution, i.e. the resolution covered by the grid.
      */
    const base::math::Vector3ui resolution;

    /** \brief
      * Updates the data of the volume grid.
      *
      * The \a intensityData must be scaled to \f$\left[0, 1\right]\f$.
      *
      * \param intensityData
      * Unary function that maps \ref base::math::Vector3ui to an intensity value. It will be queried for all values up
      * to \ref nativeResolution.
      *
      * The normal map is re-computed if `SegmentNormalsVolumeType` is not `void`.
      */
    virtual void loadIntensities( const std::function< float( const base::math::Vector3ui& ) >& intensityData ) override;
    
    /** \brief
      * Releases all previously acquired textures. Invoke this method when the volume data changes,
      * \ref loadIntensities already does takes care of that.
      *
      * If this method is not invoked after an update of the volume data, succeeding calls to \ref createNode will not
      * reflect the new data. Note however, that if you call this method between two invocations of \ref createNode
      * without the volume data been altered, same textures will get uploaded twice to video memory, i.e. video
      * resources will be wasted.
      */
    virtual void releaseGeometryFeatures() override;

    /** \brief
      * References the underlying grid.
      */
    base::VolumeGrid< SegmentIntensityVolumeType, SegmentNormalsVolumeType >& grid() const;

    virtual base::Node* createNode( unsigned int geometryType, const Spacing& spacing ) const override;
    
    virtual base::Node* createNode( unsigned int geometryType, const Extent& extent ) const override;
    
protected:

    virtual base::math::Vector3ui gridResolution() const override;

private:

    base::Node* createNode
        ( unsigned int geometryType
        , const Spacing& spacing
        , const Extent& extent ) const;

    static base::math::Vector3ui computeMaxSegmentSize
        ( const base::math::Vector3ui& nativeResolution
        , std::size_t maxSegmentBytesize );

}; // VolumeGridHelper


template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
base::math::Vector3ui VolumeGridHelper< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::computeMaxSegmentSize
    ( const base::math::Vector3ui& nativeResolution, std::size_t maxSegmentBytesize )
{
    const float maxSideLengthF = std::pow
        ( maxSegmentBytesize / static_cast< float >( sizeof( typename SegmentIntensityVolumeType::Voxel ) ), 1.f / 3 );
    const unsigned int maxSideLength = base::math::makeEven( base::math::round_ui( maxSideLengthF ), -1 );

    /* We subtract the redundant texels from effective segment size.
     * Note that this causes the effective maximum segment size to be odd.
     */
    return base::math::Vector3ui( maxSideLength - 1, maxSideLength - 1, maxSideLength - 1 );
}


template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
VolumeGridHelper< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::VolumeGridHelper
        ( const base::math::Vector3ui& nativeResolution
        , std::size_t maxSegmentBytesize )
    : VolumeGridHelperBase( nativeResolution )
    , maxSegmentBytesize( maxSegmentBytesize )
    , maxSegmentSize( computeMaxSegmentSize( nativeResolution, maxSegmentBytesize ) )
    , partitioningX( nativeResolution.x(), maxSegmentSize.x() )
    , partitioningY( nativeResolution.y(), maxSegmentSize.y() )
    , partitioningZ( nativeResolution.z(), maxSegmentSize.z() )
    , resolution( partitioningX.totalSize(), partitioningY.totalSize(), partitioningZ.totalSize() )
{
    const base::math::Vector3ui segmentCounts
        ( partitioningX.partitionsCount()
        , partitioningY.partitionsCount()
        , partitioningZ.partitionsCount() );
    myGrid.reset( new base::VolumeGrid< SegmentIntensityVolumeType, SegmentNormalsVolumeType >( maxSegmentSize, segmentCounts ) );
    NormalsComponent::setGrid( *myGrid );

    LIBCARNA_FOR_VECTOR3UI( segmentCoord, myGrid->segmentCounts )
    {
        /* Here we add the redundant texels to the buffer size considerations.
         */
        const base::math::Vector3ui segmentSize
            ( segmentCoord.x() + 1 == myGrid->segmentCounts.x() ? partitioningX.tailSize : partitioningX.regularPartitionSize + 1
            , segmentCoord.y() + 1 == myGrid->segmentCounts.y() ? partitioningY.tailSize : partitioningY.regularPartitionSize + 1
            , segmentCoord.z() + 1 == myGrid->segmentCounts.z() ? partitioningZ.tailSize : partitioningZ.regularPartitionSize + 1 );

        IntensityComponent::initializeSegment( myGrid->segmentAt( segmentCoord ), segmentSize );
        NormalsComponent  ::initializeSegment( myGrid->segmentAt( segmentCoord ), segmentSize );
    }
}


template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
void VolumeGridHelper< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::releaseGeometryFeatures()
{
    IntensityComponent::releaseGeometryFeatures();
    NormalsComponent  ::releaseGeometryFeatures();
}


template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
void VolumeGridHelper< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::loadIntensities
      ( const std::function< float( const base::math::Vector3ui& ) >& data )
{
    releaseGeometryFeatures();
    LIBCARNA_FOR_VECTOR3UI( coord, resolution )
    {
        const bool outOfNativeBounds
            =  coord.x() >= nativeResolution.x()
            || coord.y() >= nativeResolution.y()
            || coord.z() >= nativeResolution.z();
        const float intensity = outOfNativeBounds ? 0 : data( coord );
        myGrid->template setVoxel< typename base::VolumeGrid< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::IntensitySelector >( coord, intensity );
    }
    NormalsComponent::computeNormals();
}


template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
base::VolumeGrid< SegmentIntensityVolumeType, SegmentNormalsVolumeType >&
    VolumeGridHelper< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::grid() const
{
    return *myGrid;
}


template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
base::Node* VolumeGridHelper< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::createNode
    ( unsigned int geometryType, const Spacing& spacing, const Extent& extent ) const
{
    /* Compute the extent of a regular grid segment, taking the redundant texels into account. Regular segments have
     * `regularPartitionSize + 1` texels per dimension which is due to the redundant pixels.
     */
    const base::math::Vector3f regularSegmentExtent = extent.units.cwiseMin( base::math::Vector3f
        ( spacing.units.x() * partitioningX.regularPartitionSize
        , spacing.units.y() * partitioningY.regularPartitionSize
        , spacing.units.z() * partitioningZ.regularPartitionSize ) );
    
    /* Create pivot node that centers its children.
     *
     * Suppose the following example grid layout, where p is the absolute center and
     * q is the center of the first segment:
     *
     *     o-------o-------o---o
     *     |   q   |       |   |
     *     o-------o-------o---o
     *     |       | p     |   |
     *     o-------o-------o---o
     *     |       |       |   |
     *     o-------o-------o---o
     *
     * Each segment is rendered around its local center. Thus, the translation T
     * required to center the volume geometry in the origin, corresponds to:
     *
     *     T = -p + q
     */
    base::Node* const pivot = new base::Node();
    pivot->localTransform = base::math::translation4f( ( regularSegmentExtent - extent.units ) / 2 );
    pivot->setMovable( false );

    /* Create geometry nodes for all grid segments.
     */
    LIBCARNA_FOR_VECTOR3UI( segmentCoord, myGrid->segmentCounts )
    {
        const base::VolumeSegment< SegmentIntensityVolumeType, SegmentNormalsVolumeType >& segment = myGrid->segmentAt( segmentCoord );

        /* Compute extent of particular grid segment.
         */
        const bool isTailX = segmentCoord.x() + 1 == myGrid->segmentCounts.x();
        const bool isTailY = segmentCoord.y() + 1 == myGrid->segmentCounts.y();
        const bool isTailZ = segmentCoord.z() + 1 == myGrid->segmentCounts.z();
        const base::math::Vector3ui& volumeSize = segment.intensities().size; // includes redundant pixels (only along non-tail axes)
        const base::math::Vector3f segmentExtent
            ( isTailX ? ( volumeSize.x() - 1 ) * spacing.units.x() : regularSegmentExtent.x()
            , isTailY ? ( volumeSize.y() - 1 ) * spacing.units.y() : regularSegmentExtent.y()
            , isTailZ ? ( volumeSize.z() - 1 ) * spacing.units.z() : regularSegmentExtent.z() );

        /* Create geometry node for particular grid segment.
         */
        base::Geometry* const geom = new base::Geometry( geometryType );
        pivot->attachChild( geom );
        IntensityComponent::attachTexture( *geom, segment );
        NormalsComponent  ::attachTexture( *geom, segment );
        geom->setMovable( false );
        geom->setBoundingVolume( new base::BoundingBox( 1, 1, 1 ) );
        geom->localTransform
            = base::math::translation4f
                ( segmentCoord.x() * regularSegmentExtent.x() - ( isTailX ? ( regularSegmentExtent.x() - segmentExtent.x() ) / 2 : 0 )
                , segmentCoord.y() * regularSegmentExtent.y() - ( isTailY ? ( regularSegmentExtent.y() - segmentExtent.y() ) / 2 : 0 )
                , segmentCoord.z() * regularSegmentExtent.z() - ( isTailZ ? ( regularSegmentExtent.z() - segmentExtent.z() ) / 2 : 0 ) )
            * base::math::scaling4f( segmentExtent );
    }

    /* We're done.
     */
    base::Log::instance().record( base::Log::debug
        , "VolumeGridHelper computed "
        + base::text::lexical_cast< std::string >( 8 * sizeof( typename SegmentIntensityVolumeType::Voxel ) )
        + "bit grid data using "
        + base::text::lexical_cast< std::string >( myGrid->segmentCounts.x() ) + "×"
        + base::text::lexical_cast< std::string >( myGrid->segmentCounts.y() ) + "×"
        + base::text::lexical_cast< std::string >( myGrid->segmentCounts.z() ) + " segments" );
    return pivot;
}


template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
base::Node* VolumeGridHelper< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::createNode
    ( unsigned int geometryType, const Spacing& spacing ) const
{
    const base::math::Vector3f extent = (
        nativeResolution.cast< int >() - base::math::Vector3i( 1, 1, 1 )
    ).cast< float >().cwiseProduct( spacing.units );
    return createNode( geometryType, spacing, Extent( extent ) );
}


template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
base::Node* VolumeGridHelper< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::createNode
    ( unsigned int geometryType, const Extent& extent ) const
{
    const base::math::Vector3f spacing = extent.units.cast< float >().cwiseQuotient(
        ( nativeResolution.cast< int >() - base::math::Vector3i( 1, 1, 1 ) ).cast< float >()
    );
    return createNode( geometryType, Spacing( spacing ), extent );
}


template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
base::math::Vector3ui VolumeGridHelper< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::gridResolution() const
{
    return resolution;
}



}  // namespace LibCarna :: helpers

}  // namespace LibCarna

#endif // VOLUMEGRIDHELPER_H_6014714286
