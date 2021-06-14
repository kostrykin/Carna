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

#ifndef VOLUMEGRID_H_6014714286
#define VOLUMEGRID_H_6014714286

/** \file   VolumeGrid.h
  *
  * \brief  Defines \ref Carna::base::VolumeGrid.
  *
  * \author Leonid Kostrykin
  * \date   22.2.15 - 26.3.15
  */

#include <Carna/Carna.h>
#include <Carna/base/IntensityVolume.h>
#include <Carna/base/VolumeSegment.h>
#include <Carna/base/CarnaException.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// VolumeGrid< SegmentIntensityVolumeType, SegmentNormalsVolumeType >
// ----------------------------------------------------------------------------------

/** \brief
  * Represents a particular partitioning of volumetric data.
  *
  * \param SegmentIntensityVolumeType is the \ref base::BufferedIntensityVolume
  *     compatible type to use for storing the Intensity volume of a single partition.
  *
  * \param SegmentNormalsVolumeType is the \ref base::BufferedNormalMap3D compatible
  *     type to use for storing the normal map of a single partition. Set to `void`
  *     if the normal map is not required.
  *
  * \section VolumePartitioning Volume Partitioning
  *
  * Rendering volume data requires us to upload this data to the GPU. Instead of
  * creating a single, occasionally huge 3D texture, it is a better idea to partition
  * the data into smaller volumes. This reduces the probability of out-of-memory
  * exceptions due to memory fragmentation. Such partitioning induces a grid-like
  * structure, that this class represents. Objects from \ref base::VolumeSegment
  * class represent the cells of such grids.
  *
  * \author Leonid Kostrykin
  * \date   8.3.15 - 29.3.15
  */
template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
class VolumeGrid
{

    NON_COPYABLE

public:

    /** \brief
      * Reflects the type to use for storing the intensity volume of a single partition.
      */
    typedef SegmentIntensityVolumeType SegmentIntensityVolume;

    /** \brief
      * Reflects the type to use for storing the normal map of a single partition.
      */
    typedef SegmentNormalsVolumeType SegmentNormalsVolume;

    /** \brief
      * Reflects the data type that represents a single partition.
      */
    typedef VolumeSegment< SegmentIntensityVolumeType, SegmentNormalsVolumeType > Segment;

    /** \brief
      * Instantiates.
      *
      * \param maxSegmentSize is the maximum resolution of a single partition.
      * \param segmentCounts is the number of partitions along each dimension.
      */
    VolumeGrid( const math::Vector3ui& maxSegmentSize, const math::Vector3ui& segmentCounts );

    /** \brief
      * Deletes this and all partitions.
      */
    virtual ~VolumeGrid();

    const math::Vector3ui maxSegmentSize; ///< Holds the maximum resolution of a single partition.
    const math::Vector3ui segmentCounts;  ///< Holds the number of partitions along each dimension.

    /** \brief
      * References the partition at \a location.
      * \pre `location.x() < segmentCounts.x() && location.y() < segmentCounts.y() && location.z() < segmentCounts.z()`
      */
    Segment& segmentAt( const base::math::Vector3ui& location );

    /** \overload
      */
    const Segment& segmentAt( const base::math::Vector3ui& ) const;
    
    /** \overload
      */
    Segment& segmentAt( unsigned int segmentX, unsigned int segmentY, unsigned int segmentZ );
    
    /** \overload
      */
    const Segment& segmentAt( unsigned int segmentX, unsigned int segmentY, unsigned int segmentZ ) const;

    // ------------------------------------------------------------------------------
    // VolumeGrid< SegmentIntensityVolumeType, SegmentNormalsVolumeType > :: IntensitySelector
    // ------------------------------------------------------------------------------

    /** \brief
      * References the intensity volume of a given \ref VolumePartitioning "partition".
      */
    struct IntensitySelector
    {
        /** \brief
          * Reflects the voxel type of the volume this selector references.
          */
        typedef typename SegmentIntensityVolumeType::Value VoxelType;

        /** \brief
          * References the intensity volume of a given \ref VolumePartitioning "partition".
          */
        static SegmentIntensityVolumeType& volume( Segment& segment );

        /** \overload
          */
        static const SegmentIntensityVolumeType& volume( const Segment& segment );
    };
    
    // ------------------------------------------------------------------------------
    // VolumeGrid< SegmentIntensityVolumeType, SegmentNormalsVolumeType > :: NormalSelector
    // ------------------------------------------------------------------------------

    /** \brief
      * References the normal map of a given \ref VolumePartitioning "partition".
      */
    struct NormalSelector
    {
        /** \brief
          * Reflects the voxel type of the volume this selector references.
          */
        typedef typename SegmentNormalsVolumeType::Value VoxelType;

