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

#ifndef HUVOLUME_H_6014714286
#define HUVOLUME_H_6014714286

/** \file   HUVolume.h
  *
  * \brief  Defines \ref Carna::base::Volume.
  *
  * \author Leonid Kostrykin
  * \date   25.7.11
  */

#include <Carna/base/ScalarField.h>
#include <Carna/base/noncopyable.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// HUVolume
// ----------------------------------------------------------------------------------

/** \brief  Abstract definition of \f$\mathbb N_0^3 \to [-1024,3071]\f$ volumetric data.
  *
  * \author Leonid Kostrykin
  * \date   25.7.11
  */
class CARNA_LIB HUVolume : public ScalarField< HUV >
{

    NON_COPYABLE

public:

    /** \brief  Instantiates.
      */
    HUVolume();

    /** \brief  Instantiates.
      */
    explicit HUVolume( const math::Vector3ui& size );
    
    /** \brief  Holds the dimensions.
      */
    math::Vector3ui size;

    /** \brief  Returns HUV of specified voxel.
      */
    virtual HUV operator()( unsigned int x
                          , unsigned int y
                          , unsigned int z ) const override = 0;

    /** \brief  Returns HUV of specified voxel.
      */
    virtual HUV operator()( const math::Vector3ui& at ) const override = 0;

}; // HUVolume



}  // namespace Carna :: base

}  // namespace Carna

#endif // HUVOLUME_H_6014714286
