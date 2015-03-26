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
// VolumeGrid
// ----------------------------------------------------------------------------------

template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
class VolumeGrid : public HUVolume
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
    
    virtual HUV operator()( unsigned int x, unsigned int y, unsigned int z ) const override;

    virtual HUV operator()( const math::Vector3ui& at ) const override;

    void setVoxel( const math::Vector3ui& at, HUV );

    void setVoxel( unsigned int x, unsigned int y, unsigned int z, HUV );

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
HUV VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >::operator()( unsigned int x, unsigned int y, unsigned int z ) const
{
    const unsigned int segmentX = x / maxSegmentSize.x();
    const unsigned int segmentY = y / maxSegmentSize.y();
    const unsigned int segmentZ = z / maxSegmentSize.z();

    const unsigned int localX = x % maxSegmentSize.x();
    const unsigned int localY = y % maxSegmentSize.y();
    const unsigned int localZ = z % maxSegmentSize.z();

    const Segment& segment = segmentAt( segmentX, segmentY, segmentZ );
    return segment.volume()( localX, localY, localZ );
}


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
HUV VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >::operator()( const math::Vector3ui& at ) const
{
    return ( *this )( at.x(), at.y(), at.z() );
}


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
void VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >::setVoxel( unsigned int x, unsigned int y, unsigned int z, HUV huv )
{
    const unsigned int segmentX = x / maxSegmentSize.x();
    const unsigned int segmentY = y / maxSegmentSize.y();
    const unsigned int segmentZ = z / maxSegmentSize.z();

    const unsigned int localX = x % maxSegmentSize.x();
    const unsigned int localY = y % maxSegmentSize.y();
    const unsigned int localZ = z % maxSegmentSize.z();

    Segment& segment = segmentAt( segmentX, segmentY, segmentZ );
    segment.volume().setVoxel( localX, localY, localZ, huv );

    /* Note that segments are not disjoint,
     * so we might need to update the redundant texels as well.
     */
    const bool updateRedundantX = localX == 0 && segmentX > 0;
    const bool updateRedundantY = localY == 0 && segmentY > 0;
    const bool updateRedundantZ = localZ == 0 && segmentZ > 0;

    if( updateRedundantX )
    {
        segmentAt( segmentX - 1, segmentY, segmentZ ).volume().setVoxel( maxSegmentSize.x(), localY, localZ, huv );
    }
    if( updateRedundantY )
    {
        segmentAt( segmentX, segmentY - 1, segmentZ ).volume().setVoxel( localX, maxSegmentSize.y(), localZ, huv );
    }
    if( updateRedundantZ )
    {
        segmentAt( segmentX, segmentY, segmentZ - 1 ).volume().setVoxel( localX, localY, maxSegmentSize.z(), huv );
    }

    if( updateRedundantX && updateRedundantY )
    {
        segmentAt( segmentX - 1, segmentY - 1, segmentZ ).volume().setVoxel( maxSegmentSize.x(), maxSegmentSize.y(), localZ, huv );
    }
    if( updateRedundantX && updateRedundantZ )
    {
        segmentAt( segmentX - 1, segmentY, segmentZ - 1 ).volume().setVoxel( maxSegmentSize.x(), localY, maxSegmentSize.z(), huv );
    }
    if( updateRedundantY && updateRedundantZ )
    {
        segmentAt( segmentX, segmentY - 1, segmentZ - 1 ).volume().setVoxel( localX, maxSegmentSize.y(), maxSegmentSize.z(), huv );
    }

    if( updateRedundantX && updateRedundantY && updateRedundantZ )
    {
        segmentAt( segmentX - 1, segmentY - 1, segmentZ - 1 ).volume().setVoxel( maxSegmentSize, huv );
    }
}


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
void VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >::setVoxel( const math::Vector3ui& at, HUV huv )
{
    this->setVoxel( at.x(), at.y(), at.z(), huv );
}



}  // namespace Carna :: base

}  // namespace Carna

#endif // VOLUMEGRID_H_6014714286
