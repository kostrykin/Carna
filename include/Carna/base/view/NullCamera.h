/*
 *  Copyright (C) 2010 - 2014 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#ifndef NULLCAMERA_H_6014714286
#define NULLCAMERA_H_6014714286

/** \file   NullCamera.h
  * \brief  Defines \ref Carna::base::view::NullCamera.
  */

#include <Carna/base/view/Camera.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// NullCamera
// ----------------------------------------------------------------------------------

/** \brief  Null-implementation of \ref Camera
  *
  * \author Leonid Kostrykin
  * \date   15.12.11 - 21.3.13
  */
class CARNA_EXPORT NullCamera : public Camera
{

    Q_OBJECT

public:

    /** \brief  Makes \a trafo identity.
      */
    virtual void fetchCameraMatrix( Renderer&, Transformation& trafo ) override
    {
        trafo.reset();
    }

    /** \brief  Makes \a trafo identity.
      */
    virtual void fetchProjectionMatrix( Renderer&, Transformation& trafo ) override
    {
        trafo.reset();
    }


    /** \brief  Does nothing.
      */
    virtual void openShutter( Renderer& ) override
    {
    }

    /** \brief  Does nothing.
      */
    virtual void closeShutter( Renderer& ) override
    {
    }

}; // NullCamera



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // NULLCAMERA_H_6014714286
