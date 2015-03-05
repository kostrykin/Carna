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

#ifndef RAY_H_6014714286
#define RAY_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/math.h>

/** \file   Ray.h
  * \brief  Defines \ref Carna::base::Ray.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Ray
// ----------------------------------------------------------------------------------

template< typename Vector >
class Ray
{

public:

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    math::Vector3f origin;
    math::Vector3f direction;

}; // Ray



// ----------------------------------------------------------------------------------
// Ray3f
// ----------------------------------------------------------------------------------

class CARNA_LIB Ray3f : public Ray< math::Vector3f >
{

public:

    /** \brief
      * Computes a ray in world space.
      */
    void fromEye
        ( unsigned int frameX, unsigned int frameY
        , const Viewport& vp
        , const math::Matrix4f& inverseProjection
        , const math::Matrix4f& inverseView );

    /** \brief
      * Computes a ray in world space.
      */
    void fromEye
        ( unsigned int frameX, unsigned int frameY
        , const Viewport& vp
        , const Camera& cam );

}; // Ray3f



}  // namespace Carna :: base

}  // namespace Carna

#endif // RAY_H_6014714286
