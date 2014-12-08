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

#include <Carna/base/VisualizationEnvironment.h>
#include <Carna/base/CarnaException.h>
#include <Carna/base/model/Scene.h>
#include <Carna/base/model/Volume.h>
#include <Carna/base/view/ShaderBundle.h>
#include <Carna/base/view/glew.h>
#include <Carna/base/view/SceneProvider.h>
#include <Carna/SlicePlanes/SlicePlaneDetails.h>
#include <QGLFramebufferObject>
#include <QGLPixelBuffer>
#include <QGLContext>

namespace Carna
{

namespace SlicePlanes
{



// ----------------------------------------------------------------------------------
// getPixelBufferContext
// ----------------------------------------------------------------------------------

static const QGLContext* getPixelBufferContext( QGLPixelBuffer* pbuffer )
{
    pbuffer->makeCurrent();
    const QGLContext* const context = QGLContext::currentContext();;
    return context;
}



// ----------------------------------------------------------------------------------
// SlicePlane :: Details
// ----------------------------------------------------------------------------------

const double SlicePlane::Details::WINDOW_SIZE = 2 * sqrt( 3. );


SlicePlane::Details::Details
    ( SlicePlane& self
    , const base::model::Scene& model )

    : self( self )
    , myTextureSize( base::Vector3ui
        ( std::min( base::view::SceneProvider::max3DTextureSize(), model.volume().size.x )
        , std::min( base::view::SceneProvider::max3DTextureSize(), model.volume().size.y )
        , std::min( base::view::SceneProvider::max3DTextureSize(), model.volume().size.z ) ) )
    , fboSizeXY( static_cast< unsigned int >( WINDOW_SIZE * std::max( std::max( myTextureSize.x, myTextureSize.y ), myTextureSize.z ) ) )
    , fbo( nullptr )
    , valid( false )
{
}


SlicePlane::Details::~Details()
{
}


void SlicePlane::Details::initialize()
{
    CARNA_ASSERT_EX
        ( getVolumeTextureID() != 0
        , "volume texture ID must not be zero" );

    // activate context

    this->activateContext();

    // acquire GLEW

    CARNA_GLEW_INIT;

    // acquire resources

    fbo.reset( new QGLFramebufferObject( fboSizeXY, fboSizeXY ) );

    // initialize shaders for windowing

    shader.reset( new base::view::ShaderBundle( "sliceplane-windowing" ) );
}



// ----------------------------------------------------------------------------------
// SlicePlane :: ExclusiveModeDetails
// ----------------------------------------------------------------------------------

SlicePlane::ExclusiveModeDetails::ExclusiveModeDetails
    ( SlicePlane& self
    , const base::model::Scene& model )
    
    : SlicePlane::Details( self, model )
    , pbuffer( new QGLPixelBuffer( 8, 8, QGLFormat::defaultFormat() ) )
    , volumeTexture( new base::model::Volume::Texture( model.volume() ) )
    , glContext( getPixelBufferContext( pbuffer.get() ) )
{
}


void SlicePlane::ExclusiveModeDetails::activateContext()
{
    const_cast< QGLContext* >( glContext )->makeCurrent();
}


unsigned int SlicePlane::ExclusiveModeDetails::getVolumeTextureID() const
{
    return volumeTexture->textureID;
}



// ----------------------------------------------------------------------------------
// SlicePlane :: SharedModeDetails
// ----------------------------------------------------------------------------------

SlicePlane::SharedModeDetails::SharedModeDetails
    ( SlicePlane& self
    , base::VisualizationEnvironment& environment
    , base::view::SceneProvider& provider )

    : SlicePlane::Details( self, provider.scene )
    , base::view::SceneProvider::ResourceClient( environment, provider )
    , environment( environment )
{
}


void SlicePlane::SharedModeDetails::activateContext()
{
    environment.activateContext();
}


unsigned int SlicePlane::SharedModeDetails::getVolumeTextureID() const
{
    return volumeTextureID;
}



}  // namespace Carna :: SlicePlanes

}  // namespace Carna
