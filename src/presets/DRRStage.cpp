/*
 *  Copyright (C) 2010 - 2016 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 * 
 * 
 *  Copyright (C) 2021 - 2025 Leonid Kostrykin
 * 
 */

#include <LibCarna/presets/DRRStage.hpp>
#include <LibCarna/base/glew.hpp>
#include <LibCarna/base/ShaderManager.hpp>
#include <LibCarna/base/Framebuffer.hpp>
#include <LibCarna/base/Viewport.hpp>
#include <LibCarna/base/RenderState.hpp>
#include <LibCarna/base/ShaderUniform.hpp>
#include <LibCarna/base/math.hpp>
#include <LibCarna/base/LibCarnaException.hpp>

namespace LibCarna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// DRRStage :: Details
// ----------------------------------------------------------------------------------

struct DRRStage::Details
{

    Details();

    float waterAttenuation;
    float baseIntensity;
    base::HUV lowerThreshold;
    base::HUV upperThreshold;
    float upperMultiplier;
    bool renderInverse;

    std::unique_ptr< base::Texture< 2 > > accumulationColorBuffer;
    std::unique_ptr< base::Framebuffer  > accumulationFrameBuffer;

    const base::ShaderProgram* exponentialShader;

    static inline float huvToIntensity( base::HUV huv )
    {
        return ( huv + 1024 ) / 4095.f;
    }

}; // DRRStage :: Details


DRRStage::Details::Details()
    : waterAttenuation ( DEFAULT_WATER_ATTENUATION )
    , baseIntensity    ( DEFAULT_BASE_INTENSITY )
    , lowerThreshold   ( DEFAULT_LOWER_THRESHOLD )
    , upperThreshold   ( DEFAULT_UPPER_THRESHOLD )
    , upperMultiplier  ( DEFAULT_UPPER_MULTIPLIER )
    , renderInverse    ( DEFAULT_RENDER_INVERSE )
    , exponentialShader( nullptr )
{
}



// ----------------------------------------------------------------------------------
// DRRStage
// ----------------------------------------------------------------------------------

const float     DRRStage::DEFAULT_WATER_ATTENUATION = 5e-3f;
const float     DRRStage::DEFAULT_BASE_INTENSITY    = 1;
const base::HUV DRRStage::DEFAULT_LOWER_THRESHOLD   = base::HUV( -400 );
const base::HUV DRRStage::DEFAULT_UPPER_THRESHOLD   = base::HUV( +400 );
const float     DRRStage::DEFAULT_UPPER_MULTIPLIER  = 1.5f;
const bool      DRRStage::DEFAULT_RENDER_INVERSE    = false;


DRRStage::DRRStage( unsigned int geometryType )
    : VolumeRenderingStage( geometryType )
    , pimpl( new Details() )
{
}


DRRStage::~DRRStage()
{
    if( pimpl->exponentialShader != nullptr )
    {
        activateGLContext();
        base::ShaderManager::instance().releaseShader( *pimpl->exponentialShader );
    }
}


float DRRStage::waterAttenuation() const
{
    return pimpl->waterAttenuation;
}


float DRRStage::baseIntensity() const
{
    return pimpl->baseIntensity;
}


base::HUV DRRStage::lowerThreshold() const
{
    return pimpl->lowerThreshold;
}


base::HUV DRRStage::upperThreshold() const
{
    return pimpl->upperThreshold;
}


float DRRStage::upperMultiplier() const
{
    return pimpl->upperMultiplier;
}


bool DRRStage::isRenderingInverse() const
{
    return pimpl->renderInverse;
}


void DRRStage::setWaterAttenuation( float muWater )
{
    LIBCARNA_ASSERT( muWater > 0 );
    pimpl->waterAttenuation = muWater;
}


void DRRStage::setBaseIntensity( float baseIntensity )
{
    LIBCARNA_ASSERT( baseIntensity > 0 );
    pimpl->baseIntensity = baseIntensity;
}


void DRRStage::setLowerThreshold( base::HUV lower )
{
    LIBCARNA_ASSERT( lower >= -1024 && lower <= 3071 );
    pimpl->lowerThreshold = lower;
}


