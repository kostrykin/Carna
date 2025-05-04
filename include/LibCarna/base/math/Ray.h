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

#ifndef RAY_H_6014714286
#define RAY_H_6014714286

#include <LibCarna/LibCarna.h>
#include <LibCarna/base/math.h>

/** \file   Ray.h
  * \brief  Defines \ref Carna::base::math::Ray.
  */

namespace Carna
{

namespace base
{

namespace math
{



// ----------------------------------------------------------------------------------
// Ray
// ----------------------------------------------------------------------------------

/** \brief
  * Defines ray in \f$D^d\f$ where \f$D\f$ is the `Scalar` type derived from
  * \a VectorType and \f$d\f$ is dimension of \a VectorType.
  *
  * \param VectorType Specifies the which vector type is to use.
  * 
  * \author Leonid Kostrykin
  */
template< typename VectorType >
class Ray
{

public:

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    /** \brief
      * Reflects the `VectorType` argument this class template was instantiated with.
      */
    typedef VectorType Vector;

    /** \brief
      * Holds the origin of this ray.
      */
    VectorType origin;

    /** \brief
      * Holds the direction of this ray.
      */
    VectorType direction;

}; // Ray



// ----------------------------------------------------------------------------------
// Ray3f
// ----------------------------------------------------------------------------------

/** \brief
  * Defines ray in \f$\mathbb R^3\f$.
  * 
  * \author Leonid Kostrykin
  */
class LIBCARNA Ray3f : public Ray< Vector3f >
{

public:

    /** \brief
      * Computes a ray in world space.
      */
    void fromEye
        ( unsigned int frameX, unsigned int frameY
        , const Viewport& vp
        , const Matrix4f& inverseProjection
        , const Matrix4f& inverseView );

    /** \brief
      * Computes a ray in world space.
      */
    void fromEye
        ( unsigned int frameX, unsigned int frameY
        , const Viewport& vp
        , const Camera& cam );

}; // Ray3f



}  // namespace Carna :: base :: math

}  // namespace Carna :: base

}  // namespace Carna

#endif // RAY_H_6014714286
