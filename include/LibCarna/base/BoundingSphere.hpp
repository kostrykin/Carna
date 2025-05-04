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

#ifndef BOUNDINGSPHERE_H_6014714286
#define BOUNDINGSPHERE_H_6014714286

#include <LibCarna/LibCarna.hpp>
#include <LibCarna/base/BoundingVolume.hpp>

/** \file
  * \brief
  * Defines \ref LibCarna::base::BoundingSphere.
  */

namespace LibCarna
{

namespace base
{



// ----------------------------------------------------------------------------------
// BoundingSphere
// ----------------------------------------------------------------------------------

/** \brief
  * Defines a \ref Geometry minimal boundary sphere.
  *
  * \author Leonid Kostrykin
  */
class LIBCARNA BoundingSphere : public BoundingVolume
{

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    /** \brief
      * Instantiates with \a radius.
      * \pre `radius > 0`
      */
    BoundingSphere( float radius );

    /** \brief
      * Deletes.
      */
    virtual ~BoundingSphere();

    /** \brief
      * Tells the radius of this bounding sphere.
      * \post `radius() > 0`
      */
    float radius() const;

    /** \brief
      * Sets the radius of this bounding sphere.
      * \pre `radius > 0`
      */
    void setRadius( float radius );

    virtual void computeClosemostPoint( math::Vector3f& out, const math::Vector3f& reference ) const override;

}; // BoundingSphere



}  // namespace LibCarna :: base

}  // namespace LibCarna

#endif // BOUNDINGSPHERE_H_6014714286
