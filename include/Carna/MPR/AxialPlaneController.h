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

#ifndef AXIALPLANE_AXIALPLANECONTROLLER_H_6014714286
#define AXIALPLANE_AXIALPLANECONTROLLER_H_6014714286

/** \file   AxialPlaneController.h
  * \brief  Defines \ref Carna::MPR::AxialPlaneController.
  */

#include <Carna/base/controller/DefaultInteractionStrategy.h>
#include <Carna/base/Flags.h>

namespace Carna
{

namespace MPR
{



// ----------------------------------------------------------------------------------
// MPR :: AxialPlaneController
// ----------------------------------------------------------------------------------

/** \brief  \ref AxialPlaneVisualization sufficient specialization of \ref base::controller::DefaultInteractionStrategy
  *
  * This implementation disables \ref base::controller::DefaultCameraController::setRotatable "the user's ability to rotate the camera".
  * As a consequence, the perspective must be \ref base::view::DefaultCamera::setRotation "set explicitly".
  *
  * \section AxialPlaneController_Picking Denoting clicked coordinates
  *
  * The \c %AxialPlaneController class provides a \ref pickPosition method. This
  * method transforms widget-space coordinates into \ref VolumeSpace "volume or model space".
  * The code below shows an example of how to extend the existing \c %AxialPlaneController
  * implementation to make it denote the volume-space coordinates the user clicks by
  * spawning \ref base::view::Point3D "points" at those coordinates:
  *
  * \code
  * class MyAxialPlaneController : public Carna::MPR::AxialPlaneController
  * {
  * public:
  *     MyAxialPlaneController
  *         ( Carna::MPR::AxialPlaneVisualization& module
  *         , Carna::base::model::Scene& model
  *         , const ViewDirection& orientation
  *         , const Carna::base::Flags< CursorCapabilities >& cursorMode
  *             = Carna::base::Flags< CursorCapabilities >( adaptAxis ).or( adaptObjects ) );
  *
  *     virtual void event( Carna::base::Visualization& sourceModule, QEvent* event ) override;
  * }
  * \endcode
  *
  * The interface above might be implemented as follows. In the presented code snippet
  * the new functionality of spawning \c Point3D instances is enabled by holding the
  * \c SHIFT key. Otherwise, the default functionality is used by delegating the %event
  * to \c %Carna::MPR::AxialPlaneController::event.
  *
  * \code
  * MyAxialPlaneController::MyAxialPlaneController
  *         ( Carna::MPR::AxialPlaneVisualization& module
  *         , const ViewDirection& orientation
  *         , const Carna::base::Flags< CursorCapabilities >& cursorMode )
  *     : Carna::MPR::AxialPlaneController( module, orientation, cursorMode )
  * {
  * }
  *
  * void MyAxialPlaneController::event( Carna::base::Visualization& sourceModule, QEvent* event )
  * {
  *     switch( event->type() )
  *     {
  *         case QEvent::MouseButtonPress:
  *         {
  *             QMouseEvent* const mouseEvent = static_cast< QMouseEvent* >( event );
  *             if( mouseEvent->modifiers() & Qt::ShiftModifier )
  *             {
  *                 const auto pickedPosition = pickPosition( mouseEvent->x(), mouseEvent->y() );
  *                 new Carna::base::view::Point3D( model, pickedPosition.toVolumeSpace() );
  *                 event->accept();
  *                 return;
  *             }
  *             break;
  *         }
  *     }
  *     Carna::MPR::AxialPlaneController::event( sourceModule, event );
  * }
  * \endcode
  *
  * When we have our extended \c %AxialPlaneController implementation done, it is
  * time to make an \c %AxialPlaneVisualization actually use it in place of the
  * default controller:
  *
  * \code
  * static void adjustInteractionStrategy
  *         ( Carna::MPR::AxialPlaneVisualization& visualization
  *         , const ViewDirection& orientation )
  * {
  *     visualization.doAfterInitialization( [visualization]
  *         {
  *             visualization.setController( new MyAxialPlaneController
  *                 ( visualization
  *                 , visualization.renderer().provider.scene
  *                 , orientation ) );
  *         }
  *     );
  * }
  * \endcode
  *
  * \attention
  * Note that the visualization's methods
  * \ref AxialPlaneVisualization::makeFrontView "makeFrontView",
  * \ref AxialPlaneVisualization::makeLeftView "makeLeftView" and
  * \ref AxialPlaneVisualization::makeTopView "makeTopView" override the previously
  * set controller. Hence it is convenient to either
  * - ensure that the above function \c adjustInteractionStrategy is invoked \em after
  *   the \ref AxialPlaneVisualization_Perspective "the perspective was set up",
  * - or create an own \c %AxialPlaneVisualization implementation by deriving from
  *   \ref DefaultAxialPlaneVisualization and override these methods to invoke
  *   \c adjustInteractionStrategy right after calling the original \c makeFrontView,
  *   \c makeLeftView or \c makeTopView implementation.
  *
  * \author Leonid Kostrykin
  * \date   1.3.12 - 23.06.14
  */
class CARNA_EXPORT AxialPlaneController : public base::controller::DefaultInteractionStrategy
{

    Q_OBJECT

    NON_COPYABLE

public:

