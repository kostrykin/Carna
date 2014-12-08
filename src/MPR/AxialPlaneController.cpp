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

#include <Carna/MPR/AxialPlaneController.h>
#include <Carna/MPR/AxialPlaneControllerDetails.h>
#include <Carna/MPR/AxialPlaneRenderer.h>
#include <Carna/MPR/AxialPlaneVisualization.h>
#include <Carna/MPR/SliderType.h>
#include <Carna/MPR/SliderScreenSpaceBuffer.h>
#include <Carna/MPR/IntersectionMovementIntentProcessor.h>
#include <Carna/base/controller/DefaultCameraController.h>
#include <Carna/base/model/Position.h>
#include <Carna/base/view/SceneProvider.h>
#include <Carna/base/view/Point3D.h>
#include <Carna/base/CarnaException.h>
#include <Carna/base/Visualization.h>
#include <Carna/base/VisualizationEnvironment.h>
#include <QMouseEvent>
#include <QMenu>

namespace Carna
{

namespace MPR
{



// ----------------------------------------------------------------------------------
// MPR :: AxialPlaneController :: Details
// ----------------------------------------------------------------------------------

AxialPlaneController::Details::Details( AxialPlaneController& self )
    : self( self )
    , parentHasContextMenu( false )
{
}


AxialPlaneController::Details::~Details()
{
}


void AxialPlaneController::Details::spawnPoint()
{
    CARNA_ASSERT( clickedPosition.get() != nullptr );

    new base::view::Point3D( self.model, *clickedPosition );
}


    
// ----------------------------------------------------------------------------------
// MPR :: AxialPlaneController
// ----------------------------------------------------------------------------------

AxialPlaneController::AxialPlaneController
    ( AxialPlaneVisualization& module
    , const ViewDirection& orientation
    , const base::Flags< CursorCapabilities >& cursorMode )

