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

#ifndef HUCUBEFIELD_H_6014714286
#define HUCUBEFIELD_H_6014714286

/** \file   HUCubeField.h
  *
  * \brief  Defines \ref Carna::base::model::HUCubeField.
  *
  * \author Leonid Kostrykin
  * \date   22.2.2015
  */

#include <Carna/base/model/HUVolume.h>
#include <Carna/base/model/HUCubeSegment.h>

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// HUCubeField
// ----------------------------------------------------------------------------------

template< typename HUCubeSegmentVolume >
class HUCubeField : public HUVolume
{

    NON_COPYABLE

public:

    typedef typename HUCubeSegmentVolume HUCubeSegmentVolume;

    typedef HUCubeSegment< HUCubeField< HUCubeSegmentVolume >, HUCubeSegmentVolume > HUCubeSegment;

    HUCubeField
        ( const math::Vector3ui& maxSegmentSize
        , const math::Vector3ui& segmentCounts
        , const std::function< Association< HUCubeSegmentVolume >*
            ( unsigned int segmentX
            , unsigned int segmentY
            , unsigned int segmentZ ) >& segmentVolumeAt );

    virtual ~HUCubeField();

    const math::Vector3ui maxSegmentSize;
    const math::Vector3ui segmentCounts;

    const HUCubeSegment& segmentAt( unsigned int segmentX, unsigned int segmentY, unsigned int segmentZ ) const;
    
    virtual signed short operator()( unsigned int x
                                   , unsigned int y
                                   , unsigned int z ) const override;

    virtual signed short operator()( const math::Vector3ui& at ) const override;

private:

    std::vector< HUCubeSegment* > segments;

    std::size_t segmentIndex( unsigned int segmentX, unsigned int segmentY, unsigned int segmentZ ) const;

}; // HUCubeField


template< typename HUCubeSegmentVolume >
HUCubeField< HUCubeSegmentVolume >::HUCubeField
    ( const math::Vector3ui& maxSegmentSize
    , const math::Vector3ui& segmentCounts
    , const std::function< Association< HUCubeSegmentVolume >*
        ( unsigned int segmentX
        , unsigned int segmentY
        , unsigned int segmentZ ) >& segmentVolumeAt )

    : maxSegmentSize( maxSegmentSize )
    , segmentCounts( segmentCounts )
{
    for( unsigned int z = 0; z < segmentCounts.z; ++z )
    for( unsigned int y = 0; y < segmentCounts.y; ++y )
    for( unsigned int x = 0; x < segmentCounts.x; ++x )
    {
        const std::size_t index = segmentIndex( x, y, z );
        Association< HUCubeSegmentVolume >* const segmentVolume = segmentVolumeAt( x, y, z );
        HUCubeSegment* const segment = new HUCubeSegment( *this );
        segment->setVolume( segmentVolume );
        segment->offset = math::Vector3ui( x * maxSegmentSize.x, y * maxSegmentSize.y, z * maxSegmentSize.z );
        segments[ index ] = segment;

    }
}


template< typename HUCubeSegmentVolume >
HUCubeField< HUCubeSegmentVolume >::~HUCubeField()
{
    std::for_each( segments.begin(), segments.end(), std::default_delete< HUCubeSegment >() );
}


template< typename HUCubeSegmentVolume >
std::size_t HUCubeField< HUCubeSegmentVolume >::segmentIndex( unsigned int segmentX, unsigned int segmentY, unsigned int segmentZ ) const
{
    return segmentX + segmentY * segmentCounts.x + segmentZ * segmentCounts.x * segmentCounts.y;
}


template< typename HUCubeSegmentVolume >
const typename HUCubeField< HUCubeSegmentVolume >::HUCubeSegment& HUCubeField< HUCubeSegmentVolume >::segmentAt
    ( unsigned int segmentX, unsigned int segmentY, unsigned int segmentZ ) const
{
    const std::size_t index = segmentIndex( segmentX, segmentY, segmentZ );
    const HUCubeSegment& segment = *segments[ index ];
    return segment;
}


template< typename HUCubeSegmentVolume >
signed short HUCubeField< HUCubeSegmentVolume >::operator()
    ( unsigned int x
    , unsigned int y
    , unsigned int z ) const
{
    const unsigned int segmentX = x / maxSegmentSize.x;
    const unsigned int segmentY = y / maxSegmentSize.y;
    const unsigned int segmentZ = z / maxSegmentSize.z;

    const unsigned int localX = x % maxSegmentSize.x;
    const unsigned int localY = y % maxSegmentSize.y;
    const unsigned int localZ = z % maxSegmentSize.z;

    const HUCubeSegment& segment = segmentAt( segmentX, segmentY, segmentZ );
    return segment( localX, localY, localZ );
}


template< typename HUCubeSegmentVolume >
signed short HUCubeField< HUCubeSegmentVolume >::operator()( const math::Vector3ui& at ) const
{
    return ( *this )( at.x, at.y, at.z );
}



}  // namesapce Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna

#endif // HUCUBEFIELD_H_6014714286
