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

#ifndef CAMERACONTROL_H_6014714286
#define CAMERACONTROL_H_6014714286

#include <Carna/Carna.h>

/** \file   CameraControl.h
  * \brief  Defines \ref Carna::helpers::CameraControl.
  */

namespace Carna
{

namespace helpers
{



// ----------------------------------------------------------------------------------
// CameraControl
// ----------------------------------------------------------------------------------

class CARNA_LIB CameraControl
{

    NON_COPYABLE

public:
    
    virtual ~CameraControl();

    virtual void setCamera( base::Spatial& ) = 0;
    
    virtual void rotateHorizontally( float radians ) = 0;
    
    virtual void rotateVertically( float radians ) = 0;
    
    virtual void moveAxially( float distance ) = 0;

}; // CameraControl



}  // namespace Carna :: helpers

}  // namespace Carna



#endif // CAMERACONTROL_H_6014714286
