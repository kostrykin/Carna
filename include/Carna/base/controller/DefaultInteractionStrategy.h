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

#ifndef DEFAULTINTERACTIONSTRATEGY_H_6014714286
#define DEFAULTINTERACTIONSTRATEGY_H_6014714286

/** \file   DefaultInteractionStrategy.h
  * \brief  Defines \ref Carna::base::controller::DefaultInteractionStrategy.
  */

#include <Carna/base/noncopyable.h>
#include <Carna/base/controller/InteractionStrategy.h>
#include <QObject>
#include <QPoint>

class QMouseEvent;

namespace Carna
{

namespace base
{

namespace controller
{



// ----------------------------------------------------------------------------------
// DefaultInteractionStrategy
// ----------------------------------------------------------------------------------

/** \brief
  * Implementation of InteractionStrategy which \ref setCameraController "controls the camera" and
  * is capable of \ref setMovementIntentProcessor "performing object dragging".
  *
  * Camera event processing is delegated to \ref setCameraController "the supplied camera controller".
  *
  * \author Leonid Kostrykin
  * \date   28.2.12 - 21.3.13
  */
class CARNA_EXPORT DefaultInteractionStrategy : public QObject, public InteractionStrategy
{

    NON_COPYABLE

    Q_OBJECT

public:

    /** \brief
      * References the visualization module this strategy is a component of.
      */
    Visualization& module;


    /** \brief
      * Associates with visualization \a module and
      * new \ref DefaultCameraController instance as the \ref setCameraController "camera controller".
      */
    explicit DefaultInteractionStrategy( Visualization& module );

    /** \brief
      * Associates with visualization \a module and \a cameraController.
      *
      * \pre
      * <code>cameraController != nullptr</code>
      */
    DefaultInteractionStrategy( Visualization& module, InteractionStrategy* cameraController );

    /** \brief
      * Releases acquired resources.
      */
    virtual ~DefaultInteractionStrategy();

    // ------------------------------------------------------------------------------
    // InteractionStrategy Interface
    // ------------------------------------------------------------------------------

    virtual void event( Visualization& sourceModule, QEvent* event ) override;

    // ------------------------------------------------------------------------------
    // MovementIntentProcessor Management
    // ------------------------------------------------------------------------------

    /** \brief  Defines the processing of \ref MovementIntent instances.
      */
    void setMovementIntentProcessor( MovementIntentProcessor* );

    /** \brief  References the definition of the \ref MovementIntent processing.
      */
    MovementIntentProcessor& movementIntentProcessor();

    /** \brief  References the definition of the \ref MovementIntent processing.
      */
    const MovementIntentProcessor& movementIntentProcessor() const
    {
        return const_cast< DefaultInteractionStrategy* >( this )->movementIntentProcessor();
    }

    /** \brief  Tells whether a \ref MovementIntent instances processing definition is supplied.
      */
    bool hasMovementIntentProcessor() const;

    // ------------------------------------------------------------------------------
    // Camera Controller
    // ------------------------------------------------------------------------------

    /** \brief  Sets a new camera controller.
      *
      * \pre    <code>newCameraController != nullptr</code>
      *
      * Emits the \ref cameraControllerExchanged signals.
      */
    void setCameraController( InteractionStrategy* newCameraController );

    /** \brief  References current mutable camera controller.
      */
    InteractionStrategy& cameraController();
    
    /** \brief  References current immutable camera controller.
      */
    const InteractionStrategy& cameraController() const;

    
signals:
    
    /** \brief  Emitted by \ref setCameraController when a new camera controller is set.
      */
    void cameraControllerExchanged();
    
    /** \brief  Emitted by \ref setCameraController when a new camera controller is set.
      */
    void cameraControllerExchanged( Carna::base::controller::InteractionStrategy& );


private:

    class Details;
    const std::unique_ptr< Details > pimpl;

}; // DefaultInteractionStrategy



}  // namespace Carna :: base :: controller

}  // namespace Carna :: base

}  // namespace Carna

#endif // DEFAULTINTERACTIONSTRATEGY_H_6014714286