        /** \brief
          * References the normal map of a given \ref VolumePartitioning "partition".
          */
        static SegmentNormalsVolumeType& volume( Segment& segment );

        /** \overload
          */
        static const SegmentNormalsVolumeType& volume( const Segment& segment );
    };

    // ------------------------------------------------------------------------------

    /** \brief
      * Reads the voxel of the volume that the \a Selector selects from the
      * \ref segmentAt "partition at" \a location.
      */
    template< typename Selector >
    typename Selector::VoxelType getVoxel( const math::Vector3ui& location );

    /** \overload
      */
    template< typename Selector >
    typename Selector::VoxelType getVoxel( unsigned int x, unsigned int y, unsigned int z );
    
    /** \brief
      * Writes the \a voxel of the volume that the \a Selector selects from the
      * \ref segmentAt "partition at" \a location.
      */
    template< typename Selector >
    void setVoxel( const math::Vector3ui& location, const typename Selector::VoxelType& voxel );
    
    /** \overload
      */
    template< typename Selector >
    void setVoxel( unsigned int x, unsigned int y, unsigned int z, const typename Selector::VoxelType& voxel );

private:

    std::vector< Segment* > segments;

    std::size_t segmentIndex( unsigned int segmentX, unsigned int segmentY, unsigned int segmentZ ) const;

}; // VolumeGrid


template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
VolumeGrid< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::VolumeGrid
        ( const math::Vector3ui& maxSegmentSize
        , const math::Vector3ui& segmentCounts )
    : maxSegmentSize( maxSegmentSize )
    , segmentCounts( segmentCounts )
{
    CARNA_ASSERT( maxSegmentSize.x() > 0 && maxSegmentSize.y() > 0 && maxSegmentSize.z() > 0 );
    CARNA_ASSERT(  segmentCounts.x() > 0 &&  segmentCounts.y() > 0 &&  segmentCounts.z() > 0 );
    segments.resize( segmentCounts.x() * segmentCounts.y() * segmentCounts.z() );
    for( unsigned int z = 0; z < segmentCounts.z(); ++z )
    for( unsigned int y = 0; y < segmentCounts.y(); ++y )
    for( unsigned int x = 0; x < segmentCounts.x(); ++x )
    {
        const std::size_t index = segmentIndex( x, y, z );
        Segment* const segment = new Segment( *this );
        segment->offset = math::Vector3ui( x * maxSegmentSize.x(), y * maxSegmentSize.y(), z * maxSegmentSize.z() );
        segments[ index ] = segment;
    }
}


template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
VolumeGrid< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::~VolumeGrid()
{
    std::for_each( segments.begin(), segments.end(), std::default_delete< Segment >() );
}


template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
std::size_t VolumeGrid< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::segmentIndex
    ( unsigned int segmentX
    , unsigned int segmentY
    , unsigned int segmentZ ) const
{
    return segmentX + segmentY * segmentCounts.x() + segmentZ * segmentCounts.x() * segmentCounts.y();
}


template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
typename VolumeGrid< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::Segment&
    VolumeGrid< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::segmentAt
        ( const base::math::Vector3ui& p )
{
    return segmentAt( p.x(), p.y(), p.z() );
}


template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
const typename VolumeGrid< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::Segment&
    VolumeGrid< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::segmentAt
        ( const base::math::Vector3ui& p ) const
{
    return segmentAt( p.x(), p.y(), p.z() );
}


template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
typename VolumeGrid< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::Segment&
    VolumeGrid< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::segmentAt
        ( unsigned int segmentX
        , unsigned int segmentY
        , unsigned int segmentZ )
{
    const std::size_t index = segmentIndex( segmentX, segmentY, segmentZ );
    Segment& segment = *segments[ index ];
    return segment;
}


template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
const typename VolumeGrid< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::Segment&
    VolumeGrid< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::segmentAt
        ( unsigned int segmentX
        , unsigned int segmentY
        , unsigned int segmentZ ) const
{
    const std::size_t index = segmentIndex( segmentX, segmentY, segmentZ );
    const Segment& segment = *segments[ index ];
    return segment;
}


template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
template< typename Selector >
typename Selector::VoxelType VolumeGrid< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::getVoxel
    ( unsigned int x, unsigned int y, unsigned int z )
{
    const unsigned int segmentX = x / maxSegmentSize.x();
    const unsigned int segmentY = y / maxSegmentSize.y();
    const unsigned int segmentZ = z / maxSegmentSize.z();

    const unsigned int localX = x % maxSegmentSize.x();
    const unsigned int localY = y % maxSegmentSize.y();
    const unsigned int localZ = z % maxSegmentSize.z();

    const Segment& segment = segmentAt( segmentX, segmentY, segmentZ );
    return Selector::volume( segment )( localX, localY, localZ );
}


