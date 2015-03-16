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

#ifndef RAYPLANEHITTEST_H_6014714286
#define RAYPLANEHITTEST_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/math.h>
#include <Carna/base/Ray.h>
#include <Carna/base/CarnaException.h>

/** \file   RayPlaneHitTest.h
  * \brief  Defines \ref Carna::base::RayPlaneHitTest.
  */

namespace Carna
{

namespace base
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
  * \date   22.2.15 - 16.3.15
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
    CARNA_ASSERT( hitExists() );
    return myHitLocation;
}


template< typename VectorType, typename ScalarType >
void RayPlaneHitTest< VectorType, ScalarType >::compute
    ( const Ray< VectorType >& ray
    , const VectorType& planeNormal
    , ScalarType planeOriginOffset )
{
    CARNA_ASSERT( math::isEqual< ScalarType >( ray.direction.norm(), 1 ) );
    CARNA_ASSERT( math::isEqual< ScalarType >(  planeNormal.norm(), 1 ) );
    CARNA_ASSERT( ray.direction.rows() == ray.origin.rows() && ray.origin.rows() == planeNormal.rows() );
    CARNA_ASSERT( ray.direction.cols() == ray.origin.cols() && ray.origin.cols() == planeNormal.cols() && planeNormal.cols() == 1 );

    if( math::isEqual< ScalarType >( ray.direction.dot( planeNormal ), 0 ) )
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
            CARNA_ASSERT( math::isEqual( planeNormal.dot( myHitLocation ), planeOriginOffset ) );
        }
    }
}



}  // namespace Carna :: base

}  // namespace Carna

#endif // RAYPLANEHITTEST_H_6014714286
