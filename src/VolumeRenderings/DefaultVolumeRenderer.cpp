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
#include <Carna/base/view/DefaultCamera.h>
#include <Carna/base/view/SceneProvider.h>
#include <Carna/base/view/ShaderProgram.h>
#include <Carna/base/view/FramebufferObject.h>
#include <Carna/base/qt/RotationEdit.h>
#include <Carna/base/model/Position.h>
#include <Carna/base/Visualization.h>
#include <Carna/base/VisualizationEnvironment.h>
#include <Carna/base/CarnaException.h>
#include <Carna/VolumeRenderings/DefaultVolumeRenderer.h>
#include <Carna/VolumeRenderings/DefaultVolumeRendererDetails.h>
#include <Carna/VolumeRenderings/VolumeRendererMode.h>
#include <QDebug>
#include <QTimer>

namespace Carna
{

namespace VolumeRenderings
{



// ----------------------------------------------------------------------------------
// Types & Globals
// ----------------------------------------------------------------------------------

const static int INTENSITY_TEXTURE_SAMPLER = 0;
const static int FACTORIAL_TEXTURE_SAMPLER = 1;
const static int BACK_FACES_TEXTURE_SAMPLER = 2;
const static int FRONT_FACES_TEXTURE_SAMPLER = 3;



// ----------------------------------------------------------------------------------
// postProcessAndDrawBack
// ----------------------------------------------------------------------------------

static void postProcessAndDrawBack
    ( DefaultVolumeRenderer& renderer
    , const base::view::RenderTexture& postProcessingBuffer
    , const DefaultVolumeRenderer::ColorOperation* const overrideColorOperation = nullptr )
{
    glBindTexture( GL_TEXTURE_2D, postProcessingBuffer.id );

    base::view::ShaderProgram::Binding shader( renderer.shader( "volume-postpass" ) );

    base::Transformation trafo;

    if( renderer.isFlippedHorizontal() )
    {
        trafo.a11() *= -1;
    }
    if( renderer.isFlippedVertical() )
    {
        trafo.a22() *= -1;
    }
    if( renderer.rotation2D() != 0 )
    {
        trafo.rotateZ( -static_cast< signed >( renderer.rotation2D() ), base::Transformation::DEGREES );
    }

    trafo
        = base::Transformation().translate( 0.5, 0.5, 0.5 )
        * trafo
        * base::Transformation().translate( -0.5, -0.5, -0.5 );

    shader.putUniform1i ( "sceneTexture", 0 );
    shader.putUniform1i ( "inversion"   , overrideColorOperation ? *overrideColorOperation : renderer.colorPostProcessing() );
    shader.putUniform1i ( "mask"        , renderer.hasMask() ? 1 : 0 );
    shader.putUniform1i ( "width"       , renderer.shortestTargetSide() );
    shader.putUniform1i ( "height"      , renderer.shortestTargetSide() );
    shader.putUniform4x4( "trafo"       , trafo );

    glDisable( GL_BLEND );

    renderer.paintRectangle();

    // catch errors

    REPORT_GL_ERROR;
}



// ----------------------------------------------------------------------------------
// DefaultVolumeRenderer
// ----------------------------------------------------------------------------------

const unsigned int DefaultVolumeRenderer::FIRST_GENERAL_PURPOSE_TEXTURE_SAMPLER = 4;


DefaultVolumeRenderer::DefaultVolumeRenderer
    ( base::Visualization& module
    , base::view::SceneProvider& provider
    , QObject* parent )

    : VolumeRenderer( module, provider, parent )
    , pimpl( new Details( *this ) )
{

 // initialize post-processing buffer

    const unsigned int w = shortestTargetSide();
    const unsigned int h = shortestTargetSide();

    pimpl->pp_buffer.reset( new base::view::RenderTexture( w, h, false ) );
    pimpl->ff_buffer.reset( new base::view::RenderTexture( w, h,  true ) );
    pimpl->bf_buffer.reset( new base::view::RenderTexture( w, h,  true ) );

    pimpl->fbo.reset( new base::view::FramebufferObject( *pimpl->pp_buffer ) );

 // adjust GL states

    glDisable( GL_TEXTURE_3D );
    glDisable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE );
    glFrontFace( GL_CCW );
    glClearColor( 0, 0, 0, 0 );

 // bind textures

    glActiveTexture( GL_TEXTURE0 + FRONT_FACES_TEXTURE_SAMPLER );
    glBindTexture( GL_TEXTURE_2D, pimpl->ff_buffer->id );

