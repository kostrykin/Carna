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

#ifndef PROJECTIONCONTROL_H_6014714286
#define PROJECTIONCONTROL_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/math.h>
#include <memory>

/** \file   ProjectionControl.h
  * \brief  Defines \ref Carna::base::ProjectionControl.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// ProjectionControl
// ----------------------------------------------------------------------------------

class CARNA_LIB ProjectionControl
{

    NON_COPYABLE
    
    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    const static float DEFAULT_MINIMUM_VISIBLE_DISTANCE;
    const static float DEFAULT_MAXIMUM_VISIBLE_DISTANCE;

    ProjectionControl();
    
    virtual ~ProjectionControl();
    
    void setViewportWidth( unsigned int width );
    
    void setViewportHeight( unsigned int height );
    
    unsigned int viewportWidth() const;
    
    unsigned int viewportHeight() const;
    
    void setMinimumVisibileDistance( float minimumVisibleDistance );
    
    void setMaximumVisibileDistance( float maximumVisibleDistance );
    
    float minimumVisibleDistance() const;
    
    float maximumVisibleDistance() const;
    
    virtual void updateProjection( math::Matrix4f& ) const = 0;

}; // ProjectionControl



}  // namespace Carna :: base

}  // namespace Carna



#endif // PROJECTIONCONTROL_H_6014714286

