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

#ifndef VOLUMECONTROLLERUI_H_6014714286
#define VOLUMECONTROLLERUI_H_6014714286

/** \file   VolumeControllerUI.h
  * \brief  Defines \ref Carna::VolumeRenderings::VolumeControllerUI.
  */

#include <Carna/Carna.h>
#include <QWidget>

namespace Carna
{

namespace VolumeRenderings
{



// ----------------------------------------------------------------------------------
// VolumeRenderings :: VolumeControllerUI
// ----------------------------------------------------------------------------------

/** \brief  \ref VolumeRenderer "Volume renderings" configuration widget
  *
  * \section VolumeControllerOperationModes Controller UI Operational Modes
  *
  * This controller is able to operate in two modes:
  * -# Only controlling the associated \ref VolumeRenderer "renderer's mode-independent settings"
  * -# Controlling the whole volume rendering, including the \ref VolumeRendererMode::createControllerUI "controller UI obtained from it's current rendering mode"
  *
  * The mode, in which this controller shall operate, is determined by the time at
  * which the controller is instantiated: If the renderer, that is passed to the
  * controller's constructor, already \em has rendering modes associated, than the
  * controller will also take responsibility for controlling those modes. Otherwise,
  * namely when the rendering modes are associated \em later - after the controller
  * already has been instantiated - the rendering modes will be ignored.
  *
  * \subsection VolumeControllerMinimal Controlling Associated Renderer's Mode-independent Settings
  *
  * Example:
  *
  * \code
  * static void doVolumeRendererDemo( Carna::base::model::Scene& model )
  * {
  *     using namespace Carna::VolumeRenderings;
  *
  *     VolumeVisualization* const view = new DefaultVolumeVisualization();
  *
  *  // create controller UI
  *
  *     view->doAfterInitialization( [&]()
  *         {
  *             VolumeControllerUI* const controller_ui =
  *                 new VolumeControllerUI( view->renderer(), model );
  *
  *             controller_ui->setAttribute( Qt::WA_DeleteOnClose );
  *             controller_ui->show();
  *         }
  *     );
  *
  *  // setup renderer
  *
  *     view->doAfterInitialization( [&]()
  *         {
  *             view->renderer().installMode( new DRR::XRay( model ) );
  *             view->renderer().installMode( new MIP::MaximumIntensityProjection() );
  *             view->renderer().installMode( new DVR::DirectVolumeRendering( model ) );
  *
  *          // set initial mode
  *
  *             view->renderer().setMode( DVR::DirectVolumeRendering::NAME );
  *         }
  *     );
  * }
  * \endcode
  *
  * The above code will produce this controller UI:
  *
  * \image html VolumeController02.png "volume visualization controller UI with no support for rendering modes controlling"
  *
  * \subsection VolumeControllerFull Controlling Whole Volume Renderer
  *
  * Example:
  *
  * \code
  * static void doVolumeRendererDemo( Carna::base::model::Scene& model )
  * {
  *     using namespace Carna::VolumeRenderings;
  *
  *     VolumeVisualization* const view = new DefaultVolumeVisualization();
  *
  *  // setup renderer
  *
  *     view->doAfterInitialization( [&]()
  *         {
  *             view->renderer().installMode( new DRR::XRay( model ) );
  *             view->renderer().installMode( new MIP::MaximumIntensityProjection() );
  *             view->renderer().installMode( new DVR::DirectVolumeRendering( model ) );
  *
  *          // set initial mode
  *
  *             view->renderer().setMode( DVR::DirectVolumeRendering::NAME );
  *         }
  *     );
  *
  *  // create controller UI
  *
  *     view->doAfterInitialization( [&]()
  *         {
  *             VolumeControllerUI* const controller_ui =
  *                 new VolumeControllerUI( view->renderer(), model );
  *
  *             controller_ui->setAttribute( Qt::WA_DeleteOnClose );
  *             controller_ui->show();
  *         }
  *     );
  * }
  * \endcode
  *
  * \attention
  * It is important that a \ref VolumeRendererMode rendering mode is \ref VolumeRenderer::setMode "already set"
  * by the moment the controller is instantiated, when operating in this mode. Otherwise, the
  * controller's constructor will throw an \ref base::AssertionFailure.
  *
  * The above code will produce this controller UI:
  *
  * \image html VolumeController01.png "volume visualization controller UI with rendering modes support"
  *
  * \section VolumeControllerFurther Rendering Mode Controllers
  *
  * If you need to handle the rendering mode controllers on your own, e.g. place them somewhere
  * differently but behind the general settings page of the volume controller, you should do the following:
  * -# Make the volume controller \ref VolumeControllerMinimal "process only the renderer's mode-independent settings".
  * -# \ref VolumeRendererMode::createControllerUI "Create rendering mode controllers" explicitly.
  *
  * \author Leonid Kostrykin
  * \date   8.8.11 - 19.3.13
  */
class CARNA_EXPORT VolumeControllerUI : public QWidget
{

public:

    /** \brief  Instantiates.
      *
      * \pre    <code>renderer.modes().empty() || renderer.hasMode()</code>
      */
    VolumeControllerUI
        ( VolumeRenderer& renderer
        , base::model::Scene& model );

    /** \brief  Removes the config widget of current VolumeRendererMode from GUI, since
      *         each mode itself is responsible for deleting its resources.
      */
    virtual ~VolumeControllerUI();


private:

    class Details;
    std::unique_ptr< Details > pimpl;

}; // VolumeRenderings :: VolumeControllerUI



}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // VOLUMECONTROLLERUI_H_6014714286
