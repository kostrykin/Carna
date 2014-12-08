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

#ifndef DVR_DIRECTVOLUMERENDERING_DETAILS_H_6014714286
#define DVR_DIRECTVOLUMERENDERING_DETAILS_H_6014714286

/** \file   DirectVolumeRenderingDetails.h
  * \brief  Defines \ref Carna::VolumeRenderings::DVR::DirectVolumeRendering::Details
  */

#include <Carna/VolumeRenderings/DVR/DirectVolumeRendering.h>
#include <Carna/VolumeRenderings/DVR/ColorMap.h>

namespace Carna
{

namespace VolumeRenderings
{

namespace DVR
{



// ----------------------------------------------------------------------------------
// DVR :: DirectVolumeRendering :: Details
// ----------------------------------------------------------------------------------

class DirectVolumeRendering::Details : public QObject
{

    Q_OBJECT

    NON_COPYABLE

    DirectVolumeRendering& self;

public:

    explicit Details( DirectVolumeRendering&, base::model::Scene& );


    base::model::Scene& model;

    /** \brief  Specifies \f$ \vec s \in \mathbb R^3 \f$ in
      *         \f$ \big( \mathrm{ grad }\, I \big)( \vec x ) = \big( I( x + \lambda \cdot s ) - I( x - \lambda \cdot s ) \big) \cdot \frac{ 1 }{ \left\| \lambda \cdot s \right\| } \f$,
      *         \f$ \lambda \in \mathbb R \f$.
      */
    const base::Vector gradientSampleDistance;

    /** \brief  Holds the GL object ID of the color map texture.
      */
    unsigned int colorMapTextureID;


    /** \brief  Holds whether lighting is performed.
      */
    bool lighting;

    /** \brief  Specifies \f$ L_0 \in \mathbb R \f$ in
      *         \f$ L( \vec x ) = L_0 \cdot \big( \max\{ 0, \langle \frac{ v_\text{light} }{ \left\| v_\text{light} \right\| }, \frac{ \mathrm{ grad }( \vec x ) }{ \left\| \mathrm{ grad }( \vec x ) \right\| } \rangle \} + L_\text{back} \cdot \max\{ 0, -\langle \frac{ v_\text{light} }{ \left\| v_\text{light} \right\| }, \frac{ \mathrm{ grad }( \vec x ) }{ \left\| \mathrm{ grad }( \vec x ) \right\| } \rangle \} - L_\text{bias} \big) \f$
      *         with light direction \f$ v_\text{light} \f$.
      */
    float lightAmplitude;

    /** \brief  Specifies \f$ L_\text{bias} \in \mathbb R \f$ in
      *         \f$ L( \vec x ) = L_0 \cdot \big( \max\{ 0, \langle \frac{ v_\text{light} }{ \left\| v_\text{light} \right\| }, \frac{ \mathrm{ grad }( \vec x ) }{ \left\| \mathrm{ grad }( \vec x ) \right\| } \rangle \} + L_\text{back} \cdot \max\{ 0, -\langle \frac{ v_\text{light} }{ \left\| v_\text{light} \right\| }, \frac{ \mathrm{ grad }( \vec x ) }{ \left\| \mathrm{ grad }( \vec x ) \right\| } \rangle \} - L_\text{bias} \big) \f$
      *         with light direction \f$ v_\text{light} \f$.
      */
    float lightBias;

    /** \brief  Specifies \f$ L_\text{back} \in \mathbb R \f$ in
      *         \f$ L( \vec x ) = L_0 \cdot \big( \max\{ 0, \langle \frac{ v_\text{light} }{ \left\| v_\text{light} \right\| }, \frac{ \mathrm{ grad }( \vec x ) }{ \left\| \mathrm{ grad }( \vec x ) \right\| } \rangle \} + L_\text{back} \cdot \max\{ 0, -\langle \frac{ v_\text{light} }{ \left\| v_\text{light} \right\| }, \frac{ \mathrm{ grad }( \vec x ) }{ \left\| \mathrm{ grad }( \vec x ) \right\| } \rangle \} - L_\text{bias} \big) \f$
      *         with light direction \f$ v_\text{light} \f$.
      */
    float lightBackFaceAmount;

    /** \brief  Holds the ray intensity threshold for picking.
      */
    float pickingSaturation;

    base::model::Object3D* positionedObject;


    /** \brief  Holds a framebuffer object, which the scene is rendered to with MRT.
      */
    std::unique_ptr< base::view::FramebufferObject > fbo;

    /** \brief  Holds the resulted rendered scene.
      */
    std::unique_ptr< base::view::RenderTexture > colorBuffer;

    /** \brief  Maps screen space coordinates to volume space coordinates.
      */
    std::unique_ptr< base::view::RenderTexture > pickingBuffer;


    ColorMap colorMap;

    ColorMap::ColorBuffer colorMapBuffer;

    bool isColorMapRebuildScheduled;

    /** \brief  Periodically rebuilds the color map if \ref DirectVolumeRendering::isColorMapRebuildScheduled
      *         is set to \c true by invoking \ref DirectVolumeRendering::rebuildColorMap.
      */
    QTimer colorMapRebuildTimer;


    void installControllerFilter();

    void uninstallControllerFilter();

private:

    class Controller;
    const std::unique_ptr< Controller > controllerFilter;


public slots:

    void rebuildColorMap();
    void scheduleColorMapRebuild();

}; // DVR :: DirectVolumeRendering :: Details



}  // namespace Carna :: VolumeRenderings :: DVR

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // DVR_DIRECTVOLUMERENDERING_DETAILS_H_6014714286
