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

#include <Carna/presets/DRRStage.h>
#include <Carna/base/glew.h>
#include <Carna/base/ShaderManager.h>
#include <Carna/base/Framebuffer.h>
#include <Carna/base/RenderTexture.h>
#include <Carna/base/Viewport.h>
#include <Carna/base/RenderState.h>
#include <Carna/base/ShaderUniform.h>
#include <Carna/base/math.h>
#include <Carna/base/CarnaException.h>

namespace Carna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// DRRStage :: DRRStage
// ----------------------------------------------------------------------------------

struct DRRStage::Details
{

    Details();

    float stepLength;
    float waterAttenuation;
    float baseIntensity;
    base::HUV lowerThreshold;
    base::HUV upperThreshold;
    float upperMultiplier;
    bool renderInverse;

    std::unique_ptr< base::RenderTexture > accumulationColorBuffer;
    std::unique_ptr< base::Framebuffer   > accumulationFrameBuffer;

    const base::ShaderProgram* exponentialShader;

    static inline float huvToIntensity( base::HUV huv )
    {
        return ( huv + 1024 ) / 4095.f;
    }

}; // MIPStage :: Details


DRRStage::Details::Details()
    : waterAttenuation( 2.f )
    , baseIntensity( 1.f )
    , lowerThreshold( -400 )
    , upperThreshold( +400 )
    , upperMultiplier( 1.5f )
    , renderInverse( false )
    , exponentialShader( nullptr )
{
}



// ----------------------------------------------------------------------------------
// DRRStage
// ----------------------------------------------------------------------------------

DRRStage::DRRStage( unsigned int geometryType )
    : RayMarchingStage( geometryType )
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
    CARNA_ASSERT( muWater > 0 );
    pimpl->waterAttenuation = muWater;
}


void DRRStage::setBaseIntensity( float baseIntensity )
{
    CARNA_ASSERT( baseIntensity > 0 );
    pimpl->baseIntensity = baseIntensity;
}


void DRRStage::setLowerThreshold( base::HUV lower )
{
    CARNA_ASSERT( lower >= -1024 && lower <= 3071 );
    pimpl->lowerThreshold = lower;
}


void DRRStage::setUpperThreshold( base::HUV upper )
{
    CARNA_ASSERT( upper >= -1024 && upper <= 3071 );
    pimpl->upperThreshold = upper;
}


void DRRStage::setUpperMultiplier( float multiplier )
{
    CARNA_ASSERT( multiplier >= 0 );
    pimpl->upperMultiplier = multiplier;
}


void DRRStage::setRenderingInverse( bool inverse )
{
    pimpl->renderInverse = inverse;
}


void DRRStage::reshape( const base::FrameRenderer& fr, const base::Viewport& vp )
{
    base::GeometryStage< base::Renderable::DepthOrder< base::Renderable::ORDER_BACK_TO_FRONT > >::reshape( fr, vp );
    pimpl->accumulationColorBuffer.reset( new base::RenderTexture( vp.width, vp.height, true ) );
    pimpl->accumulationFrameBuffer.reset( new base::Framebuffer( *pimpl->accumulationColorBuffer ) );
}


void DRRStage::loadVideoResources()
{
    RayMarchingStage::loadVideoResources();
    pimpl->exponentialShader = &base::ShaderManager::instance().acquireShader( "drr_exponential" );
}


void DRRStage::renderPass
    ( const base::math::Matrix4f& vt
    , base::RenderTask& rt
    , const base::Viewport& outputViewport )
{
    const base::Viewport framebufferViewport
        ( outputViewport, 0, 0
        , pimpl->accumulationFrameBuffer->width()
        , pimpl->accumulationFrameBuffer->height() );

    /* Configure OpenGL state that is common to both following passes.
     */
    base::RenderState rs( rt.renderer.glContext() );
    rs.setBlend( true );
    rs.setDepthWrite( false );

    /* Compute step length for ray marching in model space.
     */
    pimpl->stepLength = 1.f / sampleRate();

    /* Copy depth buffer from output to the accumulation frame buffer.
     */
    const unsigned int outputFramebufferId = base::Framebuffer::currentId();
    base::Framebuffer::copy
        ( outputFramebufferId
        , pimpl->accumulationFrameBuffer->id
        , outputViewport
        , framebufferViewport
        , GL_DEPTH_BUFFER_BIT );

    /* First, evaluate the integral by rendering to the accumulation buffer.
     */
    CARNA_RENDER_TO_FRAMEBUFFER( *pimpl->accumulationFrameBuffer,

        /* Configure OpenGL state for accumulation pass.
         */
        base::RenderState rs2( rt.renderer.glContext() );
        rs2.setBlendFunction( base::BlendFunction( GL_ONE, GL_ONE ) );

        glClearColor( 0, 0, 0, 0 );
        rt.renderer.glContext().clearBuffers( GL_COLOR_BUFFER_BIT );

        framebufferViewport.makeActive();
        RayMarchingStage::renderPass( vt, rt, framebufferViewport );
        framebufferViewport.done();

    );

    /* Now compute the exponential of the integral.
     */
    rs.setDepthTest( false );
    rt.renderer.glContext().setShader( *pimpl->exponentialShader );
    base::ShaderUniform< float >( "baseIntensity", pimpl->baseIntensity ).upload();
    base::ShaderUniform< int >( "renderInverse", pimpl->renderInverse ? 1 : 0 ).upload();
    pimpl->accumulationColorBuffer->bind( 0 );
    base::FrameRenderer::RenderTextureParams params( 0 );
    params.useDefaultShader = false;
    params.textureUniformName = "integralMap";
    rt.renderer.renderTexture( params );
}


void DRRStage::createSamplers( const std::function< void( unsigned int, base::Sampler* ) >& registerSampler )
{
    base::Sampler* const huVolumeSampler = new base::Sampler();
    huVolumeSampler->setMinFilter( base::Sampler::FILTER_LINEAR );
    huVolumeSampler->setMagFilter( base::Sampler::FILTER_LINEAR );
    huVolumeSampler->setWrapModeR( base::Sampler::WRAP_MODE_CLAMP );
    huVolumeSampler->setWrapModeS( base::Sampler::WRAP_MODE_CLAMP );
    huVolumeSampler->setWrapModeT( base::Sampler::WRAP_MODE_CLAMP );
    registerSampler( ROLE_HU_VOLUME, huVolumeSampler );
}


const base::ShaderProgram& DRRStage::loadShader()
{
    return base::ShaderManager::instance().acquireShader( "drr_accumulation" );
}


const std::string& DRRStage::uniformName( unsigned int role ) const
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


void DRRStage::configureShader()
{
    base::ShaderUniform< float >(       "stepLength", pimpl->stepLength ).upload();
    base::ShaderUniform< float >( "waterAttenuation", pimpl->waterAttenuation ).upload();
    base::ShaderUniform< float >(   "lowerThreshold", Details::huvToIntensity( pimpl->lowerThreshold ) ).upload();
    base::ShaderUniform< float >(   "upperThreshold", Details::huvToIntensity( pimpl->upperThreshold ) ).upload();
    base::ShaderUniform< float >(  "upperMultiplier", pimpl->upperMultiplier ).upload();
}



}  // namespace Carna :: presets

}  // namespace Carna
