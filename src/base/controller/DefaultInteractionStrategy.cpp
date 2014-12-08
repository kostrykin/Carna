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

#include <Carna/base/controller/DefaultInteractionStrategy.h>
#include <Carna/base/controller/DefaultInteractionStrategyDetails.h>
#include <Carna/base/controller/MovementIntent.h>
#include <Carna/base/controller/MovementIntentProcessor.h>
#include <Carna/base/controller/Object3DMovementIntentProcessor.h>
#include <Carna/base/controller/DefaultCameraController.h>
#include <Carna/base/model/Object3D.h>
#include <Carna/base/view/Renderer.h>
#include <Carna/base/view/SceneProvider.h>
#include <Carna/base/view/Camera.h>
#include <Carna/base/CarnaException.h>
#include <Carna/base/Visualization.h>
#include <Carna/base/VisualizationEnvironment.h>
#include <QMouseEvent>

namespace Carna
{

namespace base
{

namespace controller
{



// ----------------------------------------------------------------------------------
// DefaultInteractionStrategy :: Details
// ----------------------------------------------------------------------------------

DefaultInteractionStrategy::Details::Details
    ( DefaultInteractionStrategy& self
    , InteractionStrategy* cameraController )

    : self( self )
    , pickedObject( nullptr )
    , cameraController( cameraController )
{
}


DefaultInteractionStrategy::Details::~Details()
{
}


void DefaultInteractionStrategy::Details::beginMoving3dObject()
{
    if( !pickedObject )
    {
        return;
    }

    self.setMovementIntentProcessor(
        new Object3DMovementIntentProcessor
            ( *pickedObject
            , pickedPoint
            , self.module.renderer() ) );
}


void DefaultInteractionStrategy::Details::rename3dObject()
{
    if( !pickedObject )
    {
        return;
    }

    bool ok;
    const QString text = self.module.environment().promptText
        ( "Renaming Object"
        , "Rename:"
        , QLineEdit::Normal
        , QString::fromStdString( pickedObject->name() )
        , ok );

    if( ok && !text.isEmpty() )
    {
        pickedObject->setName( text.toStdString() );
    }
}


void DefaultInteractionStrategy::Details::delete3dObject()
{
    if( !pickedObject )
    {
        return;
    }

    // release picked object

    delete pickedObject;
    pickedObject = nullptr;
}


bool DefaultInteractionStrategy::Details::performObjectRelatedClickAction( QMouseEvent* ev )
{
    pickedObject = self.module.renderer().pickObject3D( ev->pos() );
    pickedPoint = ev->pos();

    if( pickedObject )
    {
        switch( ev->button() )
        {

        case Qt::RightButton:
            {
                const std::unique_ptr< VisualizationEnvironment::MenuBuilder > menu( self.module.environment().createMenu() );

             // build menu

                menu->addAction
                    ( "&Move"
                    , this
                    , SLOT( beginMoving3dObject() ) );

                menu->addAction
                    ( "Re&name"
                    , this
                    , SLOT( rename3dObject() ) );

                menu->addSeparator();

                menu->addAction
                    ( "&Delete"
                    , this
                    , SLOT( delete3dObject() ) );

                self.module.environment().setViewCursor( Qt::ArrowCursor );
                menu->present( ev->x(), ev->y() );

                ev->accept();
                return true;
            }

        case Qt::LeftButton:
            {
                beginMoving3dObject();

                ev->accept();
                return true;
            }

        default:
            {
            }

        }
    }

    return false;
}


bool DefaultInteractionStrategy::Details::performMovement( QMouseEvent* ev )
{
    if( self.hasMovementIntentProcessor() )
    {
        self.movementIntentProcessor().processIntent( MovementIntent( ev->pos() ) );

        ev->accept();
        return true;
    }

    return false;
}


void DefaultInteractionStrategy::Details::delegateToCamera( Visualization& sourceModule, QEvent* event )
{
    CARNA_ASSERT( cameraController.get() != nullptr );

    cameraController->event( sourceModule, event );
}



// ----------------------------------------------------------------------------------
// DefaultInteractionStrategy
// ----------------------------------------------------------------------------------

DefaultInteractionStrategy::DefaultInteractionStrategy( Visualization& module )
    : module( module )
    , pimpl( new Details( *this, new DefaultCameraController() ) )
{
}


DefaultInteractionStrategy::DefaultInteractionStrategy
    ( Visualization& module
    , InteractionStrategy* cameraController )

    : module( module )
    , pimpl( new Details( *this, cameraController ) )
{
    CARNA_ASSERT( cameraController != nullptr );
}


DefaultInteractionStrategy::~DefaultInteractionStrategy()
{
}


void DefaultInteractionStrategy::event( Visualization& sourceModule, QEvent* event )
{
    switch( event->type() )
    {

        case QEvent::MouseButtonPress:
        {
            if( pimpl->performObjectRelatedClickAction( static_cast< QMouseEvent* >( event ) ) )
            {
                return;
            }
            break;
        }

        case QEvent::MouseButtonRelease:
        {
            setMovementIntentProcessor( nullptr );
            break;
        }

        case QEvent::MouseMove:
        {
            if( pimpl->performMovement( static_cast< QMouseEvent* >( event ) ) )
            {
                return;
            }
            break;
        }

    }

    pimpl->delegateToCamera( sourceModule, event );
}


void DefaultInteractionStrategy::setMovementIntentProcessor( MovementIntentProcessor* processor )
{
    pimpl->movementIntentProcessor.reset( processor );
}


MovementIntentProcessor& DefaultInteractionStrategy::movementIntentProcessor()
{
    return *pimpl->movementIntentProcessor;
}


bool DefaultInteractionStrategy::hasMovementIntentProcessor() const
{
    return pimpl->movementIntentProcessor.get();
}


void DefaultInteractionStrategy::setCameraController( InteractionStrategy* cameraController )
{
    CARNA_ASSERT( cameraController != nullptr );

    pimpl->cameraController.reset( cameraController );

    emit cameraControllerExchanged();
    emit cameraControllerExchanged( *cameraController );
}


InteractionStrategy& DefaultInteractionStrategy::cameraController()
{
    CARNA_ASSERT( pimpl->cameraController != nullptr );

    return *pimpl->cameraController;
}


const InteractionStrategy& DefaultInteractionStrategy::cameraController() const
{
    CARNA_ASSERT( pimpl->cameraController != nullptr );

    return *pimpl->cameraController;
}



}  // namespace Carna :: base :: controller

}  // namespace Carna :: base

}  // namespace Carna
