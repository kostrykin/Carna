/*
 *  Copyright (C) 2010 - 2016 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 * 
 * 
 *  Copyright (C) 2021 - 2025 Leonid Kostrykin
 * 
 */

#ifndef HUVOLUME_H_6014714286
#define HUVOLUME_H_6014714286

/** \file   HUVolume.h
  *
  * \brief  Defines \ref LibCarna::base::HUVolume.
  *
  * \author Leonid Kostrykin
  */

#include <LibCarna/base/math/VectorField.h>
#include <LibCarna/base/noncopyable.h>
#include <LibCarna/base/HUV.h>

namespace LibCarna
{

namespace base
{



// ----------------------------------------------------------------------------------
// HUVolume
// ----------------------------------------------------------------------------------

/** \brief
  * Defines interface to \f$\mathbb N_0^3 \to \left[-1024,3071\right]\f$ volumetric
  * data.
  *
  * \author Leonid Kostrykin
  */
class LIBCARNA HUVolume : public math::VectorField< HUV >
{

    NON_COPYABLE

public:

    /** \brief
      * Instantiates.
      */
    HUVolume();

    /** \brief
      * Instantiates.
      */
    explicit HUVolume( const math::Vector3ui& size );
    
    /** \brief
      * Holds the resolution.
      */
    math::Vector3ui size;

    /** \brief
      * Returns HUV of specified voxel.
      */
    virtual HUV operator()( unsigned int x
                          , unsigned int y
                          , unsigned int z ) const override = 0;

    /** \overload
      */
    virtual HUV operator()( const math::Vector3ui& at ) const override = 0;

}; // HUVolume



}  // namespace LibCarna :: base

}  // namespace LibCarna

#endif // HUVOLUME_H_6014714286
