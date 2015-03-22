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

#ifndef SCALARFIELD_H_6014714286
#define SCALARFIELD_H_6014714286

/** \file   ScalarField.h
  *
  * \brief  Defines \ref Carna::base::math::ScalarField.
  *
  * \author Leonid Kostrykin
  * \date   21.2.13
  */

#include <Carna/base/math.h>
#include <Carna/Carna.h>

namespace Carna
{

namespace base
{

namespace math
{



// ----------------------------------------------------------------------------------
// ScalarField
// ----------------------------------------------------------------------------------

/** \brief
  * Abstract definition of an \f$\mathbb Z_0^3 \to \mathrm T \subset \mathbb R\f$
  * scalar field.
  *
  * \author Leonid Kostrykin
  * \date   21.2.13 - 9.3.15
  */
template< typename ScalarType >
class ScalarField
{

public:

    /** \brief
      * Holds the co-domain type of the scalar field.
      */
    typedef ScalarType Scalar;

    /** \brief
      * Does nothing.
      */
    virtual ~ScalarField()
    {
    }

    /** \brief
      * Returns value of specified voxel.
      */
    virtual ScalarType operator()
        ( unsigned int x
        , unsigned int y
        , unsigned int z ) const = 0;

    /** \brief
      * Returns value of specified voxel.
      */
    virtual ScalarType operator()( const Vector3ui& at ) const;

}; // ScalarField


template< typename ScalarType >
ScalarType ScalarField< ScalarType >::operator()( const math::Vector3ui& at ) const
{
    return ( *this )( at.x(), at.y(), at.z() );
}



}  // namespace Carna :: base :: math

}  // namespace Carna :: base

}  // namespace Carna

#endif // SCALARFIELD_H_6014714286
