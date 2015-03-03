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

#ifndef RAYBOXHITTEST_H_6014714286
#define RAYBOXHITTEST_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/math.h>

/** \file   RayBoxHitTest.h
  * \brief  Defines \ref Carna::base::RayBoxHitTest.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// RayBoxHitTest
// ----------------------------------------------------------------------------------

class RayBoxHitTest
{

    bool myHitExists;
    math::Vector3f myHitLocation;

public:

    RayBoxHitTest();

    /* Performs the hit test. The ray must be specified in the box' space. It assumed
     * that the origin of this space is the center of the box.
     */
    void compute
        ( const math::Vector3f& rayDirection
        , const math::Vector3f& rayOrigin
        , const math::Vector3f& boxSize = math::Vector3f( 1, 1, 1 ) );

    bool hitExists() const;

    const math::Vector3f& hitLocation() const;

}; // RayBoxHitTest



}  // namespace Carna :: base

}  // namespace Carna

#endif // RAYBOXHITTEST_H_6014714286
