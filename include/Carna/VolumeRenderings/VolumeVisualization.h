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

#ifndef VOLUMEVISUALIZATION_H_6014714286
#define VOLUMEVISUALIZATION_H_6014714286

/** \file   VolumeVisualization.h
  * \brief  Defines \ref Carna::VolumeRenderings::VolumeVisualization.
  */

#include <Carna/base/Visualization.h>

namespace Carna
{

namespace VolumeRenderings
{



// ----------------------------------------------------------------------------------
// VolumeRenderings :: VolumeVisualization
// ----------------------------------------------------------------------------------

/** \brief  Abstract implementation of the \ref Visualization class that is supposed
  *         to perform \ref VolumeRenderer "volume rendering" using the \ref VolumeRenderer.
  *         This is a generalization of the \ref DefaultVolumeVisualization class.
  *
  * \author Leonid Kostrykin
  * \date   26.5.11 - 2.12.13
  */
class CARNA_EXPORT VolumeVisualization : public base::Visualization
{

public:

    /** \brief
      * Instantiates with new instances of \ref base::controller::DefaultInteractionStrategy
      * and \ref VolumeVisualizationCameraController.
      */
    VolumeVisualization();
    
    /** \brief
      * Instantiates with \a initialController.
      */
    VolumeVisualization( base::controller::InteractionStrategy* initialController );


    /** \brief  References the mutable composed renderer.
      *
      * \pre    <code>isInitialized() == true</code>
      *
      * Specialization of \ref base::Visualization::renderer that returns only such renderers that are of class \ref VolumeRenderer.
      *
      * \since  \ref v_2_3
      */
    VolumeRenderer& renderer();

    /** \brief  References the immutable composed renderer.
      *
      * \pre    <code>isInitialized() == true</code>
      *
      * Specialization of \ref base::Visualization::renderer that returns only such renderers that are of class \ref VolumeRenderer.
      *
      * \since  \ref v_2_3
      */
    const VolumeRenderer& renderer() const;

}; // VolumeRenderings :: VolumeVisualization



}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // VOLUMEVISUALIZATION_H_6014714286
