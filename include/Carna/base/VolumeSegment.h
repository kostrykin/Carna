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

#include <Carna/Carna.h>
#include <Carna/base/Association.h>
#include <Carna/base/CarnaException.h>
#include <Carna/base/VolumeGrid.h>
#include <memory>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// VolumeSegmentNormalsComponent< SegmentNormalsVolumeType >
// ----------------------------------------------------------------------------------

/** \brief
  * Represents the normal map component of an \ref VolumeSegment object.
  *
  * \author Leonid Kostrykin
  * \date   8.3.15 - 29.3.15
  */
template< typename SegmentNormalsVolumeType >
class VolumeSegmentNormalsComponent
{

    NON_COPYABLE

    std::unique_ptr< Association< SegmentNormalsVolumeType > > myNormals;

public:
    
    /** \brief
      * Deletes.
      */
    virtual ~VolumeSegmentNormalsComponent();
    
    /** \brief
      * Sets the normal map of this partition.
      */
    void setNormals( Association< SegmentNormalsVolumeType >* volume );

    /** \brief
      * References the normal map of this partition.
      * \pre `hasNormals() == true`
      */
    SegmentNormalsVolumeType& normals();

    /** \overload
      */
    const SegmentNormalsVolumeType& normals() const;
    
    /** \brief
      * Tells whether this partition has a normal map associated.
      */
    bool hasNormals() const;

}; // VolumeSegmentNormalsComponent


template< typename SegmentNormalsVolumeType >
VolumeSegmentNormalsComponent< SegmentNormalsVolumeType >::~VolumeSegmentNormalsComponent()
{
}


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
// VolumeSegmentNormalsComponent< void >
// ----------------------------------------------------------------------------------

/** \brief
  * Specializes \ref VolumeSegmentNormalsComponent when no normal maps are desired.
  *
  * \author Leonid Kostrykin
  * \date   8.3.15 - 29.3.15
  */
template< >
class VolumeSegmentNormalsComponent< void >
{

public:

    /** \brief
      * Deletes.
      */
    virtual ~VolumeSegmentNormalsComponent();

}; // VolumeSegmentNormalsComponent


inline VolumeSegmentNormalsComponent< void >::~VolumeSegmentNormalsComponent()
{
}



// ----------------------------------------------------------------------------------
// VolumeSegment
// ----------------------------------------------------------------------------------

/** \brief
  * Represents a single \ref VolumePartitioning "volumetric data partition".
  *
  * \author Leonid Kostrykin
  * \date   8.3.15 - 29.3.15
  */
template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
class VolumeSegment : public VolumeSegmentNormalsComponent< SegmentNormalsVolumeType >
{

    NON_COPYABLE

public:
    
    /** \brief
      * Reflects the type to use for representation of the whole volumetric data
      * partitioning.
      */
    typedef VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType > Grid;
    
    /** \brief
      * Reflects the type to use for storing the HU volume of a single partition.
      */
    typedef SegmentHUVolumeType HUVolume;
    
    /** \brief
      * Reflects the type to use for storing the normal map of a single partition.
      */
    typedef SegmentNormalsVolumeType NormalsVolume;

    /** \brief
      * References the \ref VolumePartitioning "volumetric data partitioning" this
      * partition belongs to.
      */
    Grid& grid;

    /** \brief
      * Instantiates.
      *
      * \param grid references the
      *     \ref VolumePartitioning "volumetric data partitioning" this partition
      *     belongs to.
      */
    VolumeSegment( Grid& grid );

    /** \brief
      * Sets the HU volume data of this partition.
      */
    void setHUVolume( Association< SegmentHUVolumeType >* huVolume );

    /** \brief
      * References the HU volume data of this partition.
      * \pre `hasHUVolume() == true`
      */
    SegmentHUVolumeType& huVolume();

    /** \overload
      */
    const SegmentHUVolumeType& huVolume() const;

    /** \brief
      * Tells whether this partition has HU volume data associated.
      */
    bool hasHUVolume() const;

    /** \brief
      * Holds the coordinate offset this partition within the \ref grid "whole"
      * volumetric data partitioning.
      */
    math::Vector3ui offset;

private:

    std::unique_ptr< Association< SegmentHUVolumeType > > myHUVolume;

}; // VolumeSegment


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
VolumeSegment< SegmentHUVolumeType, SegmentNormalsVolumeType >::VolumeSegment
        ( VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >& grid )
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