void DRRStage::setUpperThreshold( base::HUV upper )
{
    LIBCARNA_ASSERT( upper >= -1024 && upper <= 3071 );
    pimpl->upperThreshold = upper;
}


void DRRStage::setUpperMultiplier( float multiplier )
{
    LIBCARNA_ASSERT( multiplier >= 0 );
    pimpl->upperMultiplier = multiplier;
}


void DRRStage::setRenderingInverse( bool inverse )
{
    pimpl->renderInverse = inverse;
}


void DRRStage::reshape( base::FrameRenderer& fr, unsigned int width, unsigned int height )
{
    base::RenderStage::reshape( fr, width, height );
    pimpl->accumulationColorBuffer.reset( base::Framebuffer::createRenderTexture( true ) );
    pimpl->accumulationFrameBuffer.reset( new base::Framebuffer( width, height, *pimpl->accumulationColorBuffer ) );
}


unsigned int DRRStage::loadVideoResources()
{
    pimpl->exponentialShader = &base::ShaderManager::instance().acquireShader( "drr_exponential" );
    return VolumeRenderingStage::loadVideoResources();
}


void DRRStage::renderPass
    ( const base::math::Matrix4f& vt
    , base::RenderTask& rt
    , const base::Viewport& outputViewport )
{
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

    /* First, evaluate the integral by rendering to the accumulation buffer.
     */
    LIBCARNA_RENDER_TO_FRAMEBUFFER( *pimpl->accumulationFrameBuffer,

        /* Configure OpenGL state for accumulation pass.
         */
        base::RenderState rs;
        rs.setBlendFunction( base::BlendFunction( GL_ONE, GL_ONE ) );

        glClearColor( 0, 0, 0, 0 );
        rt.renderer.glContext().clearBuffers( GL_COLOR_BUFFER_BIT );

        framebufferViewport.makeActive();
        VolumeRenderingStage::renderPass( vt, rt, framebufferViewport );
        framebufferViewport.done();

    );

    /* Now compute the exponential of the integral.
     */
    rs.setDepthTest( false );
    rs.setDepthWrite( false );
    rt.renderer.glContext().setShader( *pimpl->exponentialShader );
    base::ShaderUniform< float >( "baseIntensity", pimpl->baseIntensity ).upload();
    base::ShaderUniform< int >( "renderInverse", pimpl->renderInverse ? 1 : 0 ).upload();
    pimpl->accumulationColorBuffer->bind( 0 );
    base::FrameRenderer::RenderTextureParams params( 0 );
    params.useDefaultShader = false;
    params.textureUniformName = "integralMap";
    rt.renderer.renderTexture( params );
}


void DRRStage::createVolumeSamplers( const std::function< void( unsigned int, base::Sampler* ) >& registerSampler )
{
    registerSampler( ROLE_INTENSITIES, new base::Sampler
        ( base::Sampler::WRAP_MODE_CLAMP, base::Sampler::WRAP_MODE_CLAMP, base::Sampler::WRAP_MODE_CLAMP
        , base::Sampler::FILTER_LINEAR, base::Sampler::FILTER_LINEAR ) );
}


const base::ShaderProgram& DRRStage::acquireShader()
{
    return base::ShaderManager::instance().acquireShader( "drr_accumulation" );
}


const std::string& DRRStage::uniformName( unsigned int role ) const
{
    const static std::string ROLE_INTENSITIES_NAME = "huVolume";
    switch( role )
    {

    case ROLE_INTENSITIES:
        return ROLE_INTENSITIES_NAME;

    default:
        LIBCARNA_FAIL( "unknown role" );

    }
}


void DRRStage::configureShader()
{
    base::ShaderUniform< float >( "waterAttenuation", pimpl->waterAttenuation ).upload();
    base::ShaderUniform< float >(   "lowerThreshold", Details::huvToIntensity( pimpl->lowerThreshold ) ).upload();
    base::ShaderUniform< float >(   "upperThreshold", Details::huvToIntensity( pimpl->upperThreshold ) ).upload();
    base::ShaderUniform< float >(  "upperMultiplier", pimpl->upperMultiplier ).upload();
}


void DRRStage::configureShader( const base::Renderable& )
{
}



}  // namespace LibCarna :: presets

}  // namespace LibCarna
