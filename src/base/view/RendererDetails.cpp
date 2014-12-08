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

#include <Carna/base/view/RendererDetails.h>
#include <Carna/base/view/RendererSharedData.h>
#include <Carna/base/view/DefaultObject3DPainter.h>
#include <Carna/base/view/SceneProvider.h>
#include <Carna/base/view/Camera.h>
#include <Carna/base/view/DefaultCamera.h>
#include <Carna/base/view/FramebufferObject.h>
#include <Carna/base/view/Monoscopic.h>
#include <Carna/base/model/Object3DEvent.h>
#include <Carna/base/model/Scene.h>
#include <Carna/base/Composition.h>
#include <Carna/base/Flags.h>
#include <Carna/base/Visualization.h>
#include <QTimer>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// Renderer :: Details
// ----------------------------------------------------------------------------------

Renderer::Details::Details
    ( Renderer& self, Visualization& module )

    : SceneProvider::ResourceClient( module.environment(), self.provider )
    , self( self )
    , module( module )
    , camera( new Composition< Camera >( new DefaultCamera( self.provider.scene ) ) )
    , sharedData( RendererSharedData::acquire() )
    , renderingLock( 0 )
    , targetWidth( 1 )
    , targetHeight( 1 )
    , objectPainter( new Composition< Object3DPainter >( new DefaultObject3DPainter( self.provider.scene ) ) )
    , renderMode( new Monoscopic( self ) )
    , paintRecursionLevel( 0 )
{
    pickingEnviroment.valid = false;

    connect
        ( &self.provider.scene
        , SIGNAL( objectsChanged( const Carna::base::model::Object3DEvent& ) )
        , this
        , SLOT( processObjectsUpdate( const Carna::base::model::Object3DEvent& ) ) );

    connect
        ( camera->get()
        , SIGNAL( viewChanged() )
        , &self
        , SLOT( invalidate() ) );
}


Renderer::Details::~Details()
{
    RendererSharedData::release();
}


void Renderer::Details::computeTransformations()
{
    self.camera().fetchProjectionMatrix( self, projectionTrafo );
    self.camera().fetchCameraMatrix ( self, cameraTrafo  );

    self.fetchSceneMatrix( sceneTrafo, cameraTrafo, projectionTrafo );

    volumeViewTrafo = cameraTrafo * sceneTrafo;
}


void Renderer::Details::processObjectsUpdate( const model::Object3DEvent& ev )
{
    if( ev.mightAffect( Flags< model::Object3DEvent::Feature >( model::Object3DEvent::existence )
        .or( model::Object3DEvent::position )
        .or( model::Object3DEvent::rotation )
        .or( model::Object3DEvent::shape ) ) )
    {
        QTimer::singleShot( 0, &self, SLOT( invalidate() ) );
    }
}



// ----------------------------------------------------------------------------------
// Renderer :: RenderMode :: Details
// ----------------------------------------------------------------------------------

Renderer::RenderMode::Details::Details()
{
}


Renderer::RenderMode::Details::~Details()
{
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
