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
#include <Carna/base/view/Renderer.h>
#include <Carna/base/view/RendererDetails.h>
#include <Carna/base/view/RendererSharedData.h>
#include <Carna/base/view/SceneProvider.h>
#include <Carna/base/view/Camera.h>
#include <Carna/base/view/DefaultObject3DPaintingRoutine.h>
#include <Carna/base/view/IncrementalColorPaintingRoutine.h>
#include <Carna/base/view/Object3DPainter.h>
#include <Carna/base/view/ShaderBundle.h>
#include <Carna/base/view/FramebufferObject.h>
#include <Carna/base/Visualization.h>
#include <Carna/base/VisualizationEnvironment.h>
#include <Carna/base/CarnaException.h>
#include <Carna/base/model/Position.h>
#include <QDebug>
#include <QPoint>
#include <QGLFramebufferObject>
#include <QMessageBox>
#include <map>
#include <sstream>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// Renderer
// ----------------------------------------------------------------------------------

Renderer::Renderer
    ( Visualization& module
    , SceneProvider& provider
    , QObject* parent )

    : QObject( parent )
    , provider( provider )
    , pimpl( new Details( *this, module ) )
{
    CARNA_GLEW_INIT;

 // initialize GL states properly

    glEnable( GL_DEPTH_TEST );
    glClearDepth( 1.f );
    glDepthFunc( GL_LEQUAL );
    glDepthMask( GL_TRUE );

    pimpl->renderMode->initialize();
}


Renderer::~Renderer()
{
    module().environment().activateContext();
}


void Renderer::setCamera( Association< Camera >* camera )
{
    disconnect( &this->camera(), SIGNAL( viewChanged() ), this, 0 );

    CARNA_ASSERT_EX
        ( camera != nullptr && camera->get() != nullptr
        , "no Camera supplied to Renderer::setCamera" );

    pimpl->camera.reset( camera );

    connect( &this->camera(), SIGNAL( viewChanged() ), this, SLOT( invalidate() ) );

    emit cameraExchanged();
    
    this->invalidate();
}


const Camera& Renderer::camera() const
{
    return *pimpl->camera->get();
}


Camera& Renderer::camera()
{
    return *pimpl->camera->get();
}


unsigned int Renderer::volumeTextureID() const
{
    CARNA_ASSERT( pimpl->volumeTextureID > 0 );

    return pimpl->volumeTextureID;
}


unsigned int Renderer::maskTextureID() const
{
    CARNA_ASSERT( pimpl->maskTextureID > 0 );

    return pimpl->maskTextureID;
}


void Renderer::invalidate()
{
    emit invalidated();
}


void Renderer::addRenderingLock()
{
    ++pimpl->renderingLock;
}


void Renderer::removeRenderingLock()
{
    if( pimpl->renderingLock > 0 )
    {
        --pimpl->renderingLock;
    }
}


void Renderer::setObject3DPainter( Association< Object3DPainter >* objectPainter )
{
    CARNA_ASSERT( objectPainter != nullptr && objectPainter->get() != nullptr );

    pimpl->objectPainter.reset( objectPainter );
    this->invalidate();
}


void Renderer::paintRectangle
    ( float x0
    , float y0
    , float x1
    , float y1 )
{
    CARNA_ASSERT( x0 < x1 );
    CARNA_ASSERT( y0 < y1 );

    GLint viewport[ 4 ];
    glGetIntegerv( GL_VIEWPORT, viewport );

    REPORT_GL_ERROR;

    glMatrixMode( GL_PROJECTION );  //< save current, set new projection matrix
    glPushMatrix();
    glLoadIdentity();

    REPORT_GL_ERROR;
    
    glOrtho( 0, 1, 0, 1, -1, 1 );

    REPORT_GL_ERROR;

    glMatrixMode( GL_MODELVIEW );   //< save current, load identity modelview matrix
    glPushMatrix();
    glLoadIdentity();

    REPORT_GL_ERROR;

    glPushAttrib( GL_ALL_ATTRIB_BITS );

    REPORT_GL_ERROR;

    glDisable( GL_CULL_FACE );

    REPORT_GL_ERROR;

    glBegin( GL_TRIANGLE_FAN );

        glTexCoord2f( 0, 0 );
        glVertex3f( x0, y0, 0 );

        glTexCoord2f( 1, 0 );
        glVertex3f( x1, y0, 0 );

        glTexCoord2f( 1, 1 );
        glVertex3f( x1, y1, 0 );

        glTexCoord2f( 0, 1 );
        glVertex3f( x0, y1, 0 );

    glEnd();

    REPORT_GL_ERROR;

    glPopAttrib();

    REPORT_GL_ERROR;

    glPopMatrix();                  //< restore modelview matrix
    glMatrixMode( GL_PROJECTION );

    REPORT_GL_ERROR;

    glPopMatrix();                  //< restore projection matrix
    glMatrixMode( GL_MODELVIEW );

    REPORT_GL_ERROR;
}


