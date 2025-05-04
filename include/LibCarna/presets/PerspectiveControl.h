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

#ifndef PERSPECTIVECONTROL_H_6014714286
#define PERSPECTIVECONTROL_H_6014714286

#include <LibCarna/LibCarna.h>
#include <LibCarna/base/ProjectionControl.h>

/** \file   PerspectiveControl.h
  * \brief  Defines \ref LibCarna::presets::PerspectiveControl.
  */

namespace LibCarna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// PerspectiveControl
// ----------------------------------------------------------------------------------

/** \brief
  * Controls projection matrices that induce frustum-shaped visible volume.
  *
  * \author Leonid Kostrykin
  */
class LIBCARNA PerspectiveControl : public base::ProjectionControl
{

    NON_COPYABLE
    
    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    /** \brief
      * Holds the default *half* angle betwen the left and the right frustum planes.
      */
    const static float DEFAULT_FOV_HORIZONTAL;

    /** \brief
      * Instantiates.
      */
    PerspectiveControl();
    
    /** \brief
      * Deletes.
      */
    virtual ~PerspectiveControl();
    
    /** \brief
      * Creates \a perspectiveProjection matrix using \ref base::math::frustum4f.
      */
    virtual void updateProjection( base::math::Matrix4f& perspectiveProjection ) const override;
    
    /** \brief
      * Sets the *half* angle between the left and the right frustum planes.
      * \pre `radians > 0`
      */
    void setFovHorizontal( float radians );
    
    /** \brief
      * Tells the *half* angle between the left and the right frustum planes.
      */
    float fovHorizontal() const;

}; // PerspectiveControl



}  // namespace LibCarna :: presets

}  // namespace LibCarna



#endif // PERSPECTIVECONTROL_H_6014714286
