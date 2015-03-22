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

#ifndef BOUNDINGBOX_H_6014714286
#define BOUNDINGBOX_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/BoundingVolume.h>

/** \file   BoundingBox.h
  * \brief  Defines \ref Carna::base::BoundingBox.
  */

namespace Carna
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
  * \date   22.3.15
  */
class CARNA_LIB BoundingBox : public BoundingVolume
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



}  // namespace Carna :: base

}  // namespace Carna

#endif // BOUNDINGBOX_H_6014714286
