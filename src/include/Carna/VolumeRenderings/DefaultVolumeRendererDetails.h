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

#ifndef DEFAULTVOLUMERENDERER_DETAILS_H_6014714286
#define DEFAULTVOLUMERENDERER_DETAILS_H_6014714286

/** \file   DefaultVolumeRendererDetails.h
  * \brief  Defines \ref Carna::VolumeRenderings::DefaultVolumeRenderer::Details.
  */

#include <Carna/VolumeRenderings/DefaultVolumeRenderer.h>
#include <Carna/VolumeRenderings/DefaultVolumeRendererModeFrame.h>
#include <Carna/base/model/Scene.h>
#include <Carna/base/model/Volume.h>
#include <Carna/base/view/SceneProvider.h>
#include <Carna/base/view/RenderTexture.h>

namespace Carna
{

namespace VolumeRenderings
{



// ----------------------------------------------------------------------------------
// DefaultVolumeRenderer :: Details
// ----------------------------------------------------------------------------------

class DefaultVolumeRenderer::Details : public QObject
{

    Q_OBJECT

    DefaultVolumeRenderer& self;

public:

    explicit Details( DefaultVolumeRenderer& self );

    virtual ~Details();


    /** \brief  Holds the model rotation offset.
      */
    base::Transformation rotationOffset;
    
    /** \brief  Holds the model rotation offset's center in volume units.
      */
    base::Vector rotationCenter;

    /** \brief  Holds number of samples made per pixel.
      */
    unsigned int sampleCount;

    /** \brief  Aggregates the current \ref VolumeRendererMode encapsulated by an
      *         instance of \ref DefaultVolumeRendererModeFrame.
      *
      * This is done for the sake of simple GL state management following the RAII
      * paradigm.
      */
    std::unique_ptr< DefaultVolumeRendererModeFrame > modeFrame;

    DefaultVolumeRenderer::ColorOperation colorPostProcessing;
    
    /** \brief  Holds whether a circular mask is blended after rendering is done.
      */
    bool mask;

    bool flippedHorizontal;

    bool flippedVertical;

    unsigned int rotation2D;

    /** \brief  Holds an exclusive framebuffer object.
      */
    std::unique_ptr< base::view::FramebufferObject > fbo;

    /** \brief  Holds an exclusive texture which is bound to \ref fbo as color component.
      *
      * The main rendering is done to that texture. Afterwards drawing is performed
      * from here to the framebuffer next in framebuffer binding stack, performing
      * post processing.
      *
      * \see    FramebufferObject::MinimalBinding::bindings for what the framebuffer
      *         binding stack is.
      */
    std::unique_ptr< base::view::RenderTexture > pp_buffer;

    /** \brief  Holds the front faces buffer.
      */
    std::unique_ptr< base::view::RenderTexture > ff_buffer;
    
    /** \brief  Holds the back faces buffer.
      */
    std::unique_ptr< base::view::RenderTexture > bf_buffer;


    /** \brief	Holds all modes which belong to this view.
      */
    DefaultVolumeRenderer::VolumeRendererModes modes;


    /** \brief  Holds the model view transformation that is currently used for
      *         painting \ref Object3D "3D objects".
      */
    base::Transformation objectsModelView;


    float xMin, xMax, yMin, yMax, zMin, zMax;

    bool lazyModeInitializationScheduled;

}; // DefaultVolumeRenderer :: Details



}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // DEFAULTVOLUMERENDERER_DETAILS_H_6014714286
