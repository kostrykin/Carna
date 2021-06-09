/*
 *  Copyright (C) 2021 Leonid Kostrykin
 *
 */

#ifndef INTENSITYVOLUME_H_6014714286
#define INTENSITYVOLUME_H_6014714286

/** \file   IntensityVolume.h
  *
  * \brief  Defines \ref Carna::base::IntensityVolume.
  *
  * \author Leonid Kostrykin
  * \date   June 2021
  */

#include <Carna/base/math/VectorField.h>
#include <Carna/base/noncopyable.h>

namespace Carna
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
  * \date   June 2021
  * \since  \ref v_3_2_0
  */
class CARNA_LIB IntensityVolume : public math::VectorField< float >
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



}  // namespace Carna :: base

}  // namespace Carna

#endif // INTENSITYVOLUME_H_6014714286