unsigned int Renderer::shortestTargetSide() const
{
    return std::min( pimpl->targetWidth, pimpl->targetHeight );
}


unsigned int Renderer::setSquareViewport( float mod_w, float mod_h )
{
    CARNA_ASSERT( mod_w > 0 );
    CARNA_ASSERT( mod_h > 0 );

    const unsigned int w = unsigned int( pimpl->targetWidth  * mod_w + 0.5f );
    const unsigned int h = unsigned int( pimpl->targetHeight * mod_h + 0.5f );
    const unsigned int short_side = std::max( 1u, std::min( w, h ) );

    glViewport( ( w - short_side ) / 2
              , ( h - short_side ) / 2
              ,   short_side
              ,   short_side );

    return short_side;
}


unsigned int Renderer::targetWidth() const
{
    return pimpl->targetWidth;
}


unsigned int Renderer::targetHeight() const
{
    return pimpl->targetHeight;
}


void Renderer::paintObjects3D( Object3DPaintingRoutine& paintingRoutine ) const
{
    fetchModelViewMatrix ( pimpl->pickingEnviroment.modelview  );
    fetchProjectionMatrix( pimpl->pickingEnviroment.projection );

    pimpl->pickingEnviroment.valid = true;

    ( *pimpl->objectPainter )->paint( *this, paintingRoutine );
}


void Renderer::paintObjects3D() const
{
    DefaultObject3DPaintingRoutine paintingRoutine;

    paintObjects3D( paintingRoutine );
}


model::Object3D* Renderer::pickObject3D( const QPoint& p ) const
{
    if( !pimpl->pickingEnviroment.valid )
    {
        /* If no objects have been painted yet, also no objects can be picked.
         */

        return nullptr;
    }

    const unsigned int squareViewportSize = shortestTargetSide();
    
    const QPoint q( p.x() - ( targetWidth () - squareViewportSize ) / 2
                  , p.y() + ( targetHeight() - squareViewportSize ) / 2 );

    if( q.x() < 0 ||
        q.y() < 0 ||
        q.x() >= signed( targetWidth() ) ||
        q.y() >= signed( targetHeight() ) )
    {
        return nullptr;
    }

    typedef IncrementalColorPaintingRoutine::ObjectToIdMap ObjectToIdMap;
    typedef IncrementalColorPaintingRoutine::IdToObjectMap IdToObjectMap;

 // paint false-color image

    QGLFramebufferObject fbo( targetWidth(), targetHeight() );

    fbo.bind();

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadMatrix( pimpl->pickingEnviroment.modelview );

    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadMatrix( pimpl->pickingEnviroment.projection );

    glMatrixMode( GL_MODELVIEW );
    
    IncrementalColorPaintingRoutine paintingRoutine;
    paintObjects3D( paintingRoutine );

    glMatrixMode( GL_PROJECTION );
    glPopMatrix();

    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();

    glFlush();

    QImage img = fbo.toImage();

    fbo.release();

    QColor color( img.pixel( q ) );

 // pick object

    const unsigned int id = static_cast<unsigned int>( color.red() )
                         |( static_cast<unsigned int>( color.green() ) << 8 )
                         |( static_cast<unsigned int>( color.blue() ) << 16 );

    if( id == 0 )
    {
        return nullptr;
    }
    else
    {
        IdToObjectMap::const_iterator it = paintingRoutine.objectByIdMap().find( id );
        return it == paintingRoutine.objectByIdMap().end() ? 0 : const_cast< model::Object3D* >( it->second );
    }
}


const Transformation& Renderer::sceneTransformation() const
{
    return pimpl->sceneTrafo;
}


const Transformation& Renderer::cameraTransformation() const
{
    return pimpl->cameraTrafo;
}


const Transformation& Renderer::volumeViewTransformation() const
{
    return pimpl->volumeViewTrafo;
}


const Transformation& Renderer::projectionTransformation() const
{
    return pimpl->projectionTrafo;
}


void Renderer::fetchSceneMatrix
    ( Transformation& sceneMatrix
    , const Transformation& cameraMatrix
    , const Transformation& projectionMatrix ) const
{
    sceneMatrix = model::Position::toMillimetersTransformation( provider.scene );
}


