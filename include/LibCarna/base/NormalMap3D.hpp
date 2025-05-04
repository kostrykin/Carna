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

#ifndef NORMALMAP3D_H_6014714286
#define NORMALMAP3D_H_6014714286

/** \file   NormalMap3D.h
  *
  * \brief  Defines \ref LibCarna::base::NormalMap3D.
  *
  * \author Leonid Kostrykin
  */

#include <LibCarna/base/math/VectorField.hpp>
#include <LibCarna/base/math.hpp>
#include <LibCarna/base/noncopyable.hpp>

namespace LibCarna
{

namespace base
{



// ----------------------------------------------------------------------------------
// NormalMap3D
// ----------------------------------------------------------------------------------

/** \brief
  * Defines interface to \f$\mathbb N_0^3 \to \left\{\vec v \middle| \left\|\vec v
  * \in \mathbb R^3\right\|_2 \leq 1\right\}\f$ mapping.
  *
  * \author Leonid Kostrykin
  */
class LIBCARNA NormalMap3D : public math::VectorField< math::Vector3f >
{

    NON_COPYABLE

public:

    /** \brief
      * Instantiates.
      */
    NormalMap3D();

    /** \brief
      * Instantiates.
      */
    explicit NormalMap3D( const math::Vector3ui& size );
    
    /** \brief
      * Holds the resolution.
      */
    math::Vector3ui size;

}; // NormalMap3D



}  // namespace LibCarna :: base

}  // namespace LibCarna

#endif // NORMALMAP3D_H_6014714286
