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

#ifndef DEFAULTVOLUMEVISUALIZATION_H_6014714286
#define DEFAULTVOLUMEVISUALIZATION_H_6014714286

/** \file   DefaultVolumeVisualization.h
  * \brief  Defines \ref Carna::VolumeRenderings::DefaultVolumeVisualization.
  */

#include <Carna/VolumeRenderings/VolumeVisualization.h>

namespace Carna
{

namespace VolumeRenderings
{



// ----------------------------------------------------------------------------------
// VolumeRenderings :: DefaultVolumeVisualization
// ----------------------------------------------------------------------------------

/** \brief  Visualization that performs \ref VolumeRenderer "volume rendering".
  *
  * Refer to the \ref VolumeRenderer "volume rendering" documentation for details.
  *
  * \author Leonid Kostrykin
  * \date   26.5.11 - 18.11.13
  */
class CARNA_EXPORT DefaultVolumeVisualization : public VolumeVisualization
{

public:

    /** \brief
      * Instantiates with new instances of \ref base::controller::DefaultInteractionStrategy
      * and \ref VolumeVisualizationCameraController.
      */
    DefaultVolumeVisualization();


    /** \brief  References the mutable composed renderer.
      *
      * \pre    <code>isInitialized() == true</code>
      *
      * Specialization of \ref base::Visualization::renderer that returns only such renderers that are of class \ref DefaultVolumeRenderer.
      *
      * \since  \ref v_2_3
      */
    DefaultVolumeRenderer& renderer();

    /** \brief  References the immutable composed renderer.
      *
      * \pre    <code>isInitialized() == true</code>
      *
      * Specialization of \ref base::Visualization::renderer that returns only such renderers that are of class \ref DefaultVolumeRenderer.
      *
      * \since  \ref v_2_3
      */
    const DefaultVolumeRenderer& renderer() const;


protected:

    virtual base::view::Renderer* createRenderer( base::view::SceneProvider& provider ) override;

}; // VolumeRenderings :: DefaultVolumeVisualization



}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // DEFAULTVOLUMEVISUALIZATION_H_6014714286
