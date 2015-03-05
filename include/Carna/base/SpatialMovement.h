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

#ifndef SPATIALMOVEMENT_H_6014714286
#define SPATIALMOVEMENT_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>
#include <Carna/base/math.h>
#include <memory>

/** \file   Ray.h
  * \brief  Defines \ref Carna::base::Ray.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// SpatialMovement
// ----------------------------------------------------------------------------------

class CARNA_LIB SpatialMovement
{

    NON_COPYABLE

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    SpatialMovement
        ( Spatial& movedSpatial
        , unsigned int frameX, unsigned int frameY
        , const Viewport& vp
        , const Camera& cam );

    ~SpatialMovement();

    const Viewport& viewport;
    const Camera& cam;
    const math::Matrix4f inverseProjection;

    bool update( unsigned int frameX, unsigned int frameY );

    bool hasMovedSpatial() const;

    Spatial& movedSpatial() const;

}; // SpatialMovement



}  // namespace Carna :: base

}  // namespace Carna

#endif // SPATIALMOVEMENT_H_6014714286