    /** \brief  Lists possible \ref AxialPlaneRenderer "axial plane renderer" directions.
      *
      * The value of each enumeration value specifies the depth-axis.
      */
    enum ViewDirection
    {
        front = 2,
        left = 0,
        top = 1
    };

    /** \brief  Lists supported cursor capabilities.
      *
      * \attention
      * Don't use \c adaptObjects if you are experiencing performance problems.
      */
    enum CursorCapabilities
    {
        /** \brief
          * Makes the cursor change it's shape when an axis slider is hovered.
          */
        adaptAxis = ( 1 << 0 ),
        
        /** \brief
          * Makes the cursor change it's shape when an 3D object is hovered.
          */
        adaptObjects = ( 1 << 1 )
    };


    /** \brief  Associates with particular visualization module.
      *
      * \attention
      * Don't use \c adaptObjects for \a cursorMode if you are experiencing performance problems.
      *
      * \param module
      * References the visualization module, this controller instance should belong to logically.
      *
      * \param orientation
      * Determines the \f$i\f$ in the definition of the rendered plane \f$ \vec x : \langle \vec e_i, \vec x \rangle = d_i \f$.
      * Refer \ref AxialPlaneRenderer "here" for details.
      *
      * \param cursorMode
      * Defines the cursor behavior. The flags are documented \ref CursorCapabilities "here".
      */
    AxialPlaneController
        ( AxialPlaneVisualization& module
        , const ViewDirection& orientation
        , const base::Flags< CursorCapabilities >& cursorMode = base::Flags< CursorCapabilities >( adaptAxis ).or( adaptObjects ) );

    /** \brief	Releases acquired resources.
      */
    virtual ~AxialPlaneController();


    /** \brief  References the mutable data model.
      */
    base::model::Scene& model;

    /** \brief  Holds the orientation of the managed \ref AxialPlaneRenderer "axial plane renderer".
      */
    const ViewDirection orientation;

    /** \brief  Holds the definition of the cursor behavior.
      *
      * See the \ref CursorCapabilities documentation for details.
      */
    const base::Flags< CursorCapabilities > cursorMode;


    virtual void event( base::Visualization& sourceModule, QEvent* event ) override;

    /** \brief  Transforms the given widget-space coordinates into \ref VolumeSpace "volume or model space".
      *
      * Example:
      *
      * \code
      * virtual void event( Carna::base::Visualization& sourceModule, QEvent* event ) override
      * {
      *     if( event->type() == QEvent::MouseButtonPress )
      *     {
      *         QMouseEvent* const mouseEvent = static_cast< QMouseEvent* >( event );
      *
      *         const Carna::base::Vector pickedPosition = pickPosition
      *                 ( mouseEvent->x(), mouseEvent->y() ).toVolumeSpace();
      *
      *         qDebug()
      *             << pickedPosition.x() << ", "
      *             << pickedPosition.y() << ", "
      *             << pickedPosition.z();
      *     }
      *
      *     Carna::MPR::AxialPlaneController::event( sourceModule, event );
      * }
      * \endcode
      *
      * The arguments \a widgetSpaceX and \a widgetSpaceY must be given in pixels.
      */
    base::model::Position pickPosition( int widgetSpaceX, int widgetSpaceY ) const;


    /** \brief
      * Tells whether the context menu produced by this interaction strategy offers a 'more'
      * item that propagates the event to the view's parent.
      */
    bool isParentHavingContextMenu() const;


public slots:
    
    /** \brief
      * Sets whether the context menu produced by this interaction strategy shall offer a 'more'
      * item that propagates the event to the view's parent.
      */
    void setParentHavingContextMenu( bool );


protected:

    /** \brief  Performs the default behavior of this interaction strategy implementation.
      *
      * You may use this function in your custom \ref event implementation.
      *
      * Returns whether further tasks may be performed in order to satisfy the interaction intent.
      * This means, that if this method is called from within your custom \ref event implementation
      * and it's returned value is \c false, you should finish your event processing:
      *
      * \code
      * virtual void event( Carna::base::Visualization& sourceModule, QEvent* event ) override
      * {
      *     // ...
      *
      *     if( !processMouseMoved( event ) )
      *     {
      *         return;
      *     }
      *
      *     // ...
      * }
      * \endcode
      */
    bool processMouseMoved( QEvent* );
    
    /** \brief  Performs the default behavior of this interaction strategy implementation.
      *
      * You may use this function in your custom \ref event implementation.
      * The \a contextMenus argument specifies whether context menus may be produced.
      *
      * Returns whether further tasks may be performed in order to satisfy the interaction intent.
      * This means, that if this method is called from within your custom \ref event implementation
      * and it's returned value is \c false, you should finish your event processing:
      *
      * \code
      * virtual void processEvent( Carna::base::Visualization& sourceModule, QEvent* event ) override
      * {
      *     // ...
      *
      *     if( !processMousePressed( event ) )
      *     {
      *         return;
      *     }
      *
      *     // ...
      * }
      * \endcode
      */
    bool processMousePressed( QEvent*, bool contextMenus = true );


private:

    class Details;
    const std::unique_ptr< Details > pimpl;

}; // MPR :: AxialPlaneController



}  // namespace Carna :: MPR

}  // namespace Carna

#endif // AXIALPLANE_AXIALPLANECONTROLLER_H_6014714286
