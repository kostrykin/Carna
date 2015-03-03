/*
 *  Copyright (C) 2010 - 2014 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#include <Carna/base/RayBoxHitTest.h>
#include <Carna/base/RayPlaneHitTest.h>
#include <Carna/base/CarnaException.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// RayBoxHitTest
// ----------------------------------------------------------------------------------

RayBoxHitTest::RayBoxHitTest()
    : myHitExists( false )
{
}


void RayBoxHitTest::compute
    ( const math::Vector3f& rayOrigin
    , const math::Vector3f& rayDirection
    , const math::Vector3f& boxSize )
{
    CARNA_ASSERT( math::isEqual< float >( rayDirection.norm(), 1 ) );
    CARNA_ASSERT( boxSize.x() > 0 && boxSize.y() > 0 && boxSize.z() > 0 );

    myHitExists = false;

    /* Lets first test whether the ray intersects a minimal bounding sphere.
     */
    const float dotRayOriginDirection = rayOrigin.dot( rayDirection );
    if( dotRayOriginDirection > 0 )
    {
        /* The ray faces away from the box.
         */
        return;
    }
    const float mbsSquaredRadius = ( boxSize / 2 ).squaredNorm();
    const math::Vector3f closestRayLocation = rayOrigin - rayDirection * dotRayOriginDirection;
    if( closestRayLocation.squaredNorm() > mbsSquaredRadius )
    {
        /* The ray faces towards the box, but doesn't hit it.
         */
        return;
    }

    math::Vector3f boxFaceNormals[ 6 ];
    boxFaceNormals[ 0 ] = math::Vector3f( +1,  0,  0 );
    boxFaceNormals[ 1 ] = math::Vector3f( -1,  0,  0 );
    boxFaceNormals[ 2 ] = math::Vector3f(  0, +1,  0 );
    boxFaceNormals[ 3 ] = math::Vector3f(  0, -1,  0 );
    boxFaceNormals[ 4 ] = math::Vector3f(  0,  0, +1 );
    boxFaceNormals[ 5 ] = math::Vector3f(  0,  0, -1 );

    /* First compute the hit location of the ray with any of the front-facing box planes.
     */
    for( unsigned int boxFaceIndex = 0; boxFaceIndex < 6; ++boxFaceIndex )
    {
        const math::Vector3f& boxFaceNormal = boxFaceNormals[ boxFaceIndex ];
        const float boxFaceOffset = boxSize( boxFaceIndex / 2 ) / 2;

        /* Only allow hits with front-facing planes.
         */
        if( boxFaceNormal.dot( rayDirection ) > 0 )
        {
            continue;
        }

        /* Check whether there is a hit with current box plane.
         */
        RayPlaneHitTest< math::Vector3f > planeHitTest;
        planeHitTest.compute( rayDirection, rayOrigin, boxFaceNormal, boxFaceOffset );
        if( planeHitTest.hitExists() )
        {
            const math::Vector3f& hitCandidate = planeHitTest.hitLocation();

            /* Now that we have found a candidate, we must check whether
             * it lies on the back of all other faces.
             */
            bool candidateRejected = false;
            for( unsigned int boxFaceIndex2 = 0; boxFaceIndex2 < 6; ++boxFaceIndex2 )
            {
                if( boxFaceIndex2 != boxFaceIndex )
                {
                    const math::Vector3f& boxFaceNormal2 = boxFaceNormals[ boxFaceIndex2 ];
                    const float boxFaceOffset2 = boxSize( boxFaceIndex / 2 ) / 2;
                    if( boxFaceNormal2.dot( hitCandidate ) - boxFaceOffset2 > 0 )
                    {
                        /* Reject the candidate.
                         */
                        candidateRejected = true;
                        break;
                    }
                }
            }

            /* The candidate is a hit location if it hasn't been rejected yet.
             */
            myHitExists = !candidateRejected;
            if( !myHitExists )
            {
                myHitLocation = hitCandidate;
                break;
            }
        }
    }
}


bool RayBoxHitTest::hitExists() const
{
    return myHitExists;
}


const math::Vector3f& RayBoxHitTest::hitLocation() const
{
    CARNA_ASSERT( hitExists() );
    return myHitLocation;
}



}  // namespace Carna :: base

}  // namespace Carna