void Renderer::setTargetSize( unsigned int w, unsigned int h )
{
    w = std::max( 1u, w );
    h = std::max( 1u, h );

    pimpl->targetWidth = w;
    pimpl->targetHeight = h;

    module().environment().activateContext();

    pimpl->renderMode->resizeBuffers( w, h );
    this->invalidate();
}


void Renderer::paint()
{
    if( pimpl->renderingLock > 0 || pimpl->paintRecursionLevel > 0 )
    {
        return;
    }

    /* Skip rendering if the target width or height is zero.
     */
    const unsigned int targetSize = shortestTargetSide();
    if( targetSize == 0 )
    {
        return;
    }

    try
    {
        ++pimpl->paintRecursionLevel;

        /* Ensure the provider object data is in sync with the model data it interfaces.
         */
        pimpl->updateContext();

        /* The viewport must be set before the transformations are computed,
         * because those do read the viewport in order to get the target size and origin.
         */
        glViewport( 0, 0, targetSize, targetSize );

        camera().openShutter( *this );

        pimpl->computeTransformations();

        pimpl->renderMode->paint();

        camera().closeShutter( *this );

        --pimpl->paintRecursionLevel;
    }
    catch( ... )
    {
        addRenderingLock();

        --pimpl->paintRecursionLevel;

        throw;
    }
}


void Renderer::setRenderMode( Renderer::RenderMode* renderMode )
{
    CARNA_ASSERT( renderMode != nullptr );

    pimpl->renderMode.reset( renderMode );

    module().environment().activateContext();
    pimpl->renderMode->initialize();
    this->invalidate();

    CARNA_ASSERT( &this->renderMode() == renderMode );
}


const Renderer::RenderMode& Renderer::renderMode() const
{
    return *pimpl->renderMode;
}


ShaderProgram& Renderer::shader( const std::string& id ) const
{
    return pimpl->sharedData.getShader( id );
}


ShaderProgram& Renderer::shader
    ( const std::string& vertex_shader_file
    , const std::string& fragment_shader_file ) const
{
    return pimpl->sharedData.getShader
        ( vertex_shader_file
        , fragment_shader_file );
}


Visualization& Renderer::module()
{
    CARNA_ASSERT( !pimpl->module.isInitialized() || &pimpl->module.renderer() == this );

    return pimpl->module;
}


const Visualization& Renderer::module() const
{
    CARNA_ASSERT( !pimpl->module.isInitialized() || &pimpl->module.renderer() == this );

    return pimpl->module;
}



// ----------------------------------------------------------------------------------
// Renderer :: RenderMode
// ----------------------------------------------------------------------------------

Renderer::RenderMode::RenderMode( Renderer& renderer )
    : renderer( renderer )
    , pimpl( new Details() )
{
}


Renderer::RenderMode::~RenderMode()
{
}


void Renderer::RenderMode::initialize( unsigned int w, unsigned int h )
{
    w = std::max( 1u, w );
    h = std::max( 1u, h );

    if( !pimpl->fbo.get() )
    {
        const unsigned int s = std::min( w, h );

        pimpl->fbo.reset( new FramebufferObject( s, s ) );
    }

    this->postInitialize();
}


void Renderer::RenderMode::initialize()
{
    initialize( renderer.targetWidth(), renderer.targetHeight() );
}


void Renderer::RenderMode::renderColorTo( RenderTexture& target )
{

 // setup framebuffer object

    FramebufferObject::MinimalBinding fbo_minimal_binding( *pimpl->fbo );
    fbo_minimal_binding.setColorComponent( target );

 // bind framebuffer for drawing

    FramebufferObject::Binding fbo_binding( *pimpl->fbo );

 // perform drawing operations

    glPushAttrib( GL_ALL_ATTRIB_BITS );
    glColor4f( 1, 1, 1, 1 );
    renderer.renderColor();
    glPopAttrib();

}


void Renderer::RenderMode::renderDepthTo( RenderTexture& target )
{
    FramebufferObject::Binding fbo_binding( *pimpl->fbo );
    fbo_binding.setColorComponent( target );

    glPushAttrib( GL_ALL_ATTRIB_BITS );
    glColor4f( 1, 1, 1, 1 );
    renderer.renderDepth();
    glPopAttrib();
}


void Renderer::RenderMode::resizeBuffers( unsigned int w, unsigned int h )
{
    w = std::max( 1u, w );
    h = std::max( 1u, h );

    const unsigned int s = std::min( w, h );

    pimpl->fbo->resize( s, s );
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
