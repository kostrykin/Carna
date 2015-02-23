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

#ifndef HUCUBESEGMENT_H_6014714286
#define HUCUBESEGMENT_H_6014714286

/** \file   HUCubeSegment.h
  *
  * \brief  Defines \ref Carna::base::model::HUCubeSegment.
  *
  * \author Leonid Kostrykin
  * \date   22.2.2015
  */

#include <Carna/base/Association.h>
#include <Carna/base/CarnaException.h>
#include <Carna/base/model/HUCubeField.h>

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// HUCubeSegment
// ----------------------------------------------------------------------------------

template< typename HUCubeField, typename HUCubeSegmentVolume >
class HUCubeSegment
{

    NON_COPYABLE

public:

    typedef typename HUCubeField HUCubeField;

    typedef typename HUCubeSegmentVolume HUCubeSegmentVolume;

    HUCubeField& field;

    HUCubeSegment( HUCubeField& field );

    void setVolume( Association< HUCubeSegmentVolume >* volume );

    HUCubeSegmentVolume& volume();

    const HUCubeSegmentVolume& volume() const;

    Vector3ui offset;

private:

    std::unique_ptr< Association< HUCubeSegmentVolume > > myVolume;

}; // HUCubeSegment


template< typename HUCubeField, typename HUCubeSegmentVolume >
HUCubeSegment< HUCubeField, HUCubeSegmentVolume >::HUCubeSegment( HUCubeField& field )
    : field( field )
{
}


template< typename HUCubeField, typename HUCubeSegmentVolume >
void HUCubeSegment< HUCubeField, HUCubeSegmentVolume >::setVolume( Association< HUCubeSegmentVolume >* volume )
{
    myVolume.reset( volume );
}


template< typename HUCubeField, typename HUCubeSegmentVolume >
HUCubeSegmentVolume& HUCubeSegment< HUCubeField, HUCubeSegmentVolume >::volume()
{
    CARNA_ASSERT( myVolume.get() != nullptr );
    return *myVolume;
}


template< typename HUCubeField, typename HUCubeSegmentVolume >
const HUCubeSegmentVolume& HUCubeSegment< HUCubeField, HUCubeSegmentVolume >::volume() const
{
    CARNA_ASSERT( myVolume.get() != nullptr );
    return *myVolume;
}



}  // namesapce Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna

#endif // HUCUBESEGMENT_H_6014714286
