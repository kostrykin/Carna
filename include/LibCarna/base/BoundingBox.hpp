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

#ifndef BOUNDINGBOX_H_6014714286
#define BOUNDINGBOX_H_6014714286

#include <LibCarna/LibCarna.hpp>
#include <LibCarna/base/BoundingVolume.hpp>

/** \file
  * \brief
  * Defines \ref LibCarna::base::BoundingBox.
  */

namespace LibCarna
{

namespace base
{



// ----------------------------------------------------------------------------------
// BoundingBox
// ----------------------------------------------------------------------------------

/** \brief
  * Defines a \ref Geometry minimal boundary box.
  *
  * \author Leonid Kostrykin
  */
class LIBCARNA BoundingBox : public BoundingVolume
{

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    /** \brief
      * Instantiates bounding box of \a size.
      * \pre `size.x() > 0 && size.y() > 0 && size.z() > 0`
      */
    BoundingBox( const math::Vector3f& size );

    /** \overload
      */
    BoundingBox( float width, float height, float depth );

    /** \brief
      * Deletes
      */
    virtual ~BoundingBox();

    /** \brief
      * Tells the size of this bounding box.
      * \post `size.x() > 0 && size.y() > 0 && size.z() > 0`
      */
    const math::Vector3f& size() const;

    /** \brief
      * Changes the \a size of this bounding box.
      * \pre `size.x() > 0 && size.y() > 0 && size.z() > 0`
      */
    void setSize( const math::Vector3f& size );

    /** \overload
      */
    void setSize( float width, float height, float depth );

    virtual void computeClosemostPoint( math::Vector3f& out, const math::Vector3f& reference ) const override;

}; // BoundingBox



}  // namespace LibCarna :: base

}  // namespace LibCarna

#endif // BOUNDINGBOX_H_6014714286
