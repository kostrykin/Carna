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

#ifndef BOUNDINGVOLUME_H_6014714286
#define BOUNDINGVOLUME_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/math.h>

/** \file   BoundingVolume.h
  * \brief  Defines \ref Carna::base::BoundingVolume.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// BoundingVolume
// ----------------------------------------------------------------------------------

class BoundingVolume
{

public:

    virtual ~BoundingVolume();

    virtual bool testHit
        ( const math::Vector3f& rayOrigin
        , const math::Vector3f& rayDirection
        , math::Vector3f& hitLocation ) const = 0;

}; // BoundingVolume



}  // namespace Carna :: base

}  // namespace Carna

#endif // BOUNDINGVOLUME_H_6014714286
