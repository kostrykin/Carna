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

#ifndef DEFAULTCAMERACONTROLLER_H_6014714286
#define DEFAULTCAMERACONTROLLER_H_6014714286

/** \file   DefaultCameraController.h
  * \brief  Defines \ref Carna::base::controller::DefaultCameraController.
  */

#include <Carna/Carna.h>
#include <Carna/base/controller/InteractionStrategy.h>
#include <QObject>

class QEvent;
class QMouseEvent;
class QWheelEvent;

namespace Carna
{

namespace base
{

namespace controller
{



// ----------------------------------------------------------------------------------
// DefaultCameraController
// ----------------------------------------------------------------------------------
    
/** \brief
  * \ref InteractionStrategy implementation that controls \ref view::DefaultCamera instances.
  *
  * \author Leonid Kostrykin
  * \date   21.3.13
  */
class CARNA_EXPORT DefaultCameraController : public QObject, public InteractionStrategy
{

    Q_OBJECT

public:

    /** \brief  Instantiates.
      */
    explicit DefaultCameraController( QObject* parent = nullptr );

    /** \brief  Destroys itself.
      */
    virtual ~DefaultCameraController();


    /** \brief
      * If the camera used by the \a sourceModule is of class \ref base::view::DefaultCamera,
      * than the event is used to update the camera's state; else nothing happens.
      *
      * Rotations are only applied to the camera if it \ref setRotatable "hasn't been forbidden".
      */
    virtual void event( Visualization& sourceModule, QEvent* event ) override;


    /** \brief
      * Tells whether the camera can be rotated using this controller.
      */
    bool isRotatable() const;


public slots:

    /** \brief  Sets whether the camera is rotatable by this controller.
      */
    void setRotatable( bool rotatable = true );


private:

    class Details;
    const std::unique_ptr< Details > pimpl;

}; // DefaultCameraController



}  // namespace Carna :: base :: controller

}  // namespace Carna :: base

}  // namespace Carna

#endif // DEFAULTCAMERACONTROLLER_H_6014714286
