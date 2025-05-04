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

#ifndef BOUNDINGVOLUME_H_6014714286
#define BOUNDINGVOLUME_H_6014714286

#include <LibCarna/LibCarna.hpp>
#include <LibCarna/base/noncopyable.hpp>
#include <LibCarna/base/math.hpp>

/** \file   BoundingVolume.h
  * \brief  Defines \ref LibCarna::base::BoundingVolume.
  */

namespace LibCarna
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
  */
class LIBCARNA BoundingVolume
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



}  // namespace LibCarna :: base

}  // namespace LibCarna

#endif // BOUNDINGVOLUME_H_6014714286
