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

#ifndef HUVOLUMEGRID_H_6014714286
#define HUVOLUMEGRID_H_6014714286

/** \file   HUVolumeGrid.h
  *
  * \brief  Defines \ref Carna::base::HUVolumeGrid.
  *
  * \author Leonid Kostrykin
  * \date   22.2.2015
  */

#include <Carna/base/HUVolume.h>
#include <Carna/base/HUVolumeSegment.h>
#include <Carna/base/CarnaException.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// HUVolumeGrid
// ----------------------------------------------------------------------------------

template< typename HUVolumeSegmentVolume >
class HUVolumeGrid : public HUVolume
{

    NON_COPYABLE

public:

    typedef typename HUVolumeSegmentVolume HUVolumeSegmentVolume;

    typedef HUVolumeSegment< HUVolumeGrid< HUVolumeSegmentVolume >, HUVolumeSegmentVolume > HUVolumeSegment;

    HUVolumeGrid( const math::Vector3ui& maxSegmentSize, const math::Vector3ui& segmentCounts );

    virtual ~HUVolumeGrid();

    const math::Vector3ui maxSegmentSize;
    const math::Vector3ui segmentCounts;

    HUVolumeSegment& segmentAt( unsigned int segmentX, unsigned int segmentY, unsigned int segmentZ );

    const HUVolumeSegment& segmentAt( unsigned int segmentX, unsigned int segmentY, unsigned int segmentZ ) const;
    
    virtual HUV operator()( unsigned int x, unsigned int y, unsigned int z ) const override;

    virtual HUV operator()( const math::Vector3ui& at ) const override;

    void setVoxel( const math::Vector3ui& at, HUV );

    void setVoxel( unsigned int x, unsigned int y, unsigned int z, HUV );

private:

    std::vector< HUVolumeSegment* > segments;

    std::size_t segmentIndex( unsigned int segmentX, unsigned int segmentY, unsigned int segmentZ ) const;

}; // HUVolumeGrid


template< typename HUVolumeSegmentVolume >
HUVolumeGrid< HUVolumeSegmentVolume >::HUVolumeGrid( const math::Vector3ui& maxSegmentSize, const math::Vector3ui& segmentCounts )
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
        HUVolumeSegment* const segment = new HUVolumeSegment( *this );
        segment->offset = math::Vector3ui( x * maxSegmentSize.x(), y * maxSegmentSize.y(), z * maxSegmentSize.z() );
        segments[ index ] = segment;
    }
}


template< typename HUVolumeSegmentVolume >
HUVolumeGrid< HUVolumeSegmentVolume >::~HUVolumeGrid()
{
    std::for_each( segments.begin(), segments.end(), std::default_delete< HUVolumeSegment >() );
}


template< typename HUVolumeSegmentVolume >
std::size_t HUVolumeGrid< HUVolumeSegmentVolume >::segmentIndex( unsigned int segmentX, unsigned int segmentY, unsigned int segmentZ ) const
{
    return segmentX + segmentY * segmentCounts.x() + segmentZ * segmentCounts.x() * segmentCounts.y();
}


template< typename HUVolumeSegmentVolume >
typename HUVolumeGrid< HUVolumeSegmentVolume >::HUVolumeSegment& HUVolumeGrid< HUVolumeSegmentVolume >::segmentAt
    ( unsigned int segmentX, unsigned int segmentY, unsigned int segmentZ )
{
    const std::size_t index = segmentIndex( segmentX, segmentY, segmentZ );
    HUVolumeSegment& segment = *segments[ index ];
    return segment;
}


template< typename HUVolumeSegmentVolume >
const typename HUVolumeGrid< HUVolumeSegmentVolume >::HUVolumeSegment& HUVolumeGrid< HUVolumeSegmentVolume >::segmentAt
    ( unsigned int segmentX, unsigned int segmentY, unsigned int segmentZ ) const
{
    const std::size_t index = segmentIndex( segmentX, segmentY, segmentZ );
    const HUVolumeSegment& segment = *segments[ index ];
    return segment;
}


template< typename HUVolumeSegmentVolume >
HUV HUVolumeGrid< HUVolumeSegmentVolume >::operator()( unsigned int x, unsigned int y, unsigned int z ) const
{
    const unsigned int segmentX = x / maxSegmentSize.x();
    const unsigned int segmentY = y / maxSegmentSize.y();
    const unsigned int segmentZ = z / maxSegmentSize.z();

    const unsigned int localX = x % maxSegmentSize.x();
    const unsigned int localY = y % maxSegmentSize.y();
    const unsigned int localZ = z % maxSegmentSize.z();

    const HUVolumeSegment& segment = segmentAt( segmentX, segmentY, segmentZ );
    return segment.volume()( localX, localY, localZ );
}


template< typename HUVolumeSegmentVolume >
HUV HUVolumeGrid< HUVolumeSegmentVolume >::operator()( const math::Vector3ui& at ) const
{
    return ( *this )( at.x(), at.y(), at.z() );
}


template< typename HUVolumeSegmentVolume >
void HUVolumeGrid< HUVolumeSegmentVolume >::setVoxel( unsigned int x, unsigned int y, unsigned int z, HUV huv )
{
    const unsigned int segmentX = x / maxSegmentSize.x();
    const unsigned int segmentY = y / maxSegmentSize.y();
    const unsigned int segmentZ = z / maxSegmentSize.z();

    const unsigned int localX = x % maxSegmentSize.x();
    const unsigned int localY = y % maxSegmentSize.y();
    const unsigned int localZ = z % maxSegmentSize.z();

    const HUVolumeSegment& segment = segmentAt( segmentX, segmentY, segmentZ );
    return segment.volume().setVoxel( localX, localY, localZ, huv );
}


template< typename HUVolumeSegmentVolume >
void HUVolumeGrid< HUVolumeSegmentVolume >::setVoxel( const math::Vector3ui& at, HUV huv )
{
    return this->setVoxel( at.x(), at.y(), at.z(), huv );
}



}  // namespace Carna :: base

}  // namespace Carna

#endif // HUVOLUMEGRID_H_6014714286
