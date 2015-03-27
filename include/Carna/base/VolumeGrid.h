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

#include <Carna/base/HUVolume.h>
#include <Carna/base/VolumeSegment.h>
#include <Carna/base/CarnaException.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >
// ----------------------------------------------------------------------------------

template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
class VolumeGrid
{

    NON_COPYABLE

public:

    typedef SegmentHUVolumeType SegmentHUVolume;

    typedef VolumeSegment< SegmentHUVolumeType, SegmentNormalsVolumeType > Segment;

    VolumeGrid( const math::Vector3ui& maxSegmentSize, const math::Vector3ui& segmentCounts );

    virtual ~VolumeGrid();

    const math::Vector3ui maxSegmentSize;
    const math::Vector3ui segmentCounts;

    Segment& segmentAt( const base::math::Vector3ui& );

    const Segment& segmentAt( const base::math::Vector3ui& ) const;

    Segment& segmentAt( unsigned int segmentX, unsigned int segmentY, unsigned int segmentZ );

    const Segment& segmentAt( unsigned int segmentX, unsigned int segmentY, unsigned int segmentZ ) const;

    struct HUVSelector
    {
        typedef typename SegmentHUVolumeType::Value VoxelType;
        static SegmentHUVolumeType& volume( Segment& segment );
        static const SegmentHUVolumeType& volume( const Segment& segment );
    };

    struct NormalSelector
    {
        typedef typename SegmentNormalsVolumeType::Value VoxelType;
        static SegmentNormalsVolumeType& volume( Segment& segment );
        static const SegmentNormalsVolumeType& volume( const Segment& segment );
    };

    template< typename Selector >
    typename Selector::VoxelType getVoxel( unsigned int x, unsigned int y, unsigned int z );

    template< typename Selector >
    typename Selector::VoxelType getVoxel( const math::Vector3ui& at );

    template< typename Selector >
    void setVoxel( unsigned int x, unsigned int y, unsigned int z, const typename Selector::VoxelType& voxel );

    template< typename Selector >
    void setVoxel( const math::Vector3ui& at, const typename Selector::VoxelType& voxel );

private:

    std::vector< Segment* > segments;

    std::size_t segmentIndex( unsigned int segmentX, unsigned int segmentY, unsigned int segmentZ ) const;

}; // VolumeGrid


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >::VolumeGrid
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


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >::~VolumeGrid()
{
    std::for_each( segments.begin(), segments.end(), std::default_delete< Segment >() );
}


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
std::size_t VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >::segmentIndex
    ( unsigned int segmentX
    , unsigned int segmentY
    , unsigned int segmentZ ) const
{
    return segmentX + segmentY * segmentCounts.x() + segmentZ * segmentCounts.x() * segmentCounts.y();
}


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
typename VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >::Segment&
    VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >::segmentAt
        ( const base::math::Vector3ui& p )
{
    return segmentAt( p.x(), p.y(), p.z() );
}


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
const typename VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >::Segment&
    VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >::segmentAt
        ( const base::math::Vector3ui& p ) const
{
    return segmentAt( p.x(), p.y(), p.z() );
}


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
typename VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >::Segment&
    VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >::segmentAt
        ( unsigned int segmentX
        , unsigned int segmentY
        , unsigned int segmentZ )
{
    const std::size_t index = segmentIndex( segmentX, segmentY, segmentZ );
    Segment& segment = *segments[ index ];
    return segment;
}


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
const typename VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >::Segment&
    VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >::segmentAt
        ( unsigned int segmentX
        , unsigned int segmentY
        , unsigned int segmentZ ) const
{
    const std::size_t index = segmentIndex( segmentX, segmentY, segmentZ );
    const Segment& segment = *segments[ index ];
    return segment;
}


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
template< typename Selector >
typename Selector::VoxelType VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >::getVoxel
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


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
template< typename Selector >
typename Selector::VoxelType VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >::getVoxel
    ( const math::Vector3ui& at )
{
    return getVoxel< Selector >( at.x(), at.y(), at.z() );
}


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
template< typename Selector >
void VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >::setVoxel
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


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
template< typename Selector >
void VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >::setVoxel
    ( const math::Vector3ui& at, const typename Selector::VoxelType& voxel )
{
    setVoxel< Selector >( at.x(), at.y(), at.z(), voxel );
}



// ----------------------------------------------------------------------------------
// VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType > :: HUVSelector
// ----------------------------------------------------------------------------------

template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
SegmentHUVolumeType& VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >::HUVSelector::volume
    ( VolumeSegment< SegmentHUVolumeType, SegmentNormalsVolumeType >& segment )
{
    return segment.huVolume();
}


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
const SegmentHUVolumeType& VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >::HUVSelector::volume
    ( const VolumeSegment< SegmentHUVolumeType, SegmentNormalsVolumeType >& segment )
{
    return segment.huVolume();
}



// ----------------------------------------------------------------------------------
// VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType > :: NormalSelector
// ----------------------------------------------------------------------------------

template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
SegmentNormalsVolumeType& VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >::NormalSelector::volume
    ( VolumeSegment< SegmentHUVolumeType, SegmentNormalsVolumeType >& segment )
{
    return segment.normals();
}


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
const SegmentNormalsVolumeType& VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >::NormalSelector::volume
    ( const VolumeSegment< SegmentHUVolumeType, SegmentNormalsVolumeType >& segment )
{
    return segment.normals();
}



}  // namespace Carna :: base

}  // namespace Carna

#endif // VOLUMEGRID_H_6014714286
