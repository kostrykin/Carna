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

template< typename Vector, typename Scalar = Vector::Scalar >
class RayPlaneHitTest
{

    bool myHitExists;
    Vector myHitLocation;

public:

    RayPlaneHitTest();

    void compute( const Vector& rayOrigin, const Vector& rayDirection, const Vector& planeNormal, Scalar planeOriginOffset );

    bool hitExists() const;

    const Vector& hitLocation() const;

}; // RayPlaneHitTest


template< typename Vector, typename Scalar >
RayPlaneHitTest< Vector, Scalar >::RayPlaneHitTest()
    : myHitExists( false )
{
}


template< typename Vector, typename Scalar >
bool RayPlaneHitTest< Vector, Scalar >::hitExists() const
{
    return myHitExists;
}


template< typename Vector, typename Scalar >
const Vector& RayPlaneHitTest< Vector, Scalar >::hitLocation() const
{
    CARNA_ASSERT( hitExists() );
    return myHitLocation;
}


template< typename Vector, typename Scalar >
void RayPlaneHitTest< Vector, Scalar >::compute
    ( const Vector& rayOrigin
    , const Vector& rayDirection
    , const Vector& planeNormal
    , Scalar planeOriginOffset )
{
    CARNA_ASSERT( math::isEqual< Scalar >( rayDirection.norm(), 1 ) );
    CARNA_ASSERT( math::isEqual< Scalar >(  planeNormal.norm(), 1 ) );
    CARNA_ASSERT( rayDirection.rows() == rayOrigin.rows() && rayOrigin.rows() == planeNormal.rows() );
    CARNA_ASSERT( rayDirection.cols() == rayOrigin.cols() && rayOrigin.cols() == planeNormal.cols() && planeNormal.cols() == 1 );

    if( math::isEqual< Scalar >( rayDirection.dot( planeNormal ), 0 ) )
    {
        myHitExists = false;
    }
    else
    {
        for( unsigned int i = 0; i < static_cast< unsigned int >( planeNormal.rows() ); ++i )
        {
            if( !math::isEqual< Scalar >( 0, rayDirection( i ) ) && !math::isEqual< Scalar >( 0, planeNormal( i ) ) )
            {
                const Scalar rayLength = ( planeOriginOffset / planeNormal( i ) - rayOrigin( i ) ) / rayDirection( i );
                if( rayLength < 0 )
                {
                    myHitExists = false;
                }
                else
                {
                    myHitExists = true;
                    myHitLocation = rayOrigin + rayDirection * rayLength;
                }
                break;
            }
        }
    }
}



}  // namespace Carna :: base

}  // namespace Carna

#endif // RAYPLANEHITTEST_H_6014714286
