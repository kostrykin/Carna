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

#ifndef BOUNDINGSPHERE_H_6014714286
#define BOUNDINGSPHERE_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/BoundingVolume.h>

/** \file   BoundingSphere.h
  * \brief  Defines \ref Carna::base::BoundingSphere.
  */

namespace Carna
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
  * \date   22.3.15
  */
class CARNA_LIB BoundingSphere : public BoundingVolume
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

    virtual float computeDistance2( const math::Vector3f& point ) const override;

}; // BoundingSphere



}  // namespace Carna :: base

}  // namespace Carna

#endif // BOUNDINGSPHERE_H_6014714286
