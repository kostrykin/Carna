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

#ifndef XRAYCONTROLLERUI_H_6014714286
#define XRAYCONTROLLERUI_H_6014714286

/** \file   XRayControllerUI.h
  * \brief  Defines \ref Carna::VolumeRenderings::DRR::XRayControllerUI.
  */

#include <Carna/Carna.h>
#include <QWidget>

class QDoubleSpinBox;
class QSpinBox;

namespace Carna
{

namespace VolumeRenderings
{

namespace DRR
{



// ----------------------------------------------------------------------------------
// XRayControllerUI
// ----------------------------------------------------------------------------------

/** \brief  Represents controller widget for the \ref XRay "x-ray volume rendering mode".
  *
  * \author Leonid Kostrykin
  * \date   16.4.12 - 14.9.12
  */
class XRayControllerUI : public QWidget
{

    Q_OBJECT

public:

    /** \brief	Instantiates.
      */
    explicit XRayControllerUI( XRay& view, QWidget* parent = 0 );

    XRay& view;


public slots:

    /** \brief  Sets displayed \f$\mu_{ \text{water} }\f$ value.
      */
    void setWaterAttenuation( double mu_water );

    /** \brief  Sets displayed \f$\text{brightness}_0\f$ value.
      */
    void setBrightness( double brightness );

    /** \brief  Sets displayed \f$a_{ threshold }\f$ value.
      */
    void setLowerThreshold( int lower_threshold );

    void setUpperThreshold( int upper_threshold );

    void setUpperMultiplier( double upper_multiplier );

    void setWindowLevel( double window_level );

    void setWindowWidth( double window_width );

    void setMaskContrast( double mask_contrast );


private slots:

    void updateMaskAcquistionControl();


private:

    QDoubleSpinBox* const sbWaterAttenuation;

    QDoubleSpinBox* const sbBrightness;

    QSpinBox* const sbLowerThreshold;

    QSpinBox* const sbUpperThreshold;

    QDoubleSpinBox* const sbUpperMultiplier;

    QDoubleSpinBox* const sbWindowLevel;

    QDoubleSpinBox* const sbWindowWidth;

    QDoubleSpinBox* const sbMaskContrast;

    QPushButton* const buReleaseMask;

}; // XRayControllerUI



}  // namespace Carna :: VolumeRenderings :: DRR

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // XRAYCONTROLLERUI_H_6014714286
