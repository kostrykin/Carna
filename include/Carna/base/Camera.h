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

#ifndef CAMERA_H_6014714286
#define CAMERA_H_6014714286

/** \file   Camera.h
  * \brief  Defines \ref Carna::base::Camera.
  */

#include <Carna/base/Spatial.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Camera
// ----------------------------------------------------------------------------------

class CARNA_LIB Camera : public Spatial
{

    math::Matrix4f myViewTransform;

    math::Matrix4f myProjection;

public:

    void setProjection( const math::Matrix4f& );

    const math::Matrix4f& projection() const;

    const math::Matrix4f& viewTransform() const;

    virtual void updateWorldTransform() override;

}; // Camera



}  // namespace Carna :: base

}  // namespace Carna

#endif // CAMERA_H_6014714286
