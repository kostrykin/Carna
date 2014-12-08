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

#ifndef DEFAULTINTERACTIONSTRATEGY_DETAILS_H_6014714286
#define DEFAULTINTERACTIONSTRATEGY_DETAILS_H_6014714286

/** \file   DefaultInteractionStrategyDetails.h
  * \brief  Defines \ref Carna::base::controller::DefaultInteractionStrategy::Details.
  */

#include <Carna/base/controller/DefaultInteractionStrategy.h>
#include <QPoint>

namespace Carna
{

namespace base
{

namespace controller
{



// ----------------------------------------------------------------------------------
// DefaultInteractionStrategy :: Details
// ----------------------------------------------------------------------------------

class DefaultInteractionStrategy::Details : public QObject
{

    Q_OBJECT

    NON_COPYABLE

    DefaultInteractionStrategy& self;

public:

    Details( DefaultInteractionStrategy& self, InteractionStrategy* cameraController );

    virtual ~Details();


    model::Object3D* pickedObject;

    QPoint pickedPoint;


    std::unique_ptr< MovementIntentProcessor > movementIntentProcessor;

    std::unique_ptr< InteractionStrategy > cameraController;
    

    /** \brief  Opens context menu when an object is clicked.
      *
      * Returns \c true if a context menu has been opened and the event accepted.
      */
    bool performObjectRelatedClickAction( QMouseEvent* );

    /** \brief  Interprets the given event as \ref MovementIntent.
      *
      * Returns \c true if something has been moved and the event accepted.
      *
      * The event is passed to the currently set \ref MovementIntentProcessor and
      * \c true is returned. If no such processor is set, \c false is returned.
      */
    bool performMovement( QMouseEvent* );

    /** \brief  Delegates the interaction intent to the camera.
      */
    void delegateToCamera( Visualization& sourceModule, QEvent* event );


public slots:

    void beginMoving3dObject();

    void rename3dObject();

    void delete3dObject();

}; // DefaultInteractionStrategy :: Details



}  // namespace Carna :: base :: controller

}  // namespace Carna :: base

}  // namespace Carna

#endif // DEFAULTINTERACTIONSTRATEGY_DETAILS_H_6014714286
