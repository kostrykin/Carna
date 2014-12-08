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

#ifndef AXIALPLANEVISUALIZATION_H_6014714286
#define AXIALPLANEVISUALIZATION_H_6014714286

/** \file   AxialPlaneVisualization.h
  * \brief  Defines \ref Carna::MPR::AxialPlaneVisualization.
  */

#include <Carna/base/Visualization.h>

namespace Carna
{

namespace MPR
{



// ----------------------------------------------------------------------------------
// MPR :: AxialPlaneVisualization
// ----------------------------------------------------------------------------------
    
/** \brief  Abstract implementation of the \ref Visualization class that is supposed
  *         to display the orthogonal intersections \f$ \vec x : \langle \vec e_i, \vec x \rangle = d_i, i = \{ 1, 2, 3 \} \f$
  *         of the volume data set, \f$ d_i \f$ being variable. This is a
  *         generalization of the \ref DefaultAxialPlaneVisualization class.
  *
  * It is expected that a visualization implementing this interface realizes itself
  * through an implementation of the \ref AxialPlaneRenderer class.
  *
  * \note
  * It is safe to call all functions defined by this class' public interface even if
  * the visualization's initialization was not finished yet.
  *
  * \section AxialPlaneVisualization_Perspective Perspective
  *
  * Each \c %AxialPlaneVisualization always visualizes all three intersection planes
  * \f$ \vec x : \langle \vec e_i, \vec x \rangle = d_i \f$ with \f$ i = \{ 1, 2, 3 \} \f$.
  * However, depending on the perspective of the camera, only one plane might be actually
  * noticeable as such, while the other two planes could only be recognized as lines,
  * because they are orthogonal to the point of view.
  *
  * The following three methods take care of creating this particular scenario:
  *
  * - \ref makeFrontView makes the plane that is identified by \f$ i = 3 \f$ the only noticeable one.
  * - \ref makeLeftView does the same for the plane that is identified by \f$ i = 1 \f$.
  * - \ref makeTopView does the same for \f$ i = 2 \f$.
  * 
  * Hereinafter \f$ i \f$ identifies the only noticeable plane.
  *
  * The two planes that are degenerated to lines, identified by
  * \f$ j \in \left( \{ 1, 2, 3 \} \setminus i \right)\f$, are highlighted additionally
  * as colored lines.
  *
  * Besides choosing the proper perspective, these three methods also take care of
  * setting up an appropriate \ref AxialPlaneController instance, which ensures:
  *
  * - Those lines are treated like sliders: The user is able to set the origin distances \f$ d_j \f$
  *   via drag & drop.
  * - Drag & drop is also supported for \ref base::model::Object3D "additional 3D geometry",
  *   meaning that it can easily be moved around.
  * - The mouse cursor adapts it's shape when sliders or 3D objects are hovered.
  *
  * \see
  * An example of using customized controllers to denote clicked coordinates is provided
  * \ref AxialPlaneController_Picking "here".
  *
  * \since  \ref v_2_3
  * \author Leonid Kostrykin
  * \date   18.11.13 - 23.6.14
  */
class CARNA_EXPORT AxialPlaneVisualization : public base::Visualization
{

public:

    /** \brief  Instantiates.
      */
    AxialPlaneVisualization( base::model::Scene& model );

    /** \brief  Does nothing.
      */
    virtual ~AxialPlaneVisualization();


    /** \brief  References the aggregated model.
      *
      * \since  \ref v_2_3
      */
    base::model::Scene& model;

    
    /** \brief
      * Configures this visualization to show the
      * \f$ i = 3 \Rightarrow \langle e_3,\ x \rangle = d \f$ intersection.
      *
      * The user is able to set the origin distance \f$ d \f$ via drag & drop.
      */
    virtual void makeFrontView();
    
    /** \brief
      * Configures this visualization to show the
      * \f$ i = 1 \Rightarrow \langle e_1,\ x \rangle = d \f$ intersection.
      *
      * The user is able to set the origin distance \f$ d \f$ via drag & drop.
      */
    virtual void makeLeftView();

    /** \brief
      * Configures this visualization to show the
      * \f$ i = 2 \Rightarrow \langle e_2,\ x \rangle = d \f$ intersection.
      *
      * The user is able to set the origin distance \f$ d \f$ via drag & drop.
      */    
    virtual void makeTopView();
    
    /** \brief
      * Configures this visualization to show all three \f$ i = \{ 1, 2, 3 \} \f$ intersections.
      *
      * Equivalent:
      *
      * \code
      * static void makeRotatable( Carna::MPR::AxialPlaneVisualization& view )
      * {
      *     view.doAfterInitialization( [&]()
      *         {
      *             using namespace Carna::MPR;
      *             using namespace Carna::base::view;
      *             using namespace Carna::base::controller;
      *
      *          // Since we allow custom rotations, it is not guaranteed for the planes to
      *          // be aligned along the screen space axis - therefore we simply turn off
      *          // the slider picking. Alternatively you could implement a more flexible
      *          // SliderScreenSpaceBuffer and supply it to the view.
      *
      *             view.renderer().setSliderScreenSpaceBuffer( new NullSliderScreenSpaceBuffer() );
      *
      *          // We want all sliders to be painted:
      *
      *             view.renderer().setSliderPainter( new DefaultSliderPainter() );
      *
      *          // Pick a different strategy for processing mouse press events. The supplied
      *          // strategy initializes a dragging activity, which rotates the view:
      *
      *             view.setController( new DefaultInteractionStrategy( *this ) );
      *
      *          // Setup a rotatable camera:
      *
      *             Camera& camera = view.renderer().camera();
      *             DefaultCamera* defaultCamera;
      *             if( ( defaultCamera = dynamic_cast< DefaultCamera* >( &camera ) ) == nullptr )
      *             {
      *                 defaultCamera = new DefaultCamera( pimpl->model );
      *                 view.renderer().setCamera( new Composition< Camera >( defaultCamera ) );
      *             }
      *             defaultCamera->setRotatable( true );
      *         }
      *     );
      * }
      * \endcode
      */
    virtual void makeRotatableView();


    /** \brief  References the mutable composed renderer.
      *
      * \pre    <code>isInitialized() == true</code>
      *
      * Specialization of \ref base::Visualization::renderer that returns only such renderers that are of class \ref AxialPlaneRenderer.
      */
    AxialPlaneRenderer& renderer();
    
    /** \brief  References the immutable composed renderer.
      *
      * \pre    <code>isInitialized() == true</code>
      *
      * Specialization of \ref base::Visualization::renderer that returns only such renderers that are of class \ref AxialPlaneRenderer.
      */
    const AxialPlaneRenderer& renderer() const;

}; // MPR :: AxialPlaneVisualization



}  // namespace Carna :: MPR

}  // namespace Carna

#endif // AXIALPLANEVISUALIZATION_H_6014714286