    : DefaultInteractionStrategy( module )
    , model( module.model )
    , orientation( orientation )
    , cursorMode( cursorMode )
    , pimpl( new Details( *this ) )
{
    base::controller::DefaultCameraController* const cameraController = new base::controller::DefaultCameraController();
    cameraController->setRotatable( false );
    this->setCameraController( cameraController );
}


AxialPlaneController::~AxialPlaneController()
{
}


void AxialPlaneController::setParentHavingContextMenu( bool parentHasContextMenu )
{
    pimpl->parentHasContextMenu = parentHasContextMenu;
}


bool AxialPlaneController::isParentHavingContextMenu() const
{
    return pimpl->parentHasContextMenu;
}


void AxialPlaneController::event( base::Visualization& sourceModule, QEvent* event )
{
    switch( event->type() )
    {

        case QEvent::MouseButtonPress:
        {
            if( !processMousePressed( event ) )
            {
                return;
            }
        }

        case QEvent::MouseMove:
        {
            if( !processMouseMoved( event ) )
            {
                return;
            }
        }

    }

    DefaultInteractionStrategy::event( sourceModule, event );
}


bool AxialPlaneController::processMouseMoved( QEvent* ev )
{
    AxialPlaneRenderer& renderer = static_cast< AxialPlaneRenderer& >( module.renderer() );
    QMouseEvent* const mouseEvent = static_cast< QMouseEvent* >( ev );

    if( !hasMovementIntentProcessor() )
    {
        if( cursorMode.isSet( adaptObjects ) )
        {
            base::model::Object3D* const pickedObject( renderer.pickObject3D( mouseEvent->pos() ) );
            if( pickedObject )
            {
                module.environment().setViewCursor( Qt::SizeAllCursor );
                return true;
            }
        }

        if( cursorMode.isSet( adaptAxis ) )
        {
            const SliderType sliderType
                = renderer.sliderScreenSpaceBuffer().hitTest( mouseEvent->pos() );

            switch( sliderType.orientation )
            {

            case SliderType::horizontal:
                module.environment().setViewCursor( Qt::SizeHorCursor );
                break;

            case SliderType::vertical:
                module.environment().setViewCursor( Qt::SizeVerCursor );
                break;

            case SliderType::undefined:
                module.environment().setViewCursor( Qt::ArrowCursor );
                break;

            }
        }
    }

    return true;
}


bool AxialPlaneController::processMousePressed( QEvent* ev, bool contextMenus )
{
    AxialPlaneRenderer& renderer = static_cast< AxialPlaneRenderer& >( module.renderer() );
    QMouseEvent* const mouseEvent = static_cast< QMouseEvent* >( ev );

    base::model::Object3D* const pickedObject( renderer.pickObject3D( mouseEvent->pos() ) );
    if( pickedObject )
    {
        return true;
    }

    if( mouseEvent->button() == Qt::LeftButton )
    {
        const SliderType sliderType
            = renderer.sliderScreenSpaceBuffer().hitTest( mouseEvent->pos() );

        if( sliderType != SliderType::null )
        {
            setMovementIntentProcessor( new IntersectionMovementIntentProcessor( sliderType, mouseEvent->pos(), renderer ) );

            ev->accept();
            return false;
        }
    }
    else
    if( mouseEvent->button() == Qt::RightButton && contextMenus )
    {
        pimpl->clickedPosition.reset( new base::model::Position( pickPosition( mouseEvent->x(), mouseEvent->y() ) ) );

        const std::unique_ptr< base::VisualizationEnvironment::MenuBuilder > menuBuilder( module.environment().createMenu() );
        menuBuilder->addAction( "Spawn &point here", this, SLOT( spawnPoint() ) );

        base::VisualizationEnvironment::MenuBuilder::ActionID ignoreEvent;
        if( isParentHavingContextMenu() )
        {
            menuBuilder->addSeparator();
            ignoreEvent = menuBuilder->addAction( "More..." );
        }

        module.environment().setViewCursor( Qt::ArrowCursor );

        const bool ignore = ( menuBuilder->present( mouseEvent->x(), mouseEvent->y() ) == ignoreEvent && isParentHavingContextMenu() );
        if( !ignore )
        {
            ev->accept();
        }

        return ignore;
    }

    return true;
}


base::model::Position AxialPlaneController::pickPosition( const int widgetspace_x, const int widgetspace_y ) const
{
    typedef base::Transformation Transformation3D;
    typedef base::Vector Vector3D;

 // compute viewport-space coordinates

    const int shortest_side = std::min
        ( module.renderer().targetWidth()
        , module.renderer().targetHeight() );

    const int viewport_x = ( module.renderer().targetWidth() == shortest_side ? widgetspace_x : widgetspace_x - ( module.renderer().targetWidth() - shortest_side ) / 2 );
    const int viewport_y = shortest_side - 1 - ( module.renderer().targetHeight() == shortest_side ? widgetspace_y : widgetspace_y - ( module.renderer().targetHeight() - shortest_side ) / 2 );

 // compute screen-space coordinates

    const double screenspace_x = ( static_cast< double >( viewport_x ) / module.renderer().shortestTargetSide() - 0.5 ) * 2;
    const double screenspace_y = ( static_cast< double >( viewport_y ) / module.renderer().shortestTargetSide() - 0.5 ) * 2;

    const Vector3D screenspace( screenspace_x, screenspace_y, 0 );

 // compute screen-space to volume-space transformations

    const Transformation3D screenSpaceToVolumeSpace = ( module.renderer().projectionTransformation()
                                                      * module.renderer().volumeViewTransformation() ).inverse();

 // transform screen-space coordinates to volume-space

    Vector3D volumespace = screenSpaceToVolumeSpace * screenspace;

 // fix wrong component

    CARNA_ASSERT( 0 <= orientation && orientation <= 2 );

    const Vector3D insection = static_cast< AxialPlaneRenderer& >( module.renderer() ).intersection();
    volumespace[ orientation ] = insection[ orientation ];

 // finish

    const base::model::Position position = base::model::Position::fromVolumeUnits
        ( model
        , volumespace.x()
        , volumespace.y()
        , volumespace.z() );

    return position;
}



}  // namespace MPR

}  // namespace Carna
