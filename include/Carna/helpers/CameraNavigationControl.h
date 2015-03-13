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
#include <Carna/helpers/CameraControl.h>
#include <memory>

/** \file   CameraNavigationControl.h
  * \brief  Defines \ref Carna::helpers::CameraNavigationControl.
  */

namespace Carna
{

namespace helpers
{



// ----------------------------------------------------------------------------------
// CameraNavigationControl
// ----------------------------------------------------------------------------------

class CARNA_LIB CameraNavigationControl : public CameraControl
{

    NON_COPYABLE
    
    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    CameraNavigationControl();
    
    virtual ~CameraNavigationControl();

    virtual void setCamera( base::Spatial& ) override;
    
    virtual void rotateHorizontally( float radians ) override;
    
    virtual void rotateVertically( float radians ) override;
    
    virtual void moveAxially( float distance ) override;

}; // CameraNavigationControl



}  // namespace Carna :: helpers

}  // namespace Carna



#endif // CAMERANAVIGATIONCONTROL_H_6014714286
