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

#ifndef DEFAULTSLICEPLANERENDERER_H_6014714286
#define DEFAULTSLICEPLANERENDERER_H_6014714286

/** \file   DefaultSlicePlaneRenderer.h
  * \brief  Defines \ref Carna::SlicePlanes::DefaultSlicePlaneRenderer.
  */

#include <Carna/SlicePlanes/SlicePlaneRenderer.h>
#include <Carna/SlicePlanes/SlicePlaneVisualization.h>

namespace Carna
{

namespace SlicePlanes
{



// ----------------------------------------------------------------------------------
// SlicePlanes :: DefaultSlicePlaneRenderer
// ----------------------------------------------------------------------------------

/** \brief  \ref Renderer adapter for the \ref SlicePlane class.
  *
  * The render visualizes any custom slice planes.
  * It is part of the \ref SlicePlaneVisualization module.
  *
  * \image html SlicePlaneView01.png "visualization of any custom cross section planes"
  *
  * \see
  * The \ref SlicePlaneControllerUI class provides an easy-to-use user interface for
  * the purpose of configuring this renderer.
  *
  * \author Leonid Kostrykin
  * \date   26.5.11 - 18.11.13
  */
class CARNA_EXPORT DefaultSlicePlaneRenderer : public SlicePlaneRenderer
{

    Q_OBJECT

    NON_COPYABLE

public:

    /** \brief  Instantiates.
      */
    DefaultSlicePlaneRenderer
        ( base::Visualization& module
        , base::view::SceneProvider& provider
        , QObject* parent = nullptr );

    /** \brief  Releases acquired GL resources.
      */
    virtual ~DefaultSlicePlaneRenderer();


    virtual const base::Vector& getNormal() const override;

    virtual double getDistance() const override;

    virtual float getBorderWidth() const override;

    virtual const SlicePlane& getPlane() const override;


public slots:

    virtual bool setPlane
        ( const Carna::base::Vector& normal
        , const double& distance
        , float border = DEFAULT_BORDER_WIDTH ) override;


protected:

    virtual void renderColor() override;

    /** \brief  Does nothing.
      */
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


private:

    class Details;
    const std::unique_ptr< Details > pimpl;

}; // SlicePlanes :: DefaultSlicePlaneRenderer



}  // namespace Carna :: SlicePlanes

}  // namespace Carna

#endif // DEFAULTSLICEPLANERENDERER_H_6014714286
