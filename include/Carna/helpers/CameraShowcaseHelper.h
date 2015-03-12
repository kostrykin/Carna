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

#ifndef CAMERASHOWCASEHELPER_H_6014714286
#define CAMERASHOWCASEHELPER_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/math.h>

/** \file   CameraShowcaseHelper.h
  * \brief  Defines \ref Carna::helpers::CameraShowcaseHelper.
  */

namespace Carna
{

namespace helpers
{



// ----------------------------------------------------------------------------------
// CameraShowcaseHelper
// ----------------------------------------------------------------------------------

class CameraShowcaseHelper
{

    NON_COPYABLE
    
    base::Camera* cam;
    
    base::Spatial* exhibit;
    
    base::Matrix4f rotation;
    
    float distance;

public:

    CameraShowcaseHelper();

    void setExhibit( base::Spatial& );

    void setCamera( base::Camera& );
    
    void rotateHorizontally( float radians );
    
    void rotateVertically( float radians );
    
    void moveAxially( float distance );

}; // CameraShowcaseHelper



}  // namespace Carna :: helpers

}  // namespace Carna



#endif // CAMERASHOWCASEHELPER_H_6014714286
