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

#ifndef CAMERACONTROL_H_6014714286
#define CAMERACONTROL_H_6014714286

#include <LibCarna/LibCarna.h>

/** \file   CameraControl.h
  * \brief  Defines \ref Carna::base::CameraControl.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// CameraControl
// ----------------------------------------------------------------------------------

/** \brief
  * Defines interface for intuitive \ref Camera control.
  *
  * \author Leonid Kostrykin
  * \date   22.2.15 - 16.3.15
  */
class LIBCARNA CameraControl
{

    NON_COPYABLE

public:
    
    /** \brief
      * Does nothing.
      */
    virtual ~CameraControl();

    /** \brief
      * Sets the \a camera that this object should control.
      */
    virtual void setCamera( Spatial& camera ) = 0;
    
    /** \brief
      * Rotates the \ref setCamera "previously set camera" horizontally. The rotation
      * axis depends on the implementation.
      */
    virtual void rotateHorizontally( float radians ) = 0;
    
    /** \brief
      * Rotates the \ref setCamera "previously set camera" vertically. The rotation
      * axis depends on the implementation.
      */
    virtual void rotateVertically( float radians ) = 0;
    
    /** \brief
      * Moves the camera along its z-axis. The parameters of this movement depend on
      * the implementation.
      */
    virtual void moveAxially( float distance ) = 0;
    
    /** \brief
      * Moves the camera along its x-/y-axis. The parameters of this movement depend
      * on the implementation.
      */
    virtual void moveLaterally( float distanceX, float distanceY ) = 0;

}; // CameraControl



}  // namespace Carna :: base

}  // namespace Carna



#endif // CAMERACONTROL_H_6014714286
