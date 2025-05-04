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

#ifndef RAYPLANEHITTEST_H_6014714286
#define RAYPLANEHITTEST_H_6014714286

#include <LibCarna/LibCarna.h>
#include <LibCarna/base/math.h>
#include <LibCarna/base/math/Ray.h>
#include <LibCarna/base/LibCarnaException.h>

/** \file   RayPlaneHitTest.h
  * \brief  Defines \ref LibCarna::base::math::RayPlaneHitTest.
  */

namespace LibCarna
{

namespace base
{

namespace math
{



// ----------------------------------------------------------------------------------
// RayPlaneHitTest
// ----------------------------------------------------------------------------------

/** \brief
  * Tests whether particular plane is hit by a \ref Ray object.
  *
  * \param VectorType defines the vector type to use for computation.
  * \param ScalarType defines the scalar type to use for computation.
  *
  * \author Leonid Kostrykin
  */
template< typename VectorType, typename ScalarType >
class RayPlaneHitTest
{

    bool myHitExists;
    VectorType myHitLocation;

public:

    /** \brief
      * Instantiates.
      *
      * \post `hitExists() == false`
      */
    RayPlaneHitTest();

    /** \brief
      * Performs a hit test of \a ray with the plane with \a planeNormal and
      * \a planeOriginOffset. Use \ref hitExists and \ref hitLocation to obtain the
      * test results.
      */
    void compute( const Ray< VectorType >& ray, const VectorType& planeNormal, ScalarType planeOriginOffset );

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

}; // RayPlaneHitTest


template< typename VectorType, typename ScalarType >
RayPlaneHitTest< VectorType, ScalarType >::RayPlaneHitTest()
    : myHitExists( false )
{
}


template< typename VectorType, typename ScalarType >
bool RayPlaneHitTest< VectorType, ScalarType >::hitExists() const
{
    return myHitExists;
}


template< typename VectorType, typename ScalarType >
const VectorType& RayPlaneHitTest< VectorType, ScalarType >::hitLocation() const
{
    LIBCARNA_ASSERT( hitExists() );
    return myHitLocation;
}


template< typename VectorType, typename ScalarType >
void RayPlaneHitTest< VectorType, ScalarType >::compute
    ( const Ray< VectorType >& ray
    , const VectorType& planeNormal
    , ScalarType planeOriginOffset )
{
    LIBCARNA_ASSERT( isEqual< ScalarType >( ray.direction.norm(), 1 ) );
    LIBCARNA_ASSERT( isEqual< ScalarType >(  planeNormal.norm(), 1 ) );
    LIBCARNA_ASSERT( ray.direction.rows() == ray.origin.rows() && ray.origin.rows() == planeNormal.rows() );
    LIBCARNA_ASSERT( ray.direction.cols() == ray.origin.cols() && ray.origin.cols() == planeNormal.cols() && planeNormal.cols() == 1 );

    if( isEqual< ScalarType >( ray.direction.dot( planeNormal ), 0 ) )
    {
        myHitExists = false;
    }
    else
    {
        const ScalarType rayLength = ( planeOriginOffset - ray.origin.dot( planeNormal ) ) / ray.direction.dot( planeNormal );
        if( rayLength < 0 )
        {
            myHitExists = false;
        }
        else
        {
            myHitExists = true;
            myHitLocation = ray.origin + ray.direction * rayLength;
            LIBCARNA_ASSERT( isEqual( planeNormal.dot( myHitLocation ), planeOriginOffset ) );
        }
    }
}



}  // namespace LibCarna :: base :: math

}  // namespace LibCarna :: base

}  // namespace LibCarna

#endif // RAYPLANEHITTEST_H_6014714286
