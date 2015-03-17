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

#ifndef CAMERANAVIGATIONCONTROL_H_6014714286
#define CAMERANAVIGATIONCONTROL_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/CameraControl.h>
#include <memory>

/** \file   CameraNavigationControl.h
  * \brief  Defines \ref Carna::presets::CameraNavigationControl.
  */

namespace Carna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// CameraNavigationControl
// ----------------------------------------------------------------------------------
    
/** \brief
  * Implements \ref base::CameraControl, rotates the camera around itself and allows
  * completely free camera movement.
  *
  * \author Leonid Kostrykin
  * \date   22.2.15 - 17.3.15
  */
class CARNA_LIB CameraNavigationControl : public base::CameraControl
{

    NON_COPYABLE
    
    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    /** \brief
      * Instantiates.
      */
    CameraNavigationControl();
    
    /** \brief
      * Deletes.
      */
    virtual ~CameraNavigationControl();

    virtual void setCamera( base::Spatial& ) override;
    
    /** \brief
      * Rotates the camera around its local \f$\left(0, 1, 0\right)^\mathrm T\f$
      * vector by \a radians.
      */
    virtual void rotateHorizontally( float radians ) override;
    
    /** \brief
      * Rotates the camera around its local \f$\left(1, 0, 0\right)^\mathrm T\f$
      * vector by \a radians.
      */
    virtual void rotateVertically( float radians ) override;
    
    /** \brief
      * Moves the camera along its local \f$\left(0, 0, 1\right)^\mathrm T\f$ vector
      * by \a units.
      */
    virtual void moveAxially( float units ) override;

}; // CameraNavigationControl



}  // namespace Carna :: presets

}  // namespace Carna



#endif // CAMERANAVIGATIONCONTROL_H_6014714286
