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

#ifndef RAYSPHEREHITTEST_H_6014714286
#define RAYSPHEREHITTEST_H_6014714286

#include <LibCarna/LibCarna.h>
#include <LibCarna/base/math.h>
#include <LibCarna/base/math/Ray.h>
#include <LibCarna/base/CarnaException.h>

/** \file   RaySphereHitTest.h
  * \brief  Defines \ref Carna::base::math::RaySphereHitTest.
  */

namespace Carna
{

namespace base
{

namespace math
{



// ----------------------------------------------------------------------------------
// RaySphereHitTest
// ----------------------------------------------------------------------------------

/** \brief
  * Tests whether sphere in origin is hit by a \ref Ray object.
  *
  * \param VectorType defines the vector type to use for computation.
  * \param ScalarType defines the scalar type to use for computation.
  *
  * \author Leonid Kostrykin
  * \date   22.3.15
  */
template< typename VectorType, typename ScalarType >
class RaySphereHitTest
{

    bool myHitExists;
    VectorType myHitLocation;

public:

    /** \brief
      * Instantiates.
      *
      * \post `hitExists() == false`
      */
    RaySphereHitTest();

    /** \brief
      * Performs a hit test of \a ray with a sphere of radius \a radius. The sphere
      * is assumed to be centered in the origin. Use \ref hitExists and
      * \ref hitLocation to obtain the test results.
      */
    void compute( const Ray< VectorType >& ray, const ScalarType radius );

    /** \brief
      * Tells whether a hit exists.
      */
    bool hitExists() const;

    /** \brief
      * References the location of the hit.
      *
      * \pre `hitExists() == true`
      */
    const VectorType& hitLocation() const;

}; // RaySphereHitTest


template< typename VectorType, typename ScalarType >
RaySphereHitTest< VectorType, ScalarType >::RaySphereHitTest()
    : myHitExists( false )
{
}


template< typename VectorType, typename ScalarType >
bool RaySphereHitTest< VectorType, ScalarType >::hitExists() const
{
    return myHitExists;
}


template< typename VectorType, typename ScalarType >
const VectorType& RaySphereHitTest< VectorType, ScalarType >::hitLocation() const
{
    LIBCARNA_ASSERT( hitExists() );
    return myHitLocation;
}


template< typename VectorType, typename ScalarType >
void RaySphereHitTest< VectorType, ScalarType >::compute( const Ray< VectorType >& ray, ScalarType radius )
{
    LIBCARNA_ASSERT( isEqual< ScalarType >( ray.direction.norm(), 1 ) );
    LIBCARNA_ASSERT( isEqual< ScalarType >(  planeNormal.norm(), 1 ) );
    LIBCARNA_ASSERT( ray.direction.rows() == ray.origin.rows() && ray.origin.rows() == planeNormal.rows() );
    LIBCARNA_ASSERT( ray.direction.cols() == ray.origin.cols() && ray.origin.cols() == planeNormal.cols() && planeNormal.cols() == 1 );

    ;
}



}  // namespace Carna :: base :: math

}  // namespace Carna :: base

}  // namespace Carna

#endif // RAYSPHEREHITTEST_H_6014714286
