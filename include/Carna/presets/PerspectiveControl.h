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

#ifndef PERSPECTIVECONTROL_H_6014714286
#define PERSPECTIVECONTROL_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/ProjectionControl.h>

/** \file   PerspectiveControl.h
  * \brief  Defines \ref Carna::helpers::PerspectiveControl.
  */

namespace Carna
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
  * \date   22.2.15 - 16.3.15
  */
class CARNA_LIB PerspectiveControl : public base::ProjectionControl
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
      */
    void setFovHorizontal( float radians );
    
    /** \brief
      * Tells the *half* angle between the left and the right frustum planes.
      */
    float fovHorizontal() const;

}; // PerspectiveControl



}  // namespace Carna :: presets

}  // namespace Carna



#endif // PERSPECTIVECONTROL_H_6014714286