    glActiveTexture( GL_TEXTURE0 + BACK_FACES_TEXTURE_SAMPLER );
    glBindTexture( GL_TEXTURE_2D, pimpl->bf_buffer->id );

    glActiveTexture( GL_TEXTURE0 + FACTORIAL_TEXTURE_SAMPLER );
    glBindTexture( GL_TEXTURE_3D, maskTextureID() );

    glActiveTexture( GL_TEXTURE0 + INTENSITY_TEXTURE_SAMPLER );
    glBindTexture( GL_TEXTURE_3D, volumeTextureID() );

 // check for errors

    REPORT_GL_ERROR;

}


DefaultVolumeRenderer::~DefaultVolumeRenderer()
{
    module().environment().activateContext();

 // turn off current mode

    pimpl->modeFrame.reset();

 // release modes

    for( VolumeRendererModes::iterator it = pimpl->modes.begin(); it != pimpl->modes.end(); ++it )
    {
        delete *it;
    }
}


const DefaultVolumeRenderer::VolumeRendererModes& DefaultVolumeRenderer::modes() const
{
    return pimpl->modes;
}


int DefaultVolumeRenderer::sampleCount() const
{
    CARNA_ASSERT( pimpl->sampleCount >= 2 );

    return pimpl->sampleCount;
}


DefaultVolumeRenderer::ColorOperation DefaultVolumeRenderer::colorPostProcessing() const
{
    return pimpl->colorPostProcessing;
}


bool DefaultVolumeRenderer::hasMask() const
{
    return pimpl->mask;
}


void DefaultVolumeRenderer::installMode( VolumeRendererMode* mode )
{
    CARNA_ASSERT( mode != nullptr );

    if( !hasModeInstalled( *mode ) )
    {
        pimpl->modes.push_back( mode );
    }

    CARNA_ASSERT( hasModeInstalled( *mode ) );
    CARNA_ASSERT( hasModeInstalled( mode->name ) );
}


bool DefaultVolumeRenderer::hasModeInstalled( VolumeRendererMode& mode ) const
{
    if( pimpl->modes.empty() )
    {
        return false;
    }
    else
    {
        return std::find( pimpl->modes.begin(), pimpl->modes.end(), &mode ) != pimpl->modes.end();
    }
}


bool DefaultVolumeRenderer::hasModeInstalled( const std::string& modeName ) const
{
    for( VolumeRendererModes::const_iterator it = pimpl->modes.begin(); it != pimpl->modes.end(); ++it )
    {
        VolumeRendererMode& mode = **it;
        if( mode.name == modeName )
        {
            return true;
        }
    }
    return false;
}


void DefaultVolumeRenderer::setMode( const std::string& modeName )
{
    CARNA_ASSERT( hasModeInstalled( modeName ) );

    bool found = false;
    for( VolumeRendererModes::iterator it = pimpl->modes.begin(); it != pimpl->modes.end(); ++it )
    {
        VolumeRendererMode& mode = **it;
        if( mode.name == modeName )
        {
            setMode( mode );
            found = true;
        }
    }

    CARNA_ASSERT_EX( found, "DefaultVolumeRenderer has no mode installed named '" << modeName << "'" );

    CARNA_ASSERT( mode().name == modeName );
}


void DefaultVolumeRenderer::setMode( VolumeRendererMode& newMode )
{
    CARNA_ASSERT( hasModeInstalled( newMode ) );

    CARNA_ASSERT( volumeTextureID() > 0 );

    module().environment().activateContext();

 // deactivate previous mode

    pimpl->modeFrame.reset();

 // invoke initialization routine if it is the first activation

    if( !newMode.isInitialized() )
    {
        module().environment().activateContext();

        glPushAttrib( GL_ALL_ATTRIB_BITS );

        newMode.prepareForRenderer( *this );

        glPopAttrib();
    }

 // activate new mode

    module().environment().activateContext();

    pimpl->modeFrame.reset( new DefaultVolumeRendererModeFrame( newMode ) );

 // post repaint event

    pimpl->modeFrame->mode.resizeBuffers
        ( std::max( 1u, targetWidth() )
        , std::max( 1u, targetHeight() ) );

    glViewport
        ( 0
        , 0
        , shortestTargetSide()
        , shortestTargetSide() );

    this->invalidate();

 // emit signals

    emit modeChanged();
    emit modeChanged( newMode );

    CARNA_ASSERT( hasMode() );
    CARNA_ASSERT( &mode() == &newMode );
}


bool DefaultVolumeRenderer::hasMode() const
{
    return pimpl->modeFrame.get() != nullptr;
}


void DefaultVolumeRenderer::setNoMode()
{
    if( hasMode() )
    {
        module().environment().activateContext();

        pimpl->modeFrame.reset();
    }

    CARNA_ASSERT( !hasMode() );
}


void DefaultVolumeRenderer::setRotationOffset( const base::Transformation& rotationOffset )
{
    pimpl->rotationOffset = rotationOffset;

    this->invalidate();
}


void DefaultVolumeRenderer::setRotationCenter( const base::Vector& rotationCenter )
{
    pimpl->rotationCenter = rotationCenter;

    this->invalidate();
}


void DefaultVolumeRenderer::setSampleCount( int sample_count )
{
    pimpl->sampleCount = std::max( 2, sample_count );

    this->invalidate();
}


void DefaultVolumeRenderer::setColorPostProcessing( ColorOperation colorPostProcessing )
{
    pimpl->colorPostProcessing = colorPostProcessing;

    this->invalidate();
}


void DefaultVolumeRenderer::setMask( bool b )
{
    pimpl->mask = b;

    this->invalidate();
}


void DefaultVolumeRenderer::setFlippedHorizontal( bool b )
{
    pimpl->flippedHorizontal = b;

    this->invalidate();
}


void DefaultVolumeRenderer::setFlippedVertical( bool b )
{
    pimpl->flippedVertical = b;

    this->invalidate();
}


void DefaultVolumeRenderer::setRotation2D( int angle )
{
    pimpl->rotation2D = static_cast< unsigned int >( angle % 360 );

    this->invalidate();
}


void DefaultVolumeRenderer::fetchSceneMatrix
    ( base::Transformation& sceneMatrix
    , const base::Transformation& cameraMatrix
    , const base::Transformation& projectionMatrix ) const
{
    using base::Transformation;
    using base::Vector;

 // scene space

    Renderer::fetchSceneMatrix( sceneMatrix, cameraMatrix, projectionMatrix );

 // volume space

    pimpl->objectsModelView = cameraMatrix * sceneMatrix * Transformation().translate( -0.5, -0.5, -0.5 );

    const Vector rotation_center = sceneMatrix * pimpl->rotationCenter;
    const Vector rotation_center_offset = sceneMatrix * Vector( 0.5, 0.5, 0.5 );

    sceneMatrix = Transformation().translate( rotation_center.x() - rotation_center_offset.x()
                                            , rotation_center.y() - rotation_center_offset.y()
                                            , rotation_center.z() - rotation_center_offset.z() )
                * pimpl->rotationOffset
                * Transformation().translate( -rotation_center.x()
                                            , -rotation_center.y()
                                            , -rotation_center.z() )
                * sceneMatrix;
}


const base::Transformation& DefaultVolumeRenderer::getObjectsModelView() const
{
    return pimpl->objectsModelView;
}


void DefaultVolumeRenderer::renderColor()
{
    if( !pimpl->modeFrame.get() )
    {
        return;
    }

 // prepare volume rendering

    RayMarching volume_rendering
        ( *this
        , pimpl->sampleCount
        , INTENSITY_TEXTURE_SAMPLER
        , FACTORIAL_TEXTURE_SAMPLER
        , FRONT_FACES_TEXTURE_SAMPLER
        , BACK_FACES_TEXTURE_SAMPLER );

 // render pre-pass

    {
        base::view::ShaderProgram::Binding shader_binding( volume_rendering.renderer.shader( "volume-prepass" ) );

     // render volume bounding box back faces

        {
            base::view::FramebufferObject::MinimalBinding fbo_minimal_binding( *pimpl->fbo );
            fbo_minimal_binding.setColorComponent( *pimpl->bf_buffer );

            base::view::FramebufferObject::Binding fbo_binding( *pimpl->fbo );
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

            glCullFace( GL_FRONT );
            emitVolumeBoundingBox();
        }

     // render volume bounding box front faces

        {
            base::view::FramebufferObject::MinimalBinding fbo_minimal_binding( *pimpl->fbo );
            fbo_minimal_binding.setColorComponent( *pimpl->ff_buffer );

            base::view::FramebufferObject::Binding fbo_binding( *pimpl->fbo );
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

            glCullFace( GL_BACK );
            emitVolumeBoundingBox();
        }
    }

 // render to post-processing buffer

    {
        base::view::FramebufferObject::MinimalBinding fbo_minimal_binding( *pimpl->fbo );
        fbo_minimal_binding.setColorComponent( *pimpl->pp_buffer );

        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        base::view::FramebufferObject::Binding fbo_binding( *pimpl->fbo );
        pimpl->modeFrame->mode.renderColor( volume_rendering );
    }

 // perform post-processing
    
    postProcessAndDrawBack( *this, *pimpl->pp_buffer );
}


void DefaultVolumeRenderer::renderDepth()
{
    if( !pimpl->modeFrame.get() )
    {
        return;
    }

    RayMarching volume_rendering
        ( *this
        , pimpl->sampleCount
        , INTENSITY_TEXTURE_SAMPLER
        , FACTORIAL_TEXTURE_SAMPLER
        , FRONT_FACES_TEXTURE_SAMPLER
        , BACK_FACES_TEXTURE_SAMPLER );

    // render to post-processing buffer

    {
        base::view::FramebufferObject::MinimalBinding fbo_minimal_binding( *pimpl->fbo );
        fbo_minimal_binding.setColorComponent( *pimpl->pp_buffer );

        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glBindTexture( GL_TEXTURE_3D, volumeTextureID() );

        base::view::FramebufferObject::Binding fbo_binding( *pimpl->fbo );
        pimpl->modeFrame->mode.renderDepth( volume_rendering );
    }

    // perform post-processing

    glClear( GL_DEPTH_BUFFER_BIT );
    const static ColorOperation noColorOperation = DefaultVolumeRenderer::noColorOperation;
    postProcessAndDrawBack( *this, *pimpl->pp_buffer, &noColorOperation );
}


void DefaultVolumeRenderer::setTargetSize( unsigned int w, unsigned int h )
{
    Renderer::setTargetSize( w, h );

    w = std::max( 1u, w );
    h = std::max( 1u, h );

    const unsigned int s = std::min( w, h );

    pimpl->pp_buffer->resize( s, s );
    pimpl->ff_buffer->resize( s, s );
    pimpl->bf_buffer->resize( s, s );

    pimpl->fbo->resize( s, s );

    if( pimpl->modeFrame.get() )
    {
        pimpl->modeFrame->mode.resizeBuffers( s, s );
    }
}


VolumeRendererMode& DefaultVolumeRenderer::mode()
{
    CARNA_ASSERT_EX
        ( pimpl->modeFrame.get() != nullptr
        , "no VolumeRendererMode has been set" )

    return pimpl->modeFrame->mode;
}


void DefaultVolumeRenderer::emitVolumeBoundingBox() const
{
    glClear( GL_COLOR_BUFFER_BIT );

 // front

    glColor4f( 1, 1, 1, 1 );
    glBegin( GL_TRIANGLE_FAN );
    
        glVertex3f( pimpl->xMin, pimpl->yMin, pimpl->zMax );
        glVertex3f( pimpl->xMax, pimpl->yMin, pimpl->zMax );
        glVertex3f( pimpl->xMax, pimpl->yMax, pimpl->zMax );
        glVertex3f( pimpl->xMin, pimpl->yMax, pimpl->zMax );

    glEnd();

 // back

    glBegin( GL_TRIANGLE_FAN );
    
        glVertex3f( pimpl->xMin, pimpl->yMin, pimpl->zMin );
        glVertex3f( pimpl->xMin, pimpl->yMax, pimpl->zMin );
        glVertex3f( pimpl->xMax, pimpl->yMax, pimpl->zMin );
        glVertex3f( pimpl->xMax, pimpl->yMin, pimpl->zMin );

    glEnd();

 // left

    glBegin( GL_TRIANGLE_FAN );
    
        glVertex3f( pimpl->xMin, pimpl->yMin, pimpl->zMax );
        glVertex3f( pimpl->xMin, pimpl->yMax, pimpl->zMax );
        glVertex3f( pimpl->xMin, pimpl->yMax, pimpl->zMin );
        glVertex3f( pimpl->xMin, pimpl->yMin, pimpl->zMin );

    glEnd();

 // right

    glBegin( GL_TRIANGLE_FAN );
    
        glVertex3f( pimpl->xMax, pimpl->yMin, pimpl->zMin );
        glVertex3f( pimpl->xMax, pimpl->yMax, pimpl->zMin );
        glVertex3f( pimpl->xMax, pimpl->yMax, pimpl->zMax );
        glVertex3f( pimpl->xMax, pimpl->yMin, pimpl->zMax );

    glEnd();

 // top

    glBegin( GL_TRIANGLE_FAN );
    
        glVertex3f( pimpl->xMin, pimpl->yMax, pimpl->zMin );
        glVertex3f( pimpl->xMin, pimpl->yMax, pimpl->zMax );
        glVertex3f( pimpl->xMax, pimpl->yMax, pimpl->zMax );
        glVertex3f( pimpl->xMax, pimpl->yMax, pimpl->zMin );

    glEnd();

 // bottom

    glBegin( GL_TRIANGLE_FAN );
        
        glVertex3f( pimpl->xMax, pimpl->yMin, pimpl->zMin );
        glVertex3f( pimpl->xMax, pimpl->yMin, pimpl->zMax );
        glVertex3f( pimpl->xMin, pimpl->yMin, pimpl->zMax );
        glVertex3f( pimpl->xMin, pimpl->yMin, pimpl->zMin );

    glEnd();
}


double DefaultVolumeRenderer::minX() const
{
    CARNA_ASSERT( 0. <= pimpl->xMin && pimpl->xMin <= 1. );

    return pimpl->xMin;
}


double DefaultVolumeRenderer::maxX() const
{
    CARNA_ASSERT( 0. <= pimpl->xMax && pimpl->xMax <= 1. );

    return pimpl->xMax;
}


double DefaultVolumeRenderer::minY() const
{
    CARNA_ASSERT( 0. <= pimpl->yMin && pimpl->yMin <= 1. );

    return pimpl->yMin;
}


double DefaultVolumeRenderer::maxY() const
{
    CARNA_ASSERT( 0. <= pimpl->yMax && pimpl->yMax <= 1. );

    return pimpl->yMax;
}


double DefaultVolumeRenderer::minZ() const
{
    CARNA_ASSERT( 0. <= pimpl->zMin && pimpl->zMin <= 1. );

    return pimpl->zMin;
}


double DefaultVolumeRenderer::maxZ() const
{
    CARNA_ASSERT( 0. <= pimpl->zMax && pimpl->zMax <= 1. );

    return pimpl->zMax;
}


void DefaultVolumeRenderer::setMinX( double xMin )
{
    CARNA_ASSERT( 0. <= xMin && xMin <= 1. );

    pimpl->xMin = xMin;

    this->invalidate();

    emit xMinChanged( pimpl->xMin );
}


void DefaultVolumeRenderer::setMaxX( double xMax )
{
    CARNA_ASSERT( 0. <= xMax && xMax <= 1. );

    pimpl->xMax = xMax;

    this->invalidate();

    emit xMaxChanged( pimpl->xMax );
}


void DefaultVolumeRenderer::setMinY( double yMin )
{
    CARNA_ASSERT( 0. <= yMin && yMin <= 1. );

    pimpl->yMin = yMin;

    this->invalidate();

    emit yMinChanged( pimpl->yMin );
}


void DefaultVolumeRenderer::setMaxY( double yMax )
{
    CARNA_ASSERT( 0. <= yMax && yMax <= 1. );

    pimpl->yMax = yMax;

    this->invalidate();

    emit yMaxChanged( pimpl->yMax );
}


void DefaultVolumeRenderer::setMinZ( double zMin )
{
    CARNA_ASSERT( 0. <= zMin && zMin <= 1. );

    pimpl->zMin = zMin;

    this->invalidate();

    emit zMinChanged( pimpl->zMin );
}


void DefaultVolumeRenderer::setMaxZ( double zMax )
{
    CARNA_ASSERT( 0. <= zMax && zMax <= 1. );

    pimpl->zMax = zMax;

    this->invalidate();

    emit zMaxChanged( pimpl->zMax );
}


bool DefaultVolumeRenderer::isFlippedHorizontal() const
{
    return pimpl->flippedHorizontal;
}


bool DefaultVolumeRenderer::isFlippedVertical() const
{
    return pimpl->flippedVertical;
}


unsigned int DefaultVolumeRenderer::rotation2D() const
{
    const unsigned degrees = pimpl->rotation2D % 360;

    CARNA_ASSERT( degrees < 360 );

    return degrees;
}



}  // namespace Carna :: VolumeRenderings

}  // namespace Carna