template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
template< typename Selector >
typename Selector::VoxelType VolumeGrid< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::getVoxel
    ( const math::Vector3ui& at )
{
    return getVoxel< Selector >( at.x(), at.y(), at.z() );
}


template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
template< typename Selector >
void VolumeGrid< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::setVoxel
    ( unsigned int x, unsigned int y, unsigned int z, const typename Selector::VoxelType& voxel )
{
    const unsigned int segmentX = x / maxSegmentSize.x();
    const unsigned int segmentY = y / maxSegmentSize.y();
    const unsigned int segmentZ = z / maxSegmentSize.z();

    const unsigned int localX = x % maxSegmentSize.x();
    const unsigned int localY = y % maxSegmentSize.y();
    const unsigned int localZ = z % maxSegmentSize.z();

    Segment& segment = segmentAt( segmentX, segmentY, segmentZ );
    Selector::volume( segment ).setVoxel( localX, localY, localZ, voxel );

    /* Note that segments are not disjoint,
     * so we might need to update the redundant texels as well.
     */
    const bool updateRedundantX = localX == 0 && segmentX > 0;
    const bool updateRedundantY = localY == 0 && segmentY > 0;
    const bool updateRedundantZ = localZ == 0 && segmentZ > 0;

    if( updateRedundantX )
    {
        Selector::volume( segmentAt( segmentX - 1, segmentY, segmentZ ) )
            .setVoxel( maxSegmentSize.x(), localY, localZ, voxel );
    }
    if( updateRedundantY )
    {
        Selector::volume( segmentAt( segmentX, segmentY - 1, segmentZ ) )
            .setVoxel( localX, maxSegmentSize.y(), localZ, voxel );
    }
    if( updateRedundantZ )
    {
        Selector::volume( segmentAt( segmentX, segmentY, segmentZ - 1 ) )
            .setVoxel( localX, localY, maxSegmentSize.z(), voxel );
    }

    if( updateRedundantX && updateRedundantY )
    {
        Selector::volume( segmentAt( segmentX - 1, segmentY - 1, segmentZ ) )
            .setVoxel( maxSegmentSize.x(), maxSegmentSize.y(), localZ, voxel );
    }
    if( updateRedundantX && updateRedundantZ )
    {
        Selector::volume( segmentAt( segmentX - 1, segmentY, segmentZ - 1 ) )
            .setVoxel( maxSegmentSize.x(), localY, maxSegmentSize.z(), voxel );
    }
    if( updateRedundantY && updateRedundantZ )
    {
        Selector::volume( segmentAt( segmentX, segmentY - 1, segmentZ - 1 ) )
            .setVoxel( localX, maxSegmentSize.y(), maxSegmentSize.z(), voxel );
    }

    if( updateRedundantX && updateRedundantY && updateRedundantZ )
    {
        Selector::volume( segmentAt( segmentX - 1, segmentY - 1, segmentZ - 1 ) )
            .setVoxel( maxSegmentSize, voxel );
    }
}


template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
template< typename Selector >
void VolumeGrid< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::setVoxel
    ( const math::Vector3ui& at, const typename Selector::VoxelType& voxel )
{
    setVoxel< Selector >( at.x(), at.y(), at.z(), voxel );
}



// ----------------------------------------------------------------------------------
// VolumeGrid< SegmentIntensityVolumeType, SegmentNormalsVolumeType > :: IntensitySelector
// ----------------------------------------------------------------------------------

template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
SegmentIntensityVolumeType& VolumeGrid< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::IntensitySelector::volume
    ( VolumeSegment< SegmentIntensityVolumeType, SegmentNormalsVolumeType >& segment )
{
    return segment.intensities();
}


template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
const SegmentIntensityVolumeType& VolumeGrid< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::IntensitySelector::volume
    ( const VolumeSegment< SegmentIntensityVolumeType, SegmentNormalsVolumeType >& segment )
{
    return segment.intensities();
}



// ----------------------------------------------------------------------------------
// VolumeGrid< SegmentIntensityVolumeType, SegmentNormalsVolumeType > :: NormalSelector
// ----------------------------------------------------------------------------------

template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
SegmentNormalsVolumeType& VolumeGrid< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::NormalSelector::volume
    ( VolumeSegment< SegmentIntensityVolumeType, SegmentNormalsVolumeType >& segment )
{
    return segment.normals();
}


template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
const SegmentNormalsVolumeType& VolumeGrid< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::NormalSelector::volume
    ( const VolumeSegment< SegmentIntensityVolumeType, SegmentNormalsVolumeType >& segment )
{
    return segment.normals();
}



}  // namespace Carna :: base

}  // namespace Carna

#endif // VOLUMEGRID_H_6014714286
