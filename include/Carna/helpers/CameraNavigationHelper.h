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

#ifndef CAMERANAVIGATIONHELPER_H_6014714286
#define CAMERANAVIGATIONHELPER_H_6014714286

#include <Carna/Carna.h>

/** \file   CameraNavigationHelper.h
  * \brief  Defines \ref Carna::helpers::CameraNavigationHelper.
  */

namespace Carna
{

namespace helpers
{



// ----------------------------------------------------------------------------------
// CameraNavigationHelper
// ----------------------------------------------------------------------------------

class CameraNavigationHelper
{

    NON_COPYABLE
    
    base::Camera* cam;

public:

    CameraNavigationHelper();

    void setCamera( base::Camera& );
    
    void rotateHorizontally( float radians );
    
    void rotateVertically( float radians );
    
    void moveAxially( float distance );

}; // CameraNavigationHelper



}  // namespace Carna :: helpers

}  // namespace Carna



#endif // CAMERANAVIGATIONHELPER_H_6014714286
