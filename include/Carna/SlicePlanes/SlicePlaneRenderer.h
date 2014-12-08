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

#ifndef SLICEPLANERENDERER_H_6014714286
#define SLICEPLANERENDERER_H_6014714286

/** \file   SlicePlaneRenderer.h
  * \brief  Defines \ref Carna::SlicePlanes::SlicePlaneRenderer.
  */

#include <Carna/base/view/Renderer.h>
#include <Carna/SlicePlanes/SlicePlaneVisualization.h>

namespace Carna
{

namespace SlicePlanes
{



// ----------------------------------------------------------------------------------
// SlicePlanes :: SlicePlaneRenderer
// ----------------------------------------------------------------------------------
    
/** \brief  Abstract \ref Renderer specialization that is supposed to utilize the \ref SlicePlane class for rendering slice planes.
  *         This is a generalization of the \ref DefaultSlicePlaneRenderer class.
  *
  * \see
  * The \ref SlicePlaneControllerUI class provides an easy-to-use user interface for
  * the purpose of configuring this renderer.
  *
  * \author Leonid Kostrykin
  * \date   18.11.2013
  */
class CARNA_EXPORT SlicePlaneRenderer: public base::view::Renderer
{

    Q_OBJECT

    NON_COPYABLE

public:

    /** \brief  Holds default inner border width.
      */
    const static float DEFAULT_BORDER_WIDTH;


    /** \brief  Instantiates.
      */
    SlicePlaneRenderer
        ( base::Visualization& module
        , base::view::SceneProvider& provider
        , QObject* parent = nullptr );

    /** \brief  Does nothing.
      */
    virtual ~SlicePlaneRenderer();


    /** \brief  Tells \f$\vec n : \langle \vec x, \frac{\vec n}{\| \vec n \|} \rangle = d\f$
      */
    virtual const base::Vector& getNormal() const = 0;

    /** \brief  Tells \f$d : \langle \vec x, \frac{\vec n}{\| \vec n \|} \rangle = d\f$
      */
    virtual double getDistance() const = 0;

    /** \brief  Tells the width of inner border drawn.
      */
    virtual float getBorderWidth() const = 0;

    /** \brief  References the currently visualized slice plane.
      */
    virtual const SlicePlane& getPlane() const = 0;


public slots:

    /** \brief  Specifies slice plane.
      *
      * The plane is defined by
      * \f$ \vec x : \langle \vec x, \frac{\vec n}{\| \vec n \|} \rangle = d\f$,
      * where \f$ \vec n \f$ is \a normal and \f$ d \f$ is \a distance.
      *
      * \pre    <code>Carna::base::isEqual( normal.norm(), 0. ) == false</code>
      *
      * \param  normal      is normalized plane normal vector \f$ \in \mathbb{R}^3 \f$ in \ref VolumeSpace "volume space coordinates".
      * \param  distance    of plane to origin \f$ \in [0,1] \f$ in \ref VolumeSpace "volume space coordinates".
      * \param  border      specifies width of inner border to draw.
      *
      * \returns    \c true if computation was successful.
      */
    virtual bool setPlane
        ( const Carna::base::Vector& normal
        , const double& distance
        , float border = DEFAULT_BORDER_WIDTH ) = 0;

}; // SlicePlanes :: SlicePlaneRenderer



}  // namespace Carna :: SlicePlanes

}  // namespace Carna

#endif // SLICEPLANERENDERER_H_6014714286
