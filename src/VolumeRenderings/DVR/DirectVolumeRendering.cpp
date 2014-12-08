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

#include <Carna/VolumeRenderings/DVR/DirectVolumeRendering.h>
#include <Carna/VolumeRenderings/DVR/DirectVolumeRenderingDetails.h>
#include <Carna/VolumeRenderings/DVR/DirectVolumeRenderingController.h>
#include <Carna/VolumeRenderings/DVR/DirectVolumeRenderingControllerUI.h>
#include <Carna/VolumeRenderings/DVR/ColorMapEditor.h>
#include <Carna/VolumeRenderings/DVR/ColorMap.h>
#include <Carna/VolumeRenderings/RayMarching.h>
#include <Carna/VolumeRenderings/VolumeRenderer.h>
#include <Carna/base/model/Scene.h>
#include <Carna/base/model/Volume.h>
#include <Carna/base/model/Object3D.h>
#include <Carna/base/model/Histogram.h>
#include <Carna/base/model/HistogramClassDetector.h>
#include <Carna/base/qt/Object3DChooser.h>
#include <Carna/base/qt/ExpandableGroupBox.h>
#include <Carna/base/view/SceneProvider.h>
#include <Carna/base/view/RenderTexture.h>
#include <Carna/base/view/FramebufferObject.h>
#include <Carna/base/view/glew.h>
#include <Carna/base/view/glError.h>
#include <Carna/base/CarnaException.h>
#include <Carna/base/Math.h>
#include <Carna/base/VisualizationEnvironment.h>
#include <Carna/base/Visualization.h>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QFormLayout>
#include <QPushButton>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QTextStream>
#include <QDebug>

namespace Carna
{

namespace VolumeRenderings
{

namespace DVR
{



// ----------------------------------------------------------------------------------
// Types & Globals
// ----------------------------------------------------------------------------------

const static unsigned int COLOR_MAP_TEXTURE_SAMPLER_OFFSET = 0;

const static int DRAW_MODE_COLORS = 0;
const static int DRAW_MODE_NORMALS = 1;
static int draw_mode = DRAW_MODE_COLORS;



// ----------------------------------------------------------------------------------
// VolumeSliceView
// ----------------------------------------------------------------------------------

class VolumeSliceView
{

public:

    typedef base::model::Histogram< base::model::IndexedCounter< signed short >, VolumeSliceView, 2 > Histogram;

    const static unsigned int AXIS_X = 0;
    const static unsigned int AXIS_Y = 1;
    const static unsigned int AXIS_Z = 2;

    VolumeSliceView
        ( const base::model::Volume& volume
        , unsigned int slice_normal_axis
        , unsigned int slice_normal_axis_position );

    const base::model::Volume& volume;
    unsigned int axis1, axis2, slice_normal_axis;
    unsigned int slice_normal_axis_position;
    const Histogram::Vector size;

