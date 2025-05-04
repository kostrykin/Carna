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

#ifndef CAMERA_H_6014714286
#define CAMERA_H_6014714286

/** \file   Camera.h
  * \brief  Defines \ref Carna::base::Camera.
  */

#include <LibCarna/base/Spatial.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Camera
// ----------------------------------------------------------------------------------

/** \brief
  * Represents the point-of-view and defines the 3D to 2D projection.
  *
  * \author Leonid Kostrykin
  * \date   22.2.15 - 16.3.15
  */
class LIBCARNA Camera : public Spatial
{

    math::Matrix4f myViewTransform;

    math::Matrix4f myProjection;
    
    bool orthogonalProjectionHint;

public:

    /** \brief
      * Instantiates with \ref setOrthogonalProjectionHint set to `false`.
      */
    Camera();

    /** \brief
      * Sets the matrix that is to be used for mapping \ref ViewSpace "view space" to
      * \ref ClippingCoordinates "clipping coordinates". Perspective projections can
      * be obtained via \ref math::frustum4f.
      */
    void setProjection( const math::Matrix4f& );

    /** \brief
      * References the projection matrix of this camera. The projection matrix maps
      * from \ref ViewSpace "view space" to
      * \ref ClippingCoordinates "clipping coordinates".
      */
    const math::Matrix4f& projection() const;
    
    /** \brief
      * Sets hint whether \ref projection is orthogonal or perspectival.
      */
    void setOrthogonalProjectionHint( bool isOrthogonalProjection );
    
    /** \brief
      * Tells whether \ref projection is likely to be orthogonal or perspectival.
      */
    bool isOrthogonalProjectionHintSet() const;

    /** \brief
      * References the \ref ViewSpace "view matrix", that is the inverse
      * \ref worldTransform matrix.
      */
    const math::Matrix4f& viewTransform() const;

    /** \brief
      * Updates the \ref viewTransform matrix.
      */
    virtual void updateWorldTransform() override;

}; // Camera



}  // namespace Carna :: base

}  // namespace Carna

#endif // CAMERA_H_6014714286
