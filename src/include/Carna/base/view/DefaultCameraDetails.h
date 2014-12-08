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

#ifndef DEFAULTCAMERA_DETAILS_H_6014714286
#define DEFAULTCAMERA_DETAILS_H_6014714286

/** \file   DefaultCameraDetails.h
  * \brief  Defines \ref Carna::base::view::DefaultCamera::Details.
  */

#include <Carna/base/view/DefaultCamera.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// DefaultCamera :: Details
// ----------------------------------------------------------------------------------

/** \brief  PIMPL-implementation of \ref DefaultCamera
  *
  * \author Leonid Kostrykin
  * \date   27.9.12
  */
struct DefaultCamera::Details
{

    Details();


    float zoom;

    QPointF zoomPoint;


    /** \brief  Describes the inverse camera rotation.
      */
    Transformation rotation;

    /** \brief  Holds whether a perspective or an orthogonal projection is being used.
      */
    bool perspectiveProjection;


    Transformation normalization;

}; // DefaultCamera :: Details



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // DEFAULTCAMERA_DETAILS_H_6014714286