    signed short operator()( const Histogram::Vector& position_in_slice ) const;

}; // VolumeSliceView


VolumeSliceView::VolumeSliceView( const base::model::Volume& volume, unsigned int slice_normal_axis, unsigned int slice_normal_axis_position )
    : volume( volume )
    , axis1( ( slice_normal_axis + 1 ) % 3 )
    , axis2( ( slice_normal_axis + 2 ) % 3 )
    , size( volume.size[ axis1 ], volume.size[ axis2 ] )
    , slice_normal_axis( slice_normal_axis )
    , slice_normal_axis_position( slice_normal_axis_position )
{
}


signed short VolumeSliceView::operator()( const Histogram::Vector& position_in_slice ) const
{
    base::Vector3ui position_in_volume;
    position_in_volume[ slice_normal_axis ] = slice_normal_axis_position;
    position_in_volume[ axis1 ] = position_in_slice[ 0 ];
    position_in_volume[ axis2 ] = position_in_slice[ 1 ];
    return volume( position_in_volume );
}



// ----------------------------------------------------------------------------------
// DVR :: DirectVolumeRendering :: Details
// ----------------------------------------------------------------------------------

DirectVolumeRendering::Details::Details( DirectVolumeRendering& self, base::model::Scene& model )
    : self( self )
    , model( model )
    , gradientSampleDistance
        ( 1.f / model.volume().size.x
        , 1.f / model.volume().size.y
        , 1.f / model.volume().size.z )
    , colorMapTextureID( 0 )
    , lighting( true )
    , lightAmplitude( 1.1f )
    , lightBias( 0.1f )
    , lightBackFaceAmount( 0.5f )
    , pickingSaturation( 0.8f )
    , positionedObject( nullptr )
    , isColorMapRebuildScheduled( false )
    , controllerFilter( new Controller( self ) )
{
    connect( &colorMap, SIGNAL( changed() ), this, SLOT( scheduleColorMapRebuild() ), Qt::QueuedConnection );
    connect( &colorMapRebuildTimer, SIGNAL( timeout() ), this, SLOT( rebuildColorMap() ) );

 // compute histogram approximation

    const VolumeSliceView view( model.volume(), VolumeSliceView::AXIS_Z, model.volume().size[ VolumeSliceView::AXIS_Z ] / 2 );
    VolumeSliceView::Histogram histogram( view, view.size );
    histogram.updateCounter();

 // detect histogram classes

    typedef base::model::HistogramClassDetector< VolumeSliceView::Histogram > HistogramClassDetector;
    HistogramClassDetector class_detector( histogram );
    class_detector.compute( HistogramClassDetector::Configuration( -1024, 3071, 50u, 100u, 0.2 ) );
    CARNA_ASSERT( class_detector.done() );

 // create initial color map

    colorMap.clear();

    std::vector< QColor > color_cycle;
    color_cycle.push_back( QColor( 255, 210, 190 ) );   // light red
    color_cycle.push_back( QColor( 180, 180, 180 ) );   // gray
    color_cycle.push_back( QColor( 255,   0,   0 ) );   // red
    color_cycle.push_back( QColor( 100, 100, 255 ) );   // blue
    color_cycle.push_back( QColor( 255,   0, 255 ) );   // violet
    color_cycle.push_back( QColor( 255, 255,   0 ) );   // yellow
    color_cycle.push_back( QColor( 255, 150,   0 ) );   // orange

    auto class_itr = class_detector.result().begin();
    if( class_detector.result().size() >= 2 )
    {
        ++class_itr;
    }
    for( unsigned int color_index = 0; class_itr != class_detector.result().end(); ++class_itr )
    {
        if( !class_itr->typical && class_itr + 1 != class_detector.result().end() )
        {
            continue;
        }

        QColor class_color0 = color_cycle[ ( color_index++ ) % color_cycle.size() ];
        QColor class_color1 = class_color0;

        class_color0.setAlpha( static_cast< unsigned int >( 0.1 * 255 + 0.5 ) );
        class_color1.setAlpha( static_cast< unsigned int >( 1.0 * 255 + 0.5 ) );

        colorMap.put( ColorMap::HUSpan( class_itr->first, class_itr->last ), ColorMap::ColorSpan( class_color0, class_color1 ) );
    }
}


void DirectVolumeRendering::Details::installControllerFilter()
{
    self.renderer().module().addControllerFilter( *controllerFilter );
}


void DirectVolumeRendering::Details::uninstallControllerFilter()
{
    self.renderer().module().removeControllerFilter( *controllerFilter );
}


void DirectVolumeRendering::Details::rebuildColorMap()
{
    CARNA_ASSERT( self.isInitialized() );

    if( isColorMapRebuildScheduled )
    {
        isColorMapRebuildScheduled = false;
    }
    else
    {
        return;
    }

    colorMap.writeToBuffer( colorMapBuffer );

    self.renderer().module().environment().activateContext();

    glActiveTexture( GL_TEXTURE0 + self.renderer().firstGeneralPurposeTextureSampler() + COLOR_MAP_TEXTURE_SAMPLER_OFFSET );

    glBindTexture( GL_TEXTURE_1D, colorMapTextureID );

    glTexImage1D
        ( GL_TEXTURE_1D
        , 0
        , GL_RGBA8
        , colorMapBuffer.size() / 4
        , 0
        , GL_RGBA
        , GL_UNSIGNED_BYTE
        , &colorMapBuffer[ 0 ] );

    glActiveTexture( GL_TEXTURE0 );

    self.invalidate();
}


void DirectVolumeRendering::Details::scheduleColorMapRebuild()
{
    isColorMapRebuildScheduled = true;
}



// ----------------------------------------------------------------------------------
// DVR :: DirectVolumeRendering
// ----------------------------------------------------------------------------------

const std::string DirectVolumeRendering::NAME = "Direct Volume Rendering";


DirectVolumeRendering::DirectVolumeRendering( base::model::Scene& model )
    : VolumeRendererSinglePassMode( "absorption", NAME )
    , pimpl( new Details( *this, model ) )
{
}


DirectVolumeRendering::~DirectVolumeRendering()
{
    if( pimpl->colorMapTextureID )
    {
        glDeleteTextures( 1, &pimpl->colorMapTextureID );
    }
}


QWidget* DirectVolumeRendering::createControllerUI()
{
    return new DirectVolumeRenderingControllerUI( *this );
}


void DirectVolumeRendering::setLightAmplitude( double lightAmplitude )
{
    if( !base::Math::isEqual< double >( pimpl->lightAmplitude, lightAmplitude ) )
    {
        pimpl->lightAmplitude = static_cast< float >( lightAmplitude );
        this->invalidate();
    }

    CARNA_ASSERT( base::Math::isEqual( getLightAmplitude(), lightAmplitude ) );
}


void DirectVolumeRendering::setLightBias( double lightBias )
{
    if( !base::Math::isEqual< double >( pimpl->lightBias, lightBias ) )
    {
        pimpl->lightBias = static_cast< float >( lightBias );
        this->invalidate();
    }

    CARNA_ASSERT( base::Math::isEqual( getLightBias(), lightBias ) );
}


void DirectVolumeRendering::setLightBackFaceAmount( double lightBackFaceAmount )
{
    if( !base::Math::isEqual< double >( pimpl->lightBackFaceAmount, lightBackFaceAmount ) )
    {
        pimpl->lightBackFaceAmount = static_cast< float >( lightBackFaceAmount );
        this->invalidate();
    }

    CARNA_ASSERT( base::Math::isEqual( getLightBackFaceAmount(), lightBackFaceAmount ) );
}


void DirectVolumeRendering::setLighting( bool lighting )
{
    if( pimpl->lighting != lighting )
    {
        pimpl->lighting = lighting;
        this->invalidate();
    }

    CARNA_ASSERT( hasLighting() == lighting );
}


void DirectVolumeRendering::setPickingSaturation( double pickingSaturation )
{
    if( !base::Math::isEqual< double >( pimpl->pickingSaturation, pickingSaturation ) )
    {
        pimpl->pickingSaturation = static_cast< float >( pickingSaturation );
        this->invalidate();
    }

    CARNA_ASSERT( base::Math::isEqual( getPickingSaturation(), pickingSaturation ) );
}


ColorMap& DirectVolumeRendering::colorMap()
{
    return pimpl->colorMap;
}


const ColorMap& DirectVolumeRendering::colorMap() const
{
    return pimpl->colorMap;
}


void DirectVolumeRendering::prepareForRenderer( VolumeRenderer& renderer )
{
    VolumeRendererSinglePassMode::prepareForRenderer( renderer );
 
 // create color-map

    GLint max_texture_size;
    glGetIntegerv( GL_MAX_TEXTURE_SIZE, &max_texture_size );
    if( max_texture_size < 4096 )
    {
        throw std::runtime_error( "GL_MAX_TEXTURE_SIZE too small" );
    }

 // create color-map texture

    glActiveTexture( GL_TEXTURE0 + renderer.firstGeneralPurposeTextureSampler() + COLOR_MAP_TEXTURE_SAMPLER_OFFSET );

    glGenTextures( 1, &pimpl->colorMapTextureID );

    glBindTexture( GL_TEXTURE_1D, pimpl->colorMapTextureID );

    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

 // acquire framebuffer object and rendering textures

    const unsigned int s = renderer.shortestTargetSide();

    pimpl->fbo.reset          ( new base::view::FramebufferObject( s, s ) );
    pimpl->colorBuffer.reset  ( new base::view::    RenderTexture( s, s ) );
    pimpl->pickingBuffer.reset( new base::view::    RenderTexture( s, s ) );

    {
        base::view::FramebufferObject::MinimalBinding binding( *pimpl->fbo );

        binding.setColorComponent(   *pimpl->colorBuffer, 0 );
        binding.setColorComponent( *pimpl->pickingBuffer, 1 );
    }
}


void DirectVolumeRendering::start()
{
    pimpl->colorMapRebuildTimer.start( 100 );   /* Check only every 100 milliseconds whether the color map
                                                 * needs to be rebuilt, so that it doesn't get rebuilt every
                                                 * time it is changed only a little bit.
                                                 */

 /* Texture bindings are not affected by glPushAttrib / glPopAttrib so we
  * have to restore them manually:
  */

    glActiveTexture( GL_TEXTURE0 + renderer().firstGeneralPurposeTextureSampler() + COLOR_MAP_TEXTURE_SAMPLER_OFFSET );

    glBindTexture( GL_TEXTURE_1D, pimpl->colorMapTextureID );

    glActiveTexture( GL_TEXTURE0 );
    
 // install controller filter

    pimpl->installControllerFilter();

 /** Alpha color component is explicitly required to be cleared to 0 for the screen
   * space to volume space mapping to work properly since the alpha value is being
   * taken into account for recognition of empty space.
   */

    glClearColor( 0, 0, 0, 0 );

 // setup depth buffer

    glDepthMask( GL_TRUE );
    glDepthFunc( GL_LEQUAL );
    glDisable( GL_DEPTH_TEST );

}


void DirectVolumeRendering::stop()
{
    pimpl->colorMapRebuildTimer.stop();

 // uninstall controller filter

    pimpl->uninstallControllerFilter();
}


void DirectVolumeRendering::resizeBuffers( int w, int h )
{
    VolumeRendererSinglePassMode::resizeBuffers( w, h );

    const unsigned int s = std::min( w, h );

    pimpl->pickingBuffer->resize( s, s );

    pimpl->colorBuffer->resize( s, s );

    pimpl->fbo->resize( s, s );
}


void DirectVolumeRendering::renderColor( RayMarching& pass )
{

 // render the scene to framebuffer object

    {
        base::view::FramebufferObject::Binding binding( *pimpl->fbo );

        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        REPORT_GL_ERROR;

        VolumeRendererSinglePassMode::renderColor( pass );

        REPORT_GL_ERROR;
    }

 // render back the resulted scene rendering

    glActiveTexture( GL_TEXTURE0 );
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, pimpl->colorBuffer->id );

