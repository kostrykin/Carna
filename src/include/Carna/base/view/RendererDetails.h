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

#ifndef RENDERER_DETAILS_H_6014714286
#define RENDERER_DETAILS_H_6014714286

/** \file   RendererDetails.h
  * \brief  Defines \ref Carna::base::view::Renderer::Details
  */

#include <Carna/base/view/Renderer.h>
#include <Carna/base/view/SceneProvider.h>

namespace Carna
{

namespace base
{

namespace view
{

class RendererSharedData;



// ----------------------------------------------------------------------------------
// Renderer :: Details
// ----------------------------------------------------------------------------------

class Renderer :: Details : public QObject, public SceneProvider::ResourceClient
{

    NON_COPYABLE

    Q_OBJECT

    Renderer& self;

public:

    Details( Renderer& self, Visualization& module );

    virtual ~Details();


    Visualization& module;

    std::unique_ptr< Association< Camera > > camera;


    RendererSharedData& sharedData;


    unsigned int renderingLock;

    unsigned int targetWidth, targetHeight;

    std::unique_ptr< Association< Object3DPainter > > objectPainter;


    mutable struct
    {
        Transformation modelview;
        Transformation projection;

        bool valid;
    }
    pickingEnviroment;


    std::unique_ptr< Renderer::RenderMode > renderMode;


    Transformation sceneTrafo;

    Transformation cameraTrafo;

    Transformation volumeViewTrafo;

    Transformation projectionTrafo;


    void computeTransformations();


    unsigned int paintRecursionLevel;


private slots:

    void processObjectsUpdate( const Carna::base::model::Object3DEvent& );

}; // Renderer :: Details



// ----------------------------------------------------------------------------------
// Renderer :: RenderMode :: Details
// ----------------------------------------------------------------------------------

class Renderer :: RenderMode :: Details
{

public:

    Details();

    ~Details();

    std::unique_ptr< FramebufferObject > fbo;

};



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // RENDERER_DETAILS_H_6014714286
