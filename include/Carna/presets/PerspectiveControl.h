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

class CARNA_LIB PerspectiveControl : public base::ProjectionControl
{

    NON_COPYABLE
    
    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    const static float DEFAULT_FOV_HORIZONTAL;

    PerspectiveControl();
    
    virtual ~PerspectiveControl();
    
    virtual void updateProjection( base::math::Matrix4f& ) const override;
    
    void setFovHorizontal( float radians );
    
    float fovHorizontal() const;

}; // PerspectiveControl



}  // namespace Carna :: presets

}  // namespace Carna



#endif // PERSPECTIVECONTROL_H_6014714286