    renderer().paintRectangle();

}


void DirectVolumeRendering::renderDepth( RayMarching& pass )
{
    glActiveTexture( GL_TEXTURE0 );
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, pimpl->pickingBuffer->id );
    
 // fetch matrices

    base::Transformation modelview_matrix;
    base::Transformation projection_matrix;

    base::fetchModelViewMatrix( modelview_matrix );
    base::fetchProjectionMatrix( projection_matrix );

 // bind shader
    
    base::view::ShaderProgram::Binding shader_binding( pass.renderer.shader( "absorption-depth" ) );

    static float z_shift = 0.5f;

    shader_binding.putUniform1i ( "object_space", 0 );
    shader_binding.putUniform4x4( "modelview_matrix", modelview_matrix );
    shader_binding.putUniform4x4( "projection_matrix", projection_matrix );
    shader_binding.putUniform1f ( "z_shift", z_shift );

 // evaluate picking texture

    renderer().paintRectangle();
}


void DirectVolumeRendering::configureShader( RayMarching&, base::view::ShaderProgram::Binding& shader )
{
    shader.putUniform1i( "colorMap", renderer().firstGeneralPurposeTextureSampler() + COLOR_MAP_TEXTURE_SAMPLER_OFFSET );
    shader.putUniform1f( "light_amplitude", pimpl->lightAmplitude );
    shader.putUniform1f( "light_bias", pimpl->lighting ? pimpl->lightBias : 1.f );
    shader.putUniform1f( "light_back", pimpl->lightBackFaceAmount );
    shader.putUniform1f( "pickingSaturation", pimpl->pickingSaturation );
    shader.putUniform3f( "gradient_sample_distance", pimpl->gradientSampleDistance );
    shader.putUniform1i( "draw_mode", draw_mode );

    if( draw_mode == DRAW_MODE_COLORS )
    {
        base::Transformation modelview_matrix;
        base::Transformation projection_matrix;

        base::fetchModelViewMatrix( modelview_matrix );
        base::fetchProjectionMatrix( projection_matrix );

        shader.putUniform4x4( "objectToScreenSpace", projection_matrix * modelview_matrix );
    }
}


bool DirectVolumeRendering::hasPositionedObject() const
{
    return pimpl->positionedObject != nullptr;
}


base::model::Object3D& DirectVolumeRendering::getPositionedObject() const
{
    CARNA_ASSERT( hasPositionedObject() );

    return *pimpl->positionedObject;
}


void DirectVolumeRendering::setPositionedObject( base::model::Object3D& object )
{
    removePositionedObject();

    pimpl->positionedObject = &object;

    connect( &object, SIGNAL( destroyed() ), this, SLOT( removePositionedObject() ) );

    CARNA_ASSERT( hasPositionedObject() );
    CARNA_ASSERT( &getPositionedObject() == &object );
}


void DirectVolumeRendering::removePositionedObject()
{
    if( pimpl->positionedObject )
    {
        disconnect( pimpl->positionedObject, SIGNAL( destroyed() ), this, SLOT( removePositionedObject() ) );

        pimpl->positionedObject = nullptr;
    }

    CARNA_ASSERT( !hasPositionedObject() );
}


double DirectVolumeRendering::getLightAmplitude() const
{
    return pimpl->lightAmplitude;
}


double DirectVolumeRendering::getLightBias() const
{
    return pimpl->lightBias;
}


bool DirectVolumeRendering::hasLighting() const
{
    return pimpl->lighting;
}


double DirectVolumeRendering::getLightBackFaceAmount() const
{
    return pimpl->lightBackFaceAmount;
}


double DirectVolumeRendering::getPickingSaturation() const
{
    return pimpl->pickingSaturation;
}


base::model::Scene& DirectVolumeRendering::mutableModel()
{
    return pimpl->model;
}


void DirectVolumeRendering::commitColorMap() const
{
    pimpl->scheduleColorMapRebuild();
    pimpl->rebuildColorMap();
}



}  // namespace Carna :: VolumeRenderings :: DVR

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna
