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

#ifndef VOLUMEVISUALIZATIONCAMERACONTROLLER_H_6014714286
#define VOLUMEVISUALIZATIONCAMERACONTROLLER_H_6014714286

/** \file   VolumeVisualizationCameraController.h
  * \brief  Defines \ref Carna::VolumeRenderings::VolumeVisualizationCameraController.
  */

#include <Carna/base/controller/DefaultCameraController.h>

namespace Carna
{

namespace VolumeRenderings
{



// ----------------------------------------------------------------------------------
// VolumeRenderings :: VolumeVisualizationCameraController
// ----------------------------------------------------------------------------------
    
/** \brief
  * Extension of the \ref DefaultCameraController that is optimized for controlling a
  * camera in a \ref VolumeVisualization "volume rendering context".
  *
  * Particularly, this controller is capable of reducing the quality of the associated
  * visualization during it's interaction with the user, e.g. when the camera shifted
  * or rotated. The quality reduction is specified by a relative value between 0 and 1:
  * A value of 1, also represented by \ref NO_QUALITY_REDUCTION, indicates that no
  * quality reduction is performed during user interaction. The field
  * \ref DEFAULT_QUALITY_REDUCTION holds the quality reduction configured by default.
  * Refer to \ref setQualityReduction and \ref qualityReduction.
  *
  * \author Leonid Kostrykin
  * \date   21.3.13
  */
class CARNA_EXPORT VolumeVisualizationCameraController : public base::controller::DefaultCameraController
{

    Q_OBJECT

public:

    /** \brief  Instantiates.
      */
    explicit VolumeVisualizationCameraController( VolumeVisualization& module, QObject* parent = nullptr );

    /** \brief  Destroys itself.
      */
    virtual ~VolumeVisualizationCameraController();

    /** \brief  References the controlled volume rendering module.
      */
    VolumeVisualization& module;

    // ------------------------------------------------------------------------------
    // InteractionStrategy Interface
    // ------------------------------------------------------------------------------

    virtual void event( base::Visualization& sourceModule, QEvent* event ) override;

    // ------------------------------------------------------------------------------
    // Quality Reduction Control
    // ------------------------------------------------------------------------------

    /** \brief  Corresponds to no quality reduction being performed during user interaction.
      */
    const static double NO_QUALITY_REDUCTION;
    
    /** \brief  Holds the quality reduction performed by default during user interaction.
      */
    const static double DEFAULT_QUALITY_REDUCTION;
    
    /** \brief  Tells the quality reduction being performed during user interaction.
      *
      * \post   <code>qualityReduction() >= 0.0 && qualityReduction() <= 1.0</code>
      *
      * \see
      * Special values are held by the constants \ref DEFAULT_QUALITY_REDUCTION and \ref NO_QUALITY_REDUCTION.
      */
    double qualityReduction() const;
    
    /** \brief  Configures the quality reduction to be performed during user interaction.
      *
      * \pre    <code>qualityReduction >= 0.0 && qualityReduction <= 1.0</code>
      *
      * \see
      * Special values are held by the constants \ref DEFAULT_QUALITY_REDUCTION and \ref NO_QUALITY_REDUCTION.
      */
    void setQualityReduction( double qualityReduction );


private:

    class Details;
    const std::unique_ptr< Details > pimpl;

}; // VolumeRenderings :: VolumeVisualizationCameraController



}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // VOLUMEVISUALIZATIONCAMERACONTROLLER_H_6014714286
