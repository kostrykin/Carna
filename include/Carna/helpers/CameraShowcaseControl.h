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

#ifndef CAMERASHOWCASECONTROL_H_6014714286
#define CAMERASHOWCASECONTROL_H_6014714286

#include <Carna/Carna.h>
#include <Carna/helpers/CameraControl.h>
#include <memory>

/** \file   CameraShowcaseControl.h
  * \brief  Defines \ref Carna::helpers::CameraShowcaseControl.
  */

namespace Carna
{

namespace helpers
{



// ----------------------------------------------------------------------------------
// CameraShowcaseControl
// ----------------------------------------------------------------------------------

class CARNA_LIB CameraShowcaseControl : public CameraControl
{

    NON_COPYABLE
    
    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    const static float DEFAULT_MIN_DISTANCE;
    const static float DEFAULT_MAX_DISTANCE;

    CameraShowcaseControl();
    
    ~CameraShowcaseControl();
    
    void setMinDistance( float minDistance );
    
    void setMaxDistance( float maxDistance );
    
    float minDistance() const;
    
    float maxDistance() const;

    virtual void setCamera( base::Spatial& ) override;
    
    virtual void rotateHorizontally( float radians ) override;
    
    virtual void rotateVertically( float radians ) override;
    
    virtual void moveAxially( float distance ) override;

}; // CameraShowcaseControl



}  // namespace Carna :: helpers

}  // namespace Carna



#endif // CAMERASHOWCASECONTROL_H_6014714286
