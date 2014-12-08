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

#include <Carna/base/view/glew.h>
#include <Carna/base/view/glError.h>
#include <Carna/base/view/SceneProvider.h>
#include <Carna/base/view/SceneProviderDetails.h>
#include <Carna/base/view/DefaultObject3DPaintingRoutine.h>
#include <Carna/base/view/Object3DPainter.h>
#include <Carna/base/CarnaException.h>
#include <Carna/base/Transformation.h>
#include <Carna/base/VisualizationEnvironment.h>
#include <QGLFramebufferObject>
#include <QGLPixelBuffer>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// getMax3DTextureSize
// ----------------------------------------------------------------------------------

static unsigned int getMax3DTextureSize()
{
    int max3dTextureSize_;
    {
        QGLPixelBuffer pbuffer( 8, 8 );
        pbuffer.makeCurrent();

        CARNA_GLEW_INIT;

        glGetIntegerv( GL_MAX_3D_TEXTURE_SIZE, &max3dTextureSize_ );
        qDebug() << "GL_MAX_3D_TEXTURE_SIZE: " << max3dTextureSize_;

        pbuffer.doneCurrent();
    }
    return static_cast< unsigned int >( max3dTextureSize_ );
}



// ----------------------------------------------------------------------------------
// SceneProvider
// ----------------------------------------------------------------------------------

SceneProvider::SceneProvider
    ( const model::Scene& scene
    , QObject* parent )

    : QObject( parent )
    , scene( scene )
    , pimpl( new Details( *this ) )
{
}


SceneProvider::~SceneProvider()
{
}


unsigned int SceneProvider::max3DTextureSize()
{
    static const unsigned int max3dTextureSize = getMax3DTextureSize();
    return max3dTextureSize;
}



// ----------------------------------------------------------------------------------
// SceneProvider :: ResourceClient
// ----------------------------------------------------------------------------------

SceneProvider::ResourceClient::ResourceClient( VisualizationEnvironment& environment, SceneProvider& provider )
    : provider( provider )
    , environment( environment )
    , volumeTextureID( [&]()->unsigned int
        {
            environment.activateContext();

            CARNA_GLEW_INIT;

            return provider.pimpl->acquireVolumeTexture();
        }
        () )
    , maskTextureID( [&]()->unsigned int
        {
            return provider.pimpl->acquireMaskTexture();
        }
        () )
{
    CARNA_ASSERT( volumeTextureID > 0 );
    CARNA_ASSERT( maskTextureID > 0 );
    CARNA_ASSERT( volumeTextureID != maskTextureID );
}


SceneProvider::ResourceClient::~ResourceClient()
{
    environment.activateContext();
    provider.pimpl->releaseMaskTexture();
    provider.pimpl->releaseVolumeTexture();
}


void SceneProvider::ResourceClient::updateContext()
{
    if( provider.pimpl->isMaskTextureInvalidated() )
    {
        provider.pimpl->updateMaskTexture();
    }
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
