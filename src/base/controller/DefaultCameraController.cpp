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

#include <Carna/base/controller/DefaultCameraController.h>
#include <Carna/base/view/DefaultCamera.h>
#include <Carna/base/view/Renderer.h>
#include <Carna/base/Transformation.h>
#include <Carna/base/Visualization.h>
#include <Carna/base/CarnaException.h>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPoint>

namespace Carna
{

namespace base
{

namespace controller
{



// ----------------------------------------------------------------------------------
// DefaultCameraController :: Details
// ----------------------------------------------------------------------------------

class DefaultCameraController :: Details
{

    DefaultCameraController& self;

public:

    Details( DefaultCameraController& );


    void onMousePress( view::Renderer&, QMouseEvent* );

    void onMouseMove( view::Renderer&, QMouseEvent* );

    void onMouseRelease( view::Renderer&, QMouseEvent* );

    void onMouseWheel( view::Renderer&, QWheelEvent* );


    bool mouseInteraction;

    bool rotatable;

    QPoint mousepos;

}; // DefaultCameraController :: Details


DefaultCameraController::Details::Details( DefaultCameraController& self )
    : self( self )
    , mouseInteraction( false )
    , rotatable( true )
{
}


void DefaultCameraController::Details::onMousePress( view::Renderer& renderer, QMouseEvent* ev )
{
    if( ev->button() == Qt::LeftButton )
    {
        mouseInteraction = true;
        mousepos = ev->pos();

        ev->accept();
    }
}


void DefaultCameraController::Details::onMouseMove( view::Renderer& renderer, QMouseEvent* ev )
{
    view::DefaultCamera* const camera = dynamic_cast< view::DefaultCamera* >( &renderer.camera() );

    if( camera != nullptr && mouseInteraction )
    {
        if( mousepos.x() == INT_MIN || mousepos.y() == INT_MIN ) return;

        const int dx = ( ev->x() - mousepos.x() );
        const int dy = ( ev->y() - mousepos.y() );
        mousepos = ev->pos();

        if( !self.isRotatable() || ev->modifiers() & Qt::ShiftModifier )
        {
            camera->shift( dx, dy );
        }
        else
        {
            Transformation rotation;
            rotation.rotateY( dx, Transformation::DEGREES );
            rotation.rotateX( dy, Transformation::DEGREES );

            camera->rotate( rotation );
        }

        ev->accept();
    }
}


void DefaultCameraController::Details::onMouseRelease( view::Renderer& renderer, QMouseEvent* ev )
{
    if( mouseInteraction )
    {
        mouseInteraction = false;

        ev->accept();
    }
}


void DefaultCameraController::Details::onMouseWheel( view::Renderer& renderer, QWheelEvent* ev )
{
    view::DefaultCamera* const camera = dynamic_cast< view::DefaultCamera* >( &renderer.camera() );

    if( camera != nullptr )
    {
        const float zoomDelta = ev->delta() * 1e-3;

        const float dx = ev->pos().x() - ( renderer.targetWidth () / 2.f );
        const float dy = ev->pos().y() - ( renderer.targetHeight() / 2.f );

        camera->zoom( zoomDelta, dx, dy );

        ev->accept();
    }
}



// ----------------------------------------------------------------------------------
// DefaultCameraController
// ----------------------------------------------------------------------------------

DefaultCameraController::DefaultCameraController( QObject* parent )
    : QObject( parent )
    , pimpl( new Details( *this ) )
{
}


DefaultCameraController::~DefaultCameraController()
{
}


void DefaultCameraController::event( Visualization& sourceModule, QEvent* event )
{
    CARNA_ASSERT( event != nullptr );
    CARNA_ASSERT( !event->isAccepted() );
    
    switch( event->type() )
    {

        case QEvent::MouseButtonPress:
        {
            pimpl->onMousePress( sourceModule.renderer(), static_cast< QMouseEvent* >( event ) );
            break;
        }

        case QEvent::MouseButtonRelease:
        {
            pimpl->onMouseRelease( sourceModule.renderer(), static_cast< QMouseEvent* >( event ) );
            break;
        }

        case QEvent::MouseMove:
        {
            pimpl->onMouseMove( sourceModule.renderer(), static_cast< QMouseEvent* >( event ) );
            break;
        }

        case QEvent::Wheel:
        {
            pimpl->onMouseWheel( sourceModule.renderer(), static_cast< QWheelEvent* >( event ) );
            break;
        }

    }
}


void DefaultCameraController::setRotatable( bool rotatable )
{
    pimpl->rotatable = rotatable;
}


bool DefaultCameraController::isRotatable() const
{
    return pimpl->rotatable;
}



}  // namespace Carna :: base :: controller

}  // namespace Carna :: base

}  // namespace Carna
