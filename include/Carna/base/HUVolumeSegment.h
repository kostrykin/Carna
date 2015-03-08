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

template< typename HUVolumeGrid, typename HUVolumeSegmentVolume >
class HUVolumeSegment
{

    NON_COPYABLE

public:

    typedef typename HUVolumeGrid HUVolumeGrid;

    typedef typename HUVolumeSegmentVolume HUVolumeSegmentVolume;

    HUVolumeGrid& field;

    HUVolumeSegment( HUVolumeGrid& field );

    void setVolume( Association< HUVolumeSegmentVolume >* volume );

    HUVolumeSegmentVolume& volume();

    const HUVolumeSegmentVolume& volume() const;

    bool hasVolume() const;

    math::Vector3ui offset;

private:

    std::unique_ptr< Association< HUVolumeSegmentVolume > > myVolume;

}; // HUVolumeSegment


template< typename HUVolumeGrid, typename HUVolumeSegmentVolume >
HUVolumeSegment< HUVolumeGrid, HUVolumeSegmentVolume >::HUVolumeSegment( HUVolumeGrid& field )
    : field( field )
{
}


template< typename HUVolumeGrid, typename HUVolumeSegmentVolume >
void HUVolumeSegment< HUVolumeGrid, HUVolumeSegmentVolume >::setVolume( Association< HUVolumeSegmentVolume >* volume )
{
    myVolume.reset( volume );
}


template< typename HUVolumeGrid, typename HUVolumeSegmentVolume >
HUVolumeSegmentVolume& HUVolumeSegment< HUVolumeGrid, HUVolumeSegmentVolume >::volume()
{
    CARNA_ASSERT( hasVolume() );
    return **myVolume;
}


template< typename HUVolumeGrid, typename HUVolumeSegmentVolume >
const HUVolumeSegmentVolume& HUVolumeSegment< HUVolumeGrid, HUVolumeSegmentVolume >::volume() const
{
    CARNA_ASSERT( hasVolume() );
    return **myVolume;
}


template< typename HUVolumeGrid, typename HUVolumeSegmentVolume >
bool HUVolumeSegment< HUVolumeGrid, HUVolumeSegmentVolume >::hasVolume() const
{
    return myVolume.get() != nullptr && myVolume->get() != nullptr;
}



}  // namespace Carna :: base

}  // namespace Carna

#endif // HUVOLUMESEGMENT_H_6014714286
