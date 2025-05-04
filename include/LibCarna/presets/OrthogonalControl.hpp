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

#ifndef ORTHOGONALCONTROL_H_6014714286
#define ORTHOGONALCONTROL_H_6014714286

#include <LibCarna/LibCarna.hpp>
#include <LibCarna/base/ProjectionControl.hpp>
#include <LibCarna/base/CameraControl.hpp>

/** \file   OrthogonalControl.h
  * \brief  Defines \ref LibCarna::presets::OrthogonalControl.
  */

namespace LibCarna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// OrthogonalControl
// ----------------------------------------------------------------------------------

/** \brief
  * Controls orthogonal projection matrices, that induce box-shaped visible volume,
  * and \ref base::Camera objects that use such projections.
  *
  * The \ref base::CameraControl interface delegates rotations and lateral movement
  * to an arbitrary implementation that is supplied to the constructor, but overrides
  * the \ref moveAxially "axial movement" s.t. it is realized as zooming through
  * adjustment of the projection matrix. 
  *
  * \author Leonid Kostrykin
  */
class LIBCARNA OrthogonalControl : public base::ProjectionControl, public base::CameraControl
{

    NON_COPYABLE
    
    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    /** \brief
      * Default \f$\frac{\text{zoom level}}{\text{moved units}}\f$ factor.
      */
    const static float DEFAULT_ZOOM_STRENGTH;

    /** \brief
      * Instantiates.
      * \pre `cc != nullptr`
      */
    explicit OrthogonalControl( base::CameraControl* cc );
    
    /** \brief
      * Deletes.
      */
    virtual ~OrthogonalControl();
    
    /** \brief
      * Sets the \f$\frac{\text{zoom level}}{\text{moved units}}\f$ factor.
      */
    void setZoomStrength( float zoomStrength );
    
    /** \brief
      * Tells the \f$\frac{\text{zoom level}}{\text{moved units}}\f$ factor.
      */
    float zoomStrength() const;
    
    /** \brief
      * Enables and disables \ref rotateHorizontally and \ref rotateVertically.
      */
    void setRotationEnabled( bool isRotationEnabled );
    
    /** \brief
      * Tells whether \ref rotateHorizontally and \ref rotateVertically are enabled.
      */
    bool isRotationEnabled() const;
    
    /** \brief
      * Tells current zoom factor.
      */
    float zoomFactor() const;
    
    /** \brief
      * Creates \a orthogonalProjection matrix using \ref base::math::ortho4f.
      */
    virtual void updateProjection( base::math::Matrix4f& orthogonalProjection ) const override;

    /** \brief
      * Sets the \a camera that this object should control. Also sets the
      * \ref base::Camera::setOrthogonalProjectionHint "orthogonal projection hint"
      * if \a camera is from \ref base::Camera class.
      */
    virtual void setCamera( base::Spatial& camera ) override;
    
    /** \brief
      * Delegates to the \ref base::CameraControl implementation that was supplied to
      * the constructor.
      */
    virtual void rotateHorizontally( float radians ) override;
    
    /** \brief
      * Delegates to the \ref base::CameraControl implementation that was supplied to
      * the constructor.
      */
    virtual void rotateVertically( float radians ) override;
    
    /** \brief
      * Updates the result \ref updateProjection produces.
      */
    virtual void moveAxially( float units ) override;
    
    virtual void moveLaterally( float unitsX, float unitsY ) override;

}; // OrthogonalControl



}  // namespace LibCarna :: presets

}  // namespace LibCarna



#endif // ORTHOGONALCONTROL_H_6014714286
