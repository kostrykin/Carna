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

#ifndef DEFAULTAXIALPLANEVISUALIZATION_H_6014714286
#define DEFAULTAXIALPLANEVISUALIZATION_H_6014714286

/** \file   DefaultAxialPlaneVisualization.h
  * \brief  Defines \ref Carna::MPR::DefaultAxialPlaneVisualization.
  */

#include <Carna/MPR/AxialPlaneVisualization.h>
#include <Carna/MPR/AxialPlaneRenderer.h>

namespace Carna
{

namespace MPR
{



// ----------------------------------------------------------------------------------
// MPR :: DefaultAxialPlaneVisualization
// ----------------------------------------------------------------------------------

/** \brief
  * Implementation of the \ref AxialPlaneVisualization class that  displays the
  * orthogonal intersections \f$ \langle e_i,\ x \rangle = d_i, i = \{ 1, 2, 3 \} \f$
  * of the volume data set, \f$ d_i \f$ being variable.
  *
  * \see
  * The interface is documented \ref AxialPlaneVisualization here.
  *
  * \author Leonid Kostrykin
  * \date   5.3.13 - 23.06.14
  */
class CARNA_EXPORT DefaultAxialPlaneVisualization : public AxialPlaneVisualization
{

public:

    /** \brief  Instantiates.
      */
    explicit DefaultAxialPlaneVisualization( base::model::Scene& );

    /** \brief  Releases private data.
      */
    virtual ~DefaultAxialPlaneVisualization();

    
    /** \brief  References the mutable composed renderer.
      *
      * \pre    <code>isInitialized() == true</code>
      *
      * Specialization of \ref base::Visualization::renderer that returns only such
      * renderers that are of class \ref DefaultAxialPlaneRenderer.
      *
      * \since  \ref v_2_3
      */
    DefaultAxialPlaneRenderer& renderer();
    
    /** \brief  References the immutable composed renderer.
      *
      * \pre    <code>isInitialized() == true</code>
      *
      * Specialization of \ref base::Visualization::renderer that returns only such
      * renderers that are of class \ref DefaultAxialPlaneRenderer.
      *
      * \since  \ref v_2_3
      */
    const DefaultAxialPlaneRenderer& renderer() const;


protected:

    virtual base::view::Renderer* createRenderer( base::view::SceneProvider& provider ) override;

}; // MPR :: DefaultAxialPlaneVisualization



}  // namespace Carna :: MPR

}  // namespace Carna

#endif // DEFAULTAXIALPLANEVISUALIZATION_H_6014714286
