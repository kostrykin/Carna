/*
 *  Copyright (C) 2010 - 2015 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#include <Carna/presets/MIPStage.h>
#include <Carna/presets/MIPLayer.h>
#include <Carna/base/glew.h>
#include <Carna/base/ShaderManager.h>
#include <Carna/base/ShaderUniform.h>
#include <Carna/base/Framebuffer.h>
#include <Carna/base/Viewport.h>
#include <Carna/base/RenderState.h>
#include <Carna/base/math.h>
#include <Carna/base/CarnaException.h>
#include <vector>
#include <algorithm>

namespace Carna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// MIPStage :: Details
// ----------------------------------------------------------------------------------

struct MIPStage::Details
{

    Details();

    MIPLayer* currentLayer;
    std::vector< MIPLayer* > layers;

    std::unique_ptr< base::Texture< 2 > > layerColorBuffer;
    std::unique_ptr< base::Framebuffer  > layerFrameBuffer;

}; // MIPStage :: Details


MIPStage::Details::Details()
    : currentLayer( nullptr )
{
}



// ----------------------------------------------------------------------------------
// MIPStage
// ----------------------------------------------------------------------------------

MIPStage::MIPStage( unsigned int geometryType )
    : VolumeRenderingStage( geometryType )
    , pimpl( new Details() )
{
}


MIPStage::~MIPStage()
{
    clearLayers();
}


MIPStage* MIPStage::clone() const
{
    MIPStage* const result = new MIPStage( geometryType );
    result->setEnabled( isEnabled() );
    return result;
}


void MIPStage::appendLayer( MIPLayer* layer )
{
    CARNA_ASSERT( std::find( pimpl->layers.begin(), pimpl->layers.end(), layer ) == pimpl->layers.end() );
    pimpl->layers.push_back( layer );
}


MIPLayer* MIPStage::removeLayer( const MIPLayer& layer )
{
    const auto layerItr = std::find( pimpl->layers.begin(), pimpl->layers.end(), const_cast< MIPLayer* >( &layer ) );
    CARNA_ASSERT( layerItr != pimpl->layers.end() );
    MIPLayer* const ownedLayer = *layerItr;
    pimpl->layers.erase( layerItr );
    return ownedLayer;
}


void MIPStage::ascendLayer( const MIPLayer& layer )
{
    const auto layerItr = std::find( pimpl->layers.begin(), pimpl->layers.end(), const_cast< MIPLayer* >( &layer ) );
    CARNA_ASSERT( layerItr != pimpl->layers.end() );
    const auto nextLayerItr = layerItr + 1;
    if( nextLayerItr != pimpl->layers.end() )
    {
        std::swap( *layerItr, *nextLayerItr );
    }
}


void MIPStage::clearLayers()
{
    std::for_each( pimpl->layers.begin(), pimpl->layers.end(), std::default_delete< MIPLayer >() );
    pimpl->layers.clear();
}


std::size_t MIPStage::layersCount() const
{
    return pimpl->layers.size();
}


MIPLayer& MIPStage::layer( std::size_t layerIndex )
{
    CARNA_ASSERT( layerIndex < layersCount() );
    return *pimpl->layers[ layerIndex ];
}


const MIPLayer& MIPStage::layer( std::size_t layerIndex ) const
{
    CARNA_ASSERT( layerIndex < layersCount() );
    return *pimpl->layers[ layerIndex ];
}


void MIPStage::reshape( base::FrameRenderer& fr, unsigned int width, unsigned int height )
{
    base::RenderStage::reshape( fr, width, height );
    pimpl->layerColorBuffer.reset( base::Framebuffer::createRenderTexture() );
    pimpl->layerFrameBuffer.reset( new base::Framebuffer( width, height, *pimpl->layerColorBuffer ) );
}


void MIPStage::renderPass
    ( const base::math::Matrix4f& vt
    , base::RenderTask& rt
    , const base::Viewport& outputViewport )
{
    /* Configure proper OpenGL state.
     */
    base::RenderState rs;
    rs.setBlend( true );
    rs.setDepthTest( false );
    rs.setDepthWrite( false );

    /* Copy depth buffer from output to dedicated frame buffer.
     */
    const base::Viewport framebufferViewport( *pimpl->layerFrameBuffer );
    const unsigned int outputFramebufferId = base::Framebuffer::currentId();
    base::Framebuffer::copyDepthAttachment
        ( outputFramebufferId
        , pimpl->layerFrameBuffer->id
        , outputViewport
        , framebufferViewport );

    /* For each layer: First render the layer-specific MIP result to the dedicated framebuffer,
     * than render the result to the output framebuffer w.r.t. the layer function.
     */
    for( auto layerItr = pimpl->layers.begin(); layerItr != pimpl->layers.end(); ++layerItr )
    {
        pimpl->currentLayer = *layerItr;

        /* Render to dedicated framebuffer.
         */
        CARNA_RENDER_TO_FRAMEBUFFER( *pimpl->layerFrameBuffer,

            base::RenderState rs;
            rs.setBlendEquation( GL_MAX );
            rs.setDepthTest( true );

            glClearColor( 0, 0, 0, 0 );
            rt.renderer.glContext().clearBuffers( GL_COLOR_BUFFER_BIT );

            framebufferViewport.makeActive();
            VolumeRenderingStage::renderPass( vt, rt, framebufferViewport );
            framebufferViewport.done();

        );

        /* Render result to output framebuffer.
         */
        base::RenderState rs;
        rs.setBlendFunction( pimpl->currentLayer->function() );

        pimpl->layerColorBuffer->bind( 0 );
        rt.renderer.renderTexture( base::FrameRenderer::RenderTextureParams( 0 ) );
    }

    /* Denote that we're finished with rendering.
     */
    pimpl->currentLayer = nullptr;
}


void MIPStage::createVolumeSamplers( const std::function< void( unsigned int, base::Sampler* ) >& registerSampler )
{
    registerSampler( ROLE_INTENSITY_VOLUME, new base::Sampler
        ( base::Sampler::WRAP_MODE_CLAMP, base::Sampler::WRAP_MODE_CLAMP, base::Sampler::WRAP_MODE_CLAMP
        , base::Sampler::FILTER_LINEAR, base::Sampler::FILTER_LINEAR ) );
}


const base::ShaderProgram& MIPStage::acquireShader()
{
    return base::ShaderManager::instance().acquireShader( "mip" );
}


const std::string& MIPStage::uniformName( unsigned int role ) const
{
    const static std::string ROLE_INTENSITY_VOLUME_NAME = "huVolume";
    switch( role )
    {

    case ROLE_INTENSITY_VOLUME:
        return ROLE_INTENSITY_VOLUME_NAME;

    default:
        CARNA_FAIL( "unknown role" );

    }
}


void MIPStage::configureShader()
{
    CARNA_ASSERT( pimpl->currentLayer != nullptr );
    const MIPLayer& ch = *pimpl->currentLayer;

    base::ShaderUniform< float >( "minIntensity", ch.intensityRange.first ).upload();
    base::ShaderUniform< float >( "maxIntensity", ch.intensityRange.last  ).upload();
    base::ShaderUniform< base::math::Vector4f >( "color", ch.color ).upload();
}


void MIPStage::configureShader( const base::Renderable& )
{
}



}  // namespace Carna :: VolumeRenderings

}  // namespace Carna
