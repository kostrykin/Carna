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

#include <Carna/presets/DVRStage.h>
#include <Carna/base/glew.h>
#include <Carna/base/ShaderManager.h>
#include <Carna/base/Framebuffer.h>
#include <Carna/base/RenderTexture.h>
#include <Carna/base/Viewport.h>
#include <Carna/base/RenderState.h>
#include <Carna/base/ShaderUniform.h>
#include <Carna/base/math.h>
#include <Carna/base/CarnaException.h>
#include <Carna/base/Log.h>
#include <algorithm>

namespace Carna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// DVRStage :: DVRStage
// ----------------------------------------------------------------------------------

struct DVRStage::Details
{
    Details();
    std::unique_ptr< base::Texture< 2 > > accumulationColorBuffer;
    std::unique_ptr< base::Framebuffer  > accumulationFrameBuffer;
    
    const static unsigned int COLORMAP_SIZE = 1 << 12;
    const static unsigned int COLORMAP_TEXTURE_UNIT = base::Texture< 0 >::SETUP_UNIT + 1;
    base::Color colorMap[ COLORMAP_SIZE ];
    std::unique_ptr< base::Texture< 1 > > colorMapTexture;
    std::unique_ptr< base::Sampler      > colorMapSampler;
    bool isColorMapDirty;
    void updateColorMap();
    
    float translucence;

    static inline float huvToIntensity( base::HUV huv )
    {
        return ( huv + 1024 ) / 4095.f;
    }
};


DVRStage::Details::Details()
    : isColorMapDirty( true )
    , translucence( DEFAULT_TRANSLUCENCE )
{
}


void DVRStage::Details::updateColorMap()
{
    if( isColorMapDirty )
    {
        /* Create the texture if it was not created yet.
         */
        if( colorMapTexture.get() == nullptr )
        {
            colorMapTexture.reset( new base::Texture< 1 >( GL_RGBA8, GL_RGBA ) );
        }
        
        /* Update the texture.
         */
        base::Texture< 1 >::Resolution textureSize;
        textureSize.x() = COLORMAP_SIZE;
        colorMapTexture->update( textureSize, GL_UNSIGNED_BYTE, colorMap );
        isColorMapDirty = false;
        base::Log::instance().record( base::Log::debug, "DVRStage: Color map updated." );
    }
}



// ----------------------------------------------------------------------------------
// DVRStage
// ----------------------------------------------------------------------------------

const float DVRStage::DEFAULT_TRANSLUCENCE = 50;


DVRStage::DVRStage( unsigned int geometryType )
    : VolumeRenderingStage( geometryType )
    , pimpl( new Details() )
{
}


DVRStage::~DVRStage()
{
    activateGLContext();
}


void DVRStage::clearColorMap()
{
    std::fill_n( pimpl->colorMap, Details::COLORMAP_SIZE, base::Color::BLACK_NO_ALPHA );
    pimpl->isColorMapDirty = true;
}


void DVRStage::writeColorMap( const base::math::Span< base::HUV >& huRange, const base::math::Span< base::Color > colorRange )
{
    const int huRangeSize = huRange.last - huRange.first + 1;
    const base::math::Vector4f color0 = colorRange.first;
    const base::math::Vector4f color1 = colorRange.last;
    if( huRangeSize > 0 )
    {
        CARNA_ASSERT( huRange.first >= -1024 );
        CARNA_ASSERT( huRange.last  <=  3071 );
        for( int huOffset = 0; huOffset < huRangeSize; ++huOffset )
        {
            const float lambda = huOffset / static_cast< float >( huRangeSize - 1 );
            const base::Color color = base::math::mix( color0, color1, lambda );
            pimpl->colorMap[ 1024 + huRange.first + huOffset ] = color;
        }
        pimpl->isColorMapDirty = true;
    }
}


void DVRStage::writeColorMap( base::HUV huFirst, base::HUV huLast, const base::Color& colorFirst, const base::Color& colorLast )
{
    writeColorMap( base::math::Span< base::HUV >( huFirst, huLast ), base::math::Span< base::Color >( colorFirst, colorLast ) );
}


void DVRStage::setTranslucence( float translucence )
{
    CARNA_ASSERT( translucence >= 0 );
    pimpl->translucence = translucence;
}


float DVRStage::translucence() const
{
    return pimpl->translucence;
}


void DVRStage::reshape( const base::FrameRenderer& fr, unsigned int width, unsigned int height )
{
    base::GeometryStage< base::Renderable::BackToFront >::reshape( fr, width, height );
    pimpl->accumulationColorBuffer.reset( base::Framebuffer::createRenderTexture( true ) );
    pimpl->accumulationFrameBuffer.reset( new base::Framebuffer( width, height, *pimpl->accumulationColorBuffer ) );
}


unsigned int DVRStage::loadVideoResources()
{
    VolumeRenderingStage::loadVideoResources();
    pimpl->colorMapSampler.reset( new base::Sampler
        ( base::Sampler::WRAP_MODE_CLAMP, base::Sampler::WRAP_MODE_CLAMP, base::Sampler::WRAP_MODE_CLAMP
        , base::Sampler::FILTER_NEAREST, base::Sampler::FILTER_NEAREST ) );
    return Details::COLORMAP_TEXTURE_UNIT + 1;
}


void DVRStage::renderPass
    ( const base::math::Matrix4f& vt
    , base::RenderTask& rt
    , const base::Viewport& outputViewport )
{
    pimpl->updateColorMap();
    
    /* Configure OpenGL state that is common to both following passes.
     */
    base::RenderState rs;
    rs.setBlend( true );

    /* Copy depth buffer from output to the accumulation frame buffer.
     */
    const base::Viewport framebufferViewport( *pimpl->accumulationFrameBuffer );
    const unsigned int outputFramebufferId = base::Framebuffer::currentId();
    base::Framebuffer::copyDepthAttachment
        ( outputFramebufferId
        , pimpl->accumulationFrameBuffer->id
        , outputViewport
        , framebufferViewport );

    /* We will render to a dedicated render target first, s.t. the depth buffer is
     * not contaminated by the slices.
     */
    CARNA_RENDER_TO_FRAMEBUFFER( *pimpl->accumulationFrameBuffer,

        /* Configure OpenGL state for accumulation pass.
         */
        base::RenderState rs;
        rs.setBlendFunction( base::BlendFunction( GL_ONE, GL_ONE_MINUS_SRC_ALPHA ) );

        glClearColor( 0, 0, 0, 0 );
        rt.renderer.glContext().clearBuffers( GL_COLOR_BUFFER_BIT );

        framebufferViewport.makeActive();
        VolumeRenderingStage::renderPass( vt, rt, framebufferViewport );
        framebufferViewport.done();

    );

    /* Now copy the results to the output buffer.
     */
    rs.setDepthTest( false );
    rs.setDepthWrite( false );
    rs.setBlendFunction( base::BlendFunction( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) );
    pimpl->accumulationColorBuffer->bind( 0 );
    base::FrameRenderer::RenderTextureParams params( 0 );
    rt.renderer.renderTexture( params );
}


void DVRStage::createVolumeSamplers( const std::function< void( unsigned int, base::Sampler* ) >& registerSampler )
{
    registerSampler( ROLE_HU_VOLUME, new base::Sampler
        ( base::Sampler::WRAP_MODE_CLAMP, base::Sampler::WRAP_MODE_CLAMP, base::Sampler::WRAP_MODE_CLAMP
        , base::Sampler::FILTER_LINEAR, base::Sampler::FILTER_LINEAR ) );
}


const base::ShaderProgram& DVRStage::acquireShader()
{
    return base::ShaderManager::instance().acquireShader( "dvr" );
}


const std::string& DVRStage::uniformName( unsigned int role ) const
{
    const static std::string ROLE_HU_VOLUME_NAME = "huVolume";
    switch( role )
    {

    case ROLE_HU_VOLUME:
        return ROLE_HU_VOLUME_NAME;

    default:
        CARNA_FAIL( "unknown role" );

    }
}


void DVRStage::configureShader()
{
    base::ShaderUniform<   int >( "colorMap", Details::COLORMAP_TEXTURE_UNIT ).upload();
    base::ShaderUniform< float >( "translucence", pimpl->translucence ).upload();
        
    /* Bind the color map.
     */
    pimpl->colorMapTexture->bind( Details::COLORMAP_TEXTURE_UNIT );
    pimpl->colorMapSampler->bind( Details::COLORMAP_TEXTURE_UNIT );
}



}  // namespace Carna :: presets

}  // namespace Carna
