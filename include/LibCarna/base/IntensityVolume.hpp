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

#ifndef INTENSITYVOLUME_H_6014714286
#define INTENSITYVOLUME_H_6014714286

/** \file   IntensityVolume.h
  *
  * \brief  Defines \ref LibCarna::base::IntensityVolume.
  *
  * \author Leonid Kostrykin
  */

#include <LibCarna/base/math/VectorField.hpp>
#include <LibCarna/base/noncopyable.hpp>

namespace LibCarna
{

namespace base
{



// ----------------------------------------------------------------------------------
// IntensityVolume
// ----------------------------------------------------------------------------------

/** \brief
  * Defines interface to \f$\mathbb N_0^3 \to \left[0,1\right]\f$ volume data.
  *
  * \author Leonid Kostrykin
  * \since  \ref v_3_2_0
  */
class LIBCARNA IntensityVolume : public math::VectorField< float >
{

    NON_COPYABLE

public:

    /** \brief
      * Instantiates.
      */
    IntensityVolume();

    /** \brief
      * Instantiates.
      */
    explicit IntensityVolume( const math::Vector3ui& size );
    
    /** \brief
      * Holds the resolution.
      */
    math::Vector3ui size;

    /** \brief
      * Returns intensity of specified voxel.
      */
    virtual float operator()( unsigned int x
                            , unsigned int y
                            , unsigned int z ) const override = 0;

    /** \overload
      */
    virtual float operator()( const math::Vector3ui& at ) const override = 0;

}; // IntensityVolume



}  // namespace LibCarna :: base

}  // namespace LibCarna

#endif // INTENSITYVOLUME_H_6014714286
