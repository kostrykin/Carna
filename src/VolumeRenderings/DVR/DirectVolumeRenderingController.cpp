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

#include <Carna/VolumeRenderings/DVR/DirectVolumeRenderingController.h>
#include <Carna/VolumeRenderings/DVR/DirectVolumeRendering.h>
#include <Carna/VolumeRenderings/VolumeRenderer.h>
#include <Carna/base/model/Object3D.h>
#include <Carna/base/model/Position.h>
#include <Carna/base/view/FramebufferObject.h>
#include <Carna/base/Visualization.h>
#include <Carna/base/VisualizationEnvironment.h>
#include <Carna/base/Transformation.h>
#include <QMouseEvent>
#include <QEvent>
#include <QColor>
#include <QDebug>

namespace Carna
{

namespace VolumeRenderings
{

namespace DVR
{



// ----------------------------------------------------------------------------------
// DVR :: DirectVolumeRendering :: Details :: Controller
// ----------------------------------------------------------------------------------

DirectVolumeRendering::Details::Controller::Controller( DirectVolumeRendering& self )
    : self( self )
{
}


void DirectVolumeRendering::Details::Controller::event( base::Visualization& sourceModule, QEvent* event )
{
    QMouseEvent* mouseEvent;

    if  ( event->type() == QEvent::MouseButtonPress &&
        ( mouseEvent = static_cast< QMouseEvent* >( event ) )->modifiers() & Qt::ControlModifier )
    {
        const unsigned int viewport_size = self.renderer().shortestTargetSide();

        const unsigned int viewport_off_x = ( self.renderer().targetWidth () - viewport_size ) / 2;
        const unsigned int viewport_off_y = ( self.renderer().targetHeight() - viewport_size ) / 2;

        const unsigned int widget_x = mouseEvent->x();
        const unsigned int widget_y = mouseEvent->y();

        const unsigned int viewport_x = widget_x - viewport_off_x;
        const unsigned int viewport_y = widget_y - viewport_off_y;

        const unsigned int screen_x = viewport_x;
        const unsigned int screen_y = viewport_size - viewport_y;

        self.renderer().module().environment().activateContext();
        base::view::FramebufferObject::MinimalBinding fbo_binding( *self.pimpl->fbo );
        const QColor picked_color = fbo_binding.readPixel( screen_x, screen_y, 1 );
        const base::Vector object_space( picked_color.redF()
                                       , picked_color.greenF()
                                       , picked_color.blueF()
                                       , picked_color.alphaF() );

        qDebug() << "at " << screen_x << ", " << screen_y;
        if( object_space[ 3 ] != 0.f )
        {
            qDebug() << object_space[ 0 ] << ", "
                     << object_space[ 1 ] << ", "
                     << object_space[ 2 ];

            if( self.hasPositionedObject() )
            {
                base::Vector object_space3 = object_space;
                object_space3.resize( 3 );

                self.getPositionedObject().setPosition( base::model::Position::fromVolumeUnits( self.pimpl->model, object_space3 ) );
                event->accept();
            }
        }
        else
        {
            qDebug() << "open space clicked";
        }
    }
}



}  // namespace Carna :: VolumeRenderings :: DVR

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna
