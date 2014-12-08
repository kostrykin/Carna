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

#ifndef DVR_DIRECTVOLUMERENDERINGCONTROLLERUI_H_6014714286
#define DVR_DIRECTVOLUMERENDERINGCONTROLLERUI_H_6014714286

/** \file   DirectVolumeRenderingControllerUI.h
  * \brief  Defines \ref Carna::VolumeRenderings::DVR::DirectVolumeRenderingControllerUI
  */

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>
#include <Carna/base/Transformation.h>
#include <Carna/VolumeRenderings/DVR/ColorMap.h>
#include <QTimer>
#include <QWidget>

class QCheckBox;
class QDoubleSpinBox;

namespace Carna
{

namespace VolumeRenderings
{

namespace DVR
{



// ----------------------------------------------------------------------------------
// DVR :: DirectVolumeRenderingControllerUI
// ----------------------------------------------------------------------------------

class DirectVolumeRenderingControllerUI : public QWidget
{

    Q_OBJECT

    NON_COPYABLE

public:

    /** \brief  Instantiates.
      */
    explicit DirectVolumeRenderingControllerUI( DirectVolumeRendering& );

    /** \brief  Releases the color map texture.
      */
    virtual ~DirectVolumeRenderingControllerUI();


private:

    DirectVolumeRendering& mode;


    QCheckBox* const cbLighting;

    /** \brief  Configures \ref DirectVolumeRendering::lightAmplitude.
      */
    QDoubleSpinBox* const sbLightAmplitude;

    /** \brief  Configures \ref DirectVolumeRendering::lightBias.
      */
    QDoubleSpinBox* const sbLightBias;

    /** \brief  Configures \ref DirectVolumeRendering::lightBackFaceAmount.
      */
    QDoubleSpinBox* const sbLightBackFaceAmount;


    /** \brief  Configures \ref DirectVolumeRendering::pickingSaturation.
      */
    QDoubleSpinBox* const sbPickingSaturation;

    /** \brief  Defines which object is to be moved by picking.
      */
    base::qt::Object3DChooser* const cbPickingRepresentation;

    /** \brief  References the color map configuration widget.
      */
    ColorMapEditor* const colorMapEditor;


private slots:

    /** \brief  Prompts the user for an XML output file and saves current channels there.
      */
    void saveColorConfig();

    /** \brief  Prompts the user for an XML input file and loads channels from there.
      */
    void loadColorConfig();


    void updateLightingUI();

    void updateColorMap();

}; // DVR :: DirectVolumeRendering



}  // namespace Carna :: VolumeRenderings :: DVR

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // DVR_DIRECTVOLUMERENDERINGCONTROLLERUI_H_6014714286
