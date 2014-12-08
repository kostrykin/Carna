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

#ifndef DEFAULTSLICEPLANEVISUALIZATION_H_6014714286
#define DEFAULTSLICEPLANEVISUALIZATION_H_6014714286

/** \file   DefaultSlicePlaneVisualization.h
  * \brief  Defines \ref Carna::SlicePlanes::DefaultSlicePlaneVisualization.
  */

#include <Carna/SlicePlanes/SlicePlaneVisualization.h>

namespace Carna
{

namespace SlicePlanes
{



// ----------------------------------------------------------------------------------
// SlicePlanes :: DefaultSlicePlaneVisualization
// ----------------------------------------------------------------------------------

/** \brief  Visualizes any custom slice planes using the \ref SlicePlaneRenderer.
  *
  * \image html SlicePlaneView01.png "visualization of any custom cross section planes"
  *
  * \author Leonid Kostrykin
  * \date   26.5.11 - 18.11.13
  */
class CARNA_EXPORT DefaultSlicePlaneVisualization : public SlicePlaneVisualization
{

public:
    
    /** \brief  References the mutable composed renderer.
      *
      * \pre    <code>isInitialized() == true</code>
      *
      * Specialization of \ref base::Visualization::renderer that returns only such renderers that are of class \ref DefaultSlicePlaneRenderer.
      *
      * \since  \ref v_2_3
      */
    DefaultSlicePlaneRenderer& renderer();
    
    /** \brief  References the immutable composed renderer.
      *
      * \pre    <code>isInitialized() == true</code>
      *
      * Specialization of \ref base::Visualization::renderer that returns only such renderers that are of class \ref DefaultSlicePlaneRenderer.
      *
      * \since  \ref v_2_3
      */
    const DefaultSlicePlaneRenderer& renderer() const;

protected:

    virtual base::view::Renderer* createRenderer( base::view::SceneProvider& provider ) override;

}; // SlicePlanes :: DefaultSlicePlaneVisualization



}  // namespace Carna :: SlicePlanes

}  // namespace Carna

#endif // DEFAULTSLICEPLANEVISUALIZATION_H_6014714286
