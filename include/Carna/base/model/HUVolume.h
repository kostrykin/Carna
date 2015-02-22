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
  * \brief  Defines \ref Carna::base::model::Volume.
  *
  * \author Leonid Kostrykin
  * \date   25.7.11
  */

#include <Carna/base/model/ScalarField.h>
#include <Carna/base/noncopyable.h>

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// HUVolume
// ----------------------------------------------------------------------------------

/** \brief  Abstract definition of \f$\mathbb N_0^3 \to [-1024,3071]\f$ volumetric data.
  *
  * \author Leonid Kostrykin
  * \date   25.7.11
  */
class CARNA_LIB HUVolume : public ScalarField< signed short >
{

    NON_COPYABLE

public:

    /** \brief  Instantiates.
      */
    HUVolume();

    /** \brief  Instantiates.
      */
    explicit HUVolume( const Vector3ui& size );
    

    /** \brief  Holds the dimensions.
      */
    Vector3ui size;

    
    /** \brief  Returns HUV of specified voxel.
      */
    virtual signed short operator()( unsigned int x
                                   , unsigned int y
                                   , unsigned int z ) const override = 0;

    /** \brief  Returns HUV of specified voxel.
      */
    virtual signed short operator()( const Vector3ui& at ) const override = 0;

}; // HUVolume



}  // namesapce Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna

#endif // HUVOLUME_H_6014714286
