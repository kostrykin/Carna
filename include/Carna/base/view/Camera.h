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

#ifndef CAMERA_H_6014714286
#define CAMERA_H_6014714286

/** \file   Camera.h
  * \brief  Defines \ref Carna::base::view::Camera.
  */

#include <Carna/Carna.h>
#include <Carna/base/Transformation.h>
#include <QObject>

class QMouseEvent;
class QWheelEvent;

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// Camera
// ----------------------------------------------------------------------------------

/** \brief
  * \ref Renderer component that defines the point of view and perspective of the spectator.
  *
  * Speaking strictly, this' class main purpose is to define
  * \ref CoordinateSystems "coordinate mapping from scene space into screen space",
  * which it does by supplying the \em projection- and the \em camera \em matrices:
  *
  * \image html 3D-Transformation.png
  *
  * Implementations are also capable of performing any custom GL operations before
  * and after the usual drawing: refer to \ref Camera::openShutter "openShutter" and
  * \ref Camera::closeShutter "closeShutter".
  *
  * \see \ref DefaultCamera is a commonly used implementation.
  *
  * \author Leonid Kostrykin
  * \date   8.6.11 - 18.3.13
  */
class CARNA_EXPORT Camera : public QObject
{

    Q_OBJECT

public:

    /** \brief  Does nothing.
      */
    virtual ~Camera()
    {
    }


    /** \brief  Obtains the \em camera \em matrix.
      */
    virtual void fetchCameraMatrix( Renderer&, Transformation& ) = 0;

    /** \brief  Obtains the \em projection \em matrix.
      */
    virtual void fetchProjectionMatrix( Renderer&, Transformation& ) = 0;


    /** \brief
      * Invoked by the \ref Renderer "renderer" right \em before the the actual drawing.
      *
      * Note that most \ref Renderer implementations will call \c glClear afterwards.
      */
    virtual void openShutter( Renderer& ) = 0;
    
    /** \brief
      * Invoked by the \ref Renderer "renderer" right \em after the the actual drawing.
      */
    virtual void closeShutter( Renderer& ) = 0;


signals:

    /** \brief  Suggests scheduling a repaint.
      *
      * Indicates that new matrices might be available.
      */
    void viewChanged();

}; // Camera



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // CAMERA_H_6014714286
