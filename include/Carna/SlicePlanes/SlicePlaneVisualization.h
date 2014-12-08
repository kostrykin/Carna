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

#ifndef SLICEPLANEVISUALIZATION_H_6014714286
#define SLICEPLANEVISUALIZATION_H_6014714286

/** \file   SlicePlaneVisualization.h
  * \brief  Defines \ref Carna::SlicePlanes::SlicePlaneVisualization.
  */

#include <Carna/base/Visualization.h>

namespace Carna
{

namespace SlicePlanes
{



// ----------------------------------------------------------------------------------
// SlicePlanes :: SlicePlaneVisualization
// ----------------------------------------------------------------------------------

/** \brief  Abstract implementation of the \ref Visualization class that is supposed
  *         to visualize any custom slice planes using the \ref SlicePlaneRenderer.
  *         This is a generalization of the \ref DefaultSlicePlaneVisualization class.
  *
  * \author Leonid Kostrykin
  * \date   26.5.11 - 2.12.13
  */
class CARNA_EXPORT SlicePlaneVisualization : public base::Visualization
{

public:
    
    /** \brief  References the mutable composed renderer.
      *
      * \pre    <code>isInitialized() == true</code>
      *
      * Specialization of \ref base::Visualization::renderer that returns only such renderers that are of class \ref SlicePlaneRenderer.
      *
      * \since  \ref v_2_3
      */
    SlicePlaneRenderer& renderer();
    
    /** \brief  References the immutable composed renderer.
      *
      * \pre    <code>isInitialized() == true</code>
      *
      * Specialization of \ref base::Visualization::renderer that returns only such renderers that are of class \ref SlicePlaneRenderer.
      *
      * \since  \ref v_2_3
      */
    const SlicePlaneRenderer& renderer() const;

}; // SlicePlanes :: SlicePlaneVisualization



}  // namespace Carna :: SlicePlanes

}  // namespace Carna

#endif // SLICEPLANEVISUALIZATION_H_6014714286
