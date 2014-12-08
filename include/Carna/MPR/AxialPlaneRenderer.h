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

#ifndef AXIALPLANERENDERER_H_6014714286
#define AXIALPLANERENDERER_H_6014714286

/** \file   AxialPlaneRenderer.h
  * \brief  Defines \ref Carna::MPR::AxialPlaneRenderer.
  */

#include <Carna/base/view/Renderer.h>
#include <Carna/base/noncopyable.h>
#include <Carna/base/Vector3.h>
#include <Carna/Carna.h>

namespace Carna
{

namespace MPR
{



// ----------------------------------------------------------------------------------
// MPR :: AxialPlaneRenderer
// ----------------------------------------------------------------------------------

/** \brief
  * Abstract \ref base::view::Renderer specialization that is supposed to display the
  * orthogonal intersections \f$ \big\{ \vec x : \langle \vec e_i, \vec x \rangle = d_i, i \in \{ 1, 2, 3 \} \big\} \f$
  * of the volume data set, where \f$ d_i \f$ is adjustable. This is a generalization
  * of the \ref DefaultAxialPlaneRenderer class.
  *
  * \see
  * This renderer is part of the \ref AxialPlaneVisualization %module.
  *
  * The interface expects the rendered perspective to be fully determined by the
  * utilized camera. This means in particular that the camera determines which planes
  * actually are visible.
  *
  * The <em>intersection point</em> \f$ ( d_1, d_2, d_3 )^\mathrm T \f$
  * defines the position of the planes. It must be \ref setIntersection "set" using
  * \ref VolumeSpace "volume space coordinates". The intersection point is the one
  * point that is common to all three cross-sectional planes.
  *
  * \section AxialPlaneRendererCustomizations Customizations
  *
  * \par Disable colored plane indicators
  *
  * \code
  * static void disableSliderPainter( Carna::MPR::AxialPlaneRenderer& renderer )
  * {
  *     renderer.setSliderPainter( new Carna::MPR::NullSliderPainter() );
  * }
  * \endcode
  *
  * Also disable the picking ability, else the user might click some plane sliders
  * that actually are invisible:
  *
  * \par Disable ability to pick plane slider
  *
  * \code
  * static void disablePicking( Carna::MPR::AxialPlaneRenderer& renderer )
  * {
  *     renderer.setSliderScreenSpaceBuffer( new Carna::MPR::NullSliderScreenSpaceBuffer() );
  * }
  * \endcode
  *
  * \author Leonid Kostrykin
  * \date   18.11.2013
  */
class CARNA_EXPORT AxialPlaneRenderer : public base::view::Renderer
{

    Q_OBJECT

    NON_COPYABLE

public:

    /** \brief  Instantiates.
      */
    AxialPlaneRenderer
        ( base::Visualization& module
        , base::view::SceneProvider& provider
        , QWidget* parent = nullptr );

    /** \brief  Does nothing
      */
    virtual ~AxialPlaneRenderer();


    /** \brief
      * Tells common point of all three cross-sectional planes in
      * \ref VolumeSpace "volume space coordinates".
      */
    virtual const base::Vector& intersection() const = 0;

    /** \brief  Returns the mask's minimal HUV.
      */
    virtual int minimalMaskHuv() const = 0;

    /** \brief  Returns the mask's maximal HUV.
      */
    virtual int maximalMaskHuv() const = 0;


    /** \brief  Defines the way the plane sliders are painted.
      *
      * \pre    <code>painter != nullptr</code>
      */
    virtual void setSliderPainter( SliderPainter* painter ) = 0;

    /** \brief  References the plane slider painting definition.
      */
    virtual SliderPainter& sliderPainter() = 0;

    /** \brief  References the plane slider painting definition.
      */
    virtual const SliderPainter& sliderPainter() const = 0;


    /** \brief  Defines the way plane sliders positions in screen space are saved.
      *
      * \pre    <code>buffer != nullptr</code>
      */
    virtual void setSliderScreenSpaceBuffer( SliderScreenSpaceBuffer* buffer ) = 0;

    /** \brief  References the currently set slider screen space position buffer.
      */
    virtual const SliderScreenSpaceBuffer& sliderScreenSpaceBuffer() const = 0;


public slots:

    /** \brief  Supplies new intersection point and defines the plane positions.
      *
      * The \a commonPlanePoint must be set using \ref VolumeSpace "volume space coordinates".
      *
      * Also \ref base::view::Renderer::invalidate "invalidates the renderer".
      */
    virtual void setIntersection( const Carna::base::Vector& commonPlanePoint ) = 0;

    /** \brief  Sets the minimal HUV which shall be masked.
      *
      * Also \ref base::view::Renderer::invalidate "invalidates the renderer".
      */
    virtual void setMaskMinHuv( int ) = 0;

    /** \brief  Sets the maximal HUV which shall be masked.
      *
      * Also \ref base::view::Renderer::invalidate "invalidates the renderer".
      */
    virtual void setMaskMaxHuv( int ) = 0;

    /** \brief  Sets the opacity of the huv mask.
      *
      * Also \ref base::view::Renderer::invalidate "invalidates the renderer".
      */
    virtual void setHuvMaskOpacity( float ) = 0;


signals:

    /** \brief
      * Emitted when the intersection has been changed either by
      * \ref setIntersection or by user interaction.
      */
    void intersectionChanged( const Carna::base::Vector& );

}; // MPR :: AxialPlaneRenderer



}  // namespace Carna :: MPR

}  // namespace Carna

#endif // AXIALPLANERENDERER_H_6014714286
