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

#ifndef AXIALPLANE_DEFAULTAXIALPLANERENDERER_H_6014714286
#define AXIALPLANE_DEFAULTAXIALPLANERENDERER_H_6014714286

/** \file   DefaultAxialPlaneRenderer.h
  * \brief  Defines \ref Carna::MPR::DefaultAxialPlaneRenderer.
  */

#include <Carna/MPR/AxialPlaneRenderer.h>
#include <Carna/base/noncopyable.h>
#include <Carna/base/Vector3.h>
#include <Carna/Carna.h>

namespace Carna
{

namespace MPR
{



// ----------------------------------------------------------------------------------
// MPR :: DefaultAxialPlaneRenderer
// ----------------------------------------------------------------------------------

/** \brief
  * Displays the orthogonal cross-sections \f$ \langle e_i,\ x \rangle = d_i, i = \{ 1, 2, 3 \} \f$
  * of the volume data set, \f$ d_i \f$ being variable.
  *
  * The image below shows an exemplary rendering result of this class.
  *
  * \image html AxialPlaneRenderer01.png Exemplary rendering result of the \c %DefaultAxialPlaneRenderer class
  *
  * \see
  * This renderer is part of the \ref DefaultAxialPlaneVisualization %module.
  *
  * The rendered perspective is fully determined by the utilized camera. This means in
  * particular that the camera determines which planes actually are visible.
  *
  * The <em>intersection point</em> \f$ ( d_1, d_2, d_3 )^\mathrm T \f$
  * defines the position of the planes. It must be \ref setIntersection "set" using
  * \ref VolumeSpace "volume space coordinates". The intersection point is the one
  * point that is common to all three cross-sectional planes.
  *
  * \see
  * Also refer to the \ref AxialPlaneRenderer "interface documentation" for \ref AxialPlaneRendererCustomizations "details on how to customize this renderer's behavior".
  *
  * \author Leonid Kostrykin
  * \date   26.9.11 - 18.11.13
  */
class CARNA_EXPORT DefaultAxialPlaneRenderer : public AxialPlaneRenderer
{

    Q_OBJECT

    NON_COPYABLE

public:

    /** \brief  Instantiates.
      */
    DefaultAxialPlaneRenderer
        ( base::Visualization& module
        , base::view::SceneProvider& provider
        , QWidget* parent = nullptr );

    /** \brief  Releases previously acquired resources.
      */
    virtual ~DefaultAxialPlaneRenderer();


    virtual const base::Vector& intersection() const override;

    virtual int minimalMaskHuv() const override;

    virtual int maximalMaskHuv() const override;


    virtual void setSliderPainter( SliderPainter* painter ) override;

    virtual SliderPainter& sliderPainter();

    virtual const SliderPainter& sliderPainter() const override;


    virtual void setSliderScreenSpaceBuffer( SliderScreenSpaceBuffer* buffer ) override;

    virtual const SliderScreenSpaceBuffer& sliderScreenSpaceBuffer() const override;


protected:

    virtual void renderColor() override;

    virtual void renderDepth() override;
    
    /** \brief  Queries the \ref CoordinateSystems "scene matrix".
      *
      * This view's scene matrix is defined by:
      *
      * \f[ S = S_0 \cdot M_0 \cdot M_R \f]
      *
      * with
      *
      * \f[
      * M_R =
      * \begin{pmatrix}
      * 1 & 0 & 0 & -0.5 \\
      * 0 & 1 & 0 & -0.5 \\
      * 0 & 0 & 1 & -0.5 \\
      * 0 & 0 & 0 & 0 \\
      * \end{pmatrix}
      * \f]
      */
    virtual void fetchSceneMatrix
        ( base::Transformation& sceneMatrix
        , const base::Transformation& cameraMatrix
        , const base::Transformation& projectionMatrix ) const override;


public slots:

    virtual void setIntersection( const Carna::base::Vector& commonPlanePoint ) override;

    virtual void setMaskMinHuv( int ) override;

    virtual void setMaskMaxHuv( int ) override;

    virtual void setHuvMaskOpacity( float ) override;


signals:

    void intersectionChanged( const Carna::base::Vector& );


private:

    class Details;
    const std::unique_ptr< Details > pimpl;

}; // MPR :: DefaultAxialPlaneRenderer



}  // namespace Carna :: MPR

}  // namespace Carna

#endif // AXIALPLANE_DEFAULTAXIALPLANERENDERER_H_6014714286
