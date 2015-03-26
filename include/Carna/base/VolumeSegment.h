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

#ifndef VOLUMESEGMENT_H_6014714286
#define VOLUMESEGMENT_H_6014714286

/** \file   VolumeSegment.h
  *
  * \brief  Defines \ref Carna::base::VolumeSegment.
  *
  * \author Leonid Kostrykin
  * \date   22.2.15 - 26.3.15
  */

#include <Carna/base/Association.h>
#include <Carna/base/CarnaException.h>
#include <Carna/base/VolumeGrid.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// VolumeSegmentNormalsComponent
// ----------------------------------------------------------------------------------

template< typename SegmentNormalsVolumeType >
class VolumeSegmentNormalsComponent
{

    std::unique_ptr< Association< SegmentNormalsVolumeType > > myNormals;

public:

    void setNormals( Association< SegmentNormalsVolumeType >* volume );

    SegmentNormalsVolumeType& normals();

    const SegmentNormalsVolumeType& normals() const;

    bool hasNormals() const;

}; // VolumeSegmentNormalsComponent


template< >
class VolumeSegmentNormalsComponent< void >
{
};


template< typename SegmentNormalsVolumeType >
void VolumeSegmentNormalsComponent< SegmentNormalsVolumeType >::setNormals( Association< SegmentNormalsVolumeType >* normals )
{
    myNormals.reset( normals );
}


template< typename SegmentNormalsVolumeType >
SegmentNormalsVolumeType& VolumeSegmentNormalsComponent< SegmentNormalsVolumeType >::normals()
{
    CARNA_ASSERT( hasNormals() );
    return **myNormals;
}


template< typename SegmentNormalsVolumeType >
const SegmentNormalsVolumeType& VolumeSegmentNormalsComponent< SegmentNormalsVolumeType >::normals() const
{
    CARNA_ASSERT( hasNormals() );
    return **myNormals;
}


template< typename SegmentNormalsVolumeType >
bool VolumeSegmentNormalsComponent< SegmentNormalsVolumeType >::hasNormals() const
{
    return myNormals.get() != nullptr && myNormals->get() != nullptr;
}



// ----------------------------------------------------------------------------------
// VolumeSegment
// ----------------------------------------------------------------------------------

template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
class VolumeSegment
{

    NON_COPYABLE

public:

    typedef VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType > Grid;

    typedef SegmentHUVolumeType HUVolume;
    
    typedef SegmentNormalsVolumeType NormalsVolume;

    Grid& grid;

    VolumeSegment( Grid& grid );

    void setHUVolume( Association< SegmentHUVolumeType >* huVolume );

    SegmentHUVolumeType& huVolume();

    const SegmentHUVolumeType& huVolume() const;

    bool hasHUVolume() const;

    math::Vector3ui offset;

private:

    std::unique_ptr< Association< SegmentHUVolumeType > > myHUVolume;

}; // VolumeSegment


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
VolumeSegment< SegmentHUVolumeType, SegmentNormalsVolumeType >::VolumeSegment
        ( VolumeSegment< SegmentHUVolumeType, SegmentNormalsVolumeType >::Grid& grid )
    : grid( grid )
{
}


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
void VolumeSegment< SegmentHUVolumeType, SegmentNormalsVolumeType >::setHUVolume( Association< SegmentHUVolumeType >* huVolume )
{
    myHUVolume.reset( huVolume );
}


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
SegmentHUVolumeType& VolumeSegment< SegmentHUVolumeType, SegmentNormalsVolumeType >::huVolume()
{
    CARNA_ASSERT( hasHUVolume() );
    return **myHUVolume;
}


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
const SegmentHUVolumeType& VolumeSegment< SegmentHUVolumeType, SegmentNormalsVolumeType >::huVolume() const
{
    CARNA_ASSERT( hasHUVolume() );
    return **myHUVolume;
}


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
bool VolumeSegment< SegmentHUVolumeType, SegmentNormalsVolumeType >::hasHUVolume() const
{
    return myHUVolume.get() != nullptr && myHUVolume->get() != nullptr;
}



}  // namespace Carna :: base

}  // namespace Carna

#endif // VOLUMESEGMENT_H_6014714286
