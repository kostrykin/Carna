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

#ifndef HUVOLUMESEGMENT_H_6014714286
#define HUVOLUMESEGMENT_H_6014714286

/** \file   HUVolumeSegment.h
  *
  * \brief  Defines \ref Carna::base::HUVolumeSegment.
  *
  * \author Leonid Kostrykin
  * \date   22.2.2015
  */

#include <Carna/base/Association.h>
#include <Carna/base/CarnaException.h>
#include <Carna/base/HUVolumeGrid.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// HUVolumeSegment
// ----------------------------------------------------------------------------------

template< typename HUVolumeGridType, typename HUVolumeSegmentVolumeType >
class HUVolumeSegment
{

    NON_COPYABLE

public:

    typedef HUVolumeGridType HUVolumeGrid;

    typedef HUVolumeSegmentVolumeType HUVolumeSegmentVolume;

    HUVolumeGridType& field;

    HUVolumeSegment( HUVolumeGridType& field );

    void setVolume( Association< HUVolumeSegmentVolumeType >* volume );

    HUVolumeSegmentVolumeType& volume();

    const HUVolumeSegmentVolumeType& volume() const;

    bool hasVolume() const;

    math::Vector3ui offset;

private:

    std::unique_ptr< Association< HUVolumeSegmentVolumeType > > myVolume;

}; // HUVolumeSegment


template< typename HUVolumeGridType, typename HUVolumeSegmentVolumeType >
HUVolumeSegment< HUVolumeGridType, HUVolumeSegmentVolumeType >::HUVolumeSegment( HUVolumeGridType& field )
    : field( field )
{
}


template< typename HUVolumeGridType, typename HUVolumeSegmentVolumeType >
void HUVolumeSegment< HUVolumeGridType, HUVolumeSegmentVolumeType >::setVolume( Association< HUVolumeSegmentVolumeType >* volume )
{
    myVolume.reset( volume );
}


template< typename HUVolumeGridType, typename HUVolumeSegmentVolumeType >
HUVolumeSegmentVolumeType& HUVolumeSegment< HUVolumeGridType, HUVolumeSegmentVolumeType >::volume()
{
    CARNA_ASSERT( hasVolume() );
    return **myVolume;
}


template< typename HUVolumeGridType, typename HUVolumeSegmentVolumeType >
const HUVolumeSegmentVolumeType& HUVolumeSegment< HUVolumeGridType, HUVolumeSegmentVolumeType >::volume() const
{
    CARNA_ASSERT( hasVolume() );
    return **myVolume;
}


template< typename HUVolumeGridType, typename HUVolumeSegmentVolumeType >
bool HUVolumeSegment< HUVolumeGridType, HUVolumeSegmentVolumeType >::hasVolume() const
{
    return myVolume.get() != nullptr && myVolume->get() != nullptr;
}



}  // namespace Carna :: base

}  // namespace Carna

#endif // HUVOLUMESEGMENT_H_6014714286
