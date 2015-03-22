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
#include <Carna/base/noncopyable.h>
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

/** \brief
  * Represents a \ref Geometry minimal boundary volume of simple geometric shape. The
  * volume is centered within its local coordinate system.
  *
  * \author Leonid Kostrykin
  * \date   22.3.15
  */
class CARNA_LIB BoundingVolume
{

    NON_COPYABLE

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    /** \brief
      * Instantiates.
      */
    BoundingVolume();

    /** \brief
      * Deletes.
      */
    virtual ~BoundingVolume();
    
    /** \brief
      * Computes the point \a out in model space that is close-most to \a reference,
      * that is also in model space.
      */
    virtual void computeClosemostPoint( math::Vector3f& out, const math::Vector3f& reference ) const = 0;
    
    /** \brief
      * Sets the transform from the local coordinate system of this bounding volume
      * to the \ref Geometry object that uses it.
      */
    void setTransform( const math::Matrix4f& transform );

    /** \brief
      * Tells the transform from the local coordinate system of this bounding volume
      * to the model space of the \ref Geometry object that uses it.
      */
    const math::Matrix4f& transform() const;

    /** \brief
      * Tells the transform from the model space of the \ref Geometry object that
      * uses this bounding volume to its local coordinate system.
      */
    const math::Matrix4f& inverseTransform() const;

}; // BoundingVolume



}  // namespace Carna :: base

}  // namespace Carna

#endif // BOUNDINGVOLUME_H_6014714286
