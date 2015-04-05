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

#ifndef ORTHOGONALCONTROL_H_6014714286
#define ORTHOGONALCONTROL_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/ProjectionControl.h>
#include <Carna/base/CameraControl.h>

/** \file   OrthogonalControl.h
  * \brief  Defines \ref Carna::presets::OrthogonalControl.
  */

namespace Carna
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
  * \date   5.4.15
  */
class CARNA_LIB OrthogonalControl : public base::ProjectionControl, public base::CameraControl
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
      * Creates \a orthogonalProjection matrix using \ref base::math::ortho4f.
      */
    virtual void updateProjection( base::math::Matrix4f& orthogonalProjection ) const override;

    virtual void setCamera( base::Spatial& ) override;
    
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



}  // namespace Carna :: presets

}  // namespace Carna



#endif // ORTHOGONALCONTROL_H_6014714286

