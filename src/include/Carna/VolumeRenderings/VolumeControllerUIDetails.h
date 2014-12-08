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

#ifndef VOLUMECONTROLLERUI_DETAILS_H_6014714286
#define VOLUMECONTROLLERUI_DETAILS_H_6014714286

/** \file   VolumeControllerUIDetails.h
  * \brief  Defines \ref Carna::VolumeRenderings::VolumeControllerUI::Details.
  */

#include <Carna/VolumeRenderings/VolumeControllerUI.h>

class QLineEdit;
class QTabWidget;
class QCheckBox;
class QSpinBox;
class QComboBox;

namespace Carna
{

namespace VolumeRenderings
{



// ----------------------------------------------------------------------------------
// VolumeControllerUI :: Details
// ----------------------------------------------------------------------------------

/** \brief  PIMPL-implementation of \ref VolumeControllerUI
  *
  * \author Leonid Kostrykin
  * \date   8.8.11 - 26.10.12
  */
class CARNA_EXPORT VolumeControllerUI::Details : public QObject
{

    Q_OBJECT

public:

    Details
        ( VolumeRenderer& renderer
        , base::model::Scene& model );


    static const unsigned int CONTROLLER_MARGINS_TOP = 5;


    base::model::Scene& model;

    VolumeRenderer& renderer;

    QCheckBox* cbReduceQuality;

    QComboBox* cbColorPostProcessing;

    QCheckBox* cbMask;

    QCheckBox* cbFlipHorizontal;

    QCheckBox* cbFlipVertical;

    QSpinBox* sbRotation2D;

    base::qt::RotationEdit* rotationOffset;

    base::qt::Object3DChooser* rotationOffsetCenter;

    QTabWidget* toolbox;

    int mode_cfg_widget_index;


public slots:

    void syncCameraSettings();


private slots:

    void updateModeConfiguration( Carna::VolumeRenderings::VolumeRendererMode& mode );

    void setModeByIndex( int );

    void setRotationCenter();

    void setCameraQualityReduction( bool );

    void setColorPostProcessing( int );

}; // VolumeControllerUI :: Details



}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // VOLUMECONTROLLERUI_DETAILS_H_6014714286
