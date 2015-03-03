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

#include <Carna/base/BoundingVolume.h>
#include <Carna/base/math.h>

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

class BoundingBox : public BoundingVolume
{

    math::Vector3f myBoxSize;

public:

    BoundingBox( const math::Vector3f& boxSize );

    void setBoxSize( const math::Vector3f& boxSize );

    const math::Vector3f& boxSize() const;

    virtual bool testHit
        ( const math::Vector3f& rayOrigin
        , const math::Vector3f& rayDirection
        , math::Vector3f& hitLocation ) const override;

}; // BoundingBox



}  // namespace Carna :: base

}  // namespace Carna

#endif // BOUNDINGBOX_H_6014714286
