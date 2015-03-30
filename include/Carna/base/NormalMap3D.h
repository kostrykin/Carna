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

#ifndef NORMALMAP3D_H_6014714286
#define NORMALMAP3D_H_6014714286

/** \file   NormalMap3D.h
  *
  * \brief  Defines \ref Carna::base::NormalMap3D.
  *
  * \author Leonid Kostrykin
  * \date   25.7.11
  */

#include <Carna/base/math/VectorField.h>
#include <Carna/base/math.h>
#include <Carna/base/noncopyable.h>

namespace Carna
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
  * \date   26.3.15
  */
class CARNA_LIB NormalMap3D : public math::VectorField< math::Vector3f >
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



}  // namespace Carna :: base

}  // namespace Carna

#endif // NORMALMAP3D_H_6014714286
