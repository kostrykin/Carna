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
#include <Carna/presets/MIPChannel.h>
#include <Carna/base/glew.h>
#include <Carna/base/ShaderManager.h>
#include <Carna/base/ShaderUniform.h>
#include <Carna/base/Framebuffer.h>
#include <Carna/base/RenderTexture.h>
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

    MIPChannel* currentChannel;
    std::vector< MIPChannel* > channels;

    std::unique_ptr< base::RenderTexture > channelColorBuffer;
    std::unique_ptr< base::Framebuffer   > channelFrameBuffer;

    static inline float huvToIntensity( signed short huv )
    {
        return ( huv + 1024 ) / 4095.f;
    }

}; // MIPStage :: Details


MIPStage::Details::Details()
    : currentChannel( nullptr )
{
}



// ----------------------------------------------------------------------------------
// MIPStage
// ----------------------------------------------------------------------------------

MIPStage::MIPStage( unsigned int geometryType )
    : RayMarchingStage( geometryType )
    , pimpl( new Details() )
{
}


MIPStage::~MIPStage()
{
    clearChannels();
}


void MIPStage::appendChannel( MIPChannel* channel )
{
    CARNA_ASSERT( std::find( pimpl->channels.begin(), pimpl->channels.end(), channel ) == pimpl->channels.end() );
    pimpl->channels.push_back( channel );
}


MIPChannel* MIPStage::removeChannel( const MIPChannel& channel )
{
    const auto channelItr = std::find( pimpl->channels.begin(), pimpl->channels.end(), const_cast< MIPChannel* >( &channel ) );
    CARNA_ASSERT( channelItr != pimpl->channels.end() );
    pimpl->channels.erase( channelItr );
    return *channelItr;
}


void MIPStage::ascendChannel( const MIPChannel& channel )
{
    const auto channelItr = std::find( pimpl->channels.begin(), pimpl->channels.end(), const_cast< MIPChannel* >( &channel ) );
    CARNA_ASSERT( channelItr != pimpl->channels.end() );
    if( channelItr != pimpl->channels.begin() )
    {
        std::swap( *channelItr, *( channelItr - 1 ) );
    }
}


void MIPStage::clearChannels()
{
    std::for_each( pimpl->channels.begin(), pimpl->channels.end(), std::default_delete< MIPChannel >() );
    pimpl->channels.clear();
}


void MIPStage::reshape( const base::FrameRenderer& fr, const base::Viewport& vp )
{
    base::GeometryStage< base::Renderable::DepthOrder< base::Renderable::ORDER_BACK_TO_FRONT > >::reshape( fr, vp );
    pimpl->channelColorBuffer.reset( new base::RenderTexture( vp.width, vp.height ) );
    pimpl->channelFrameBuffer.reset( new base::Framebuffer( *pimpl->channelColorBuffer ) );
}


void MIPStage::renderPass
    ( const base::math::Matrix4f& vt
    , base::RenderTask& rt
    , const base::Viewport& outputViewport )
{
    const base::Viewport framebufferViewport
        ( outputViewport, 0, 0
        , pimpl->channelFrameBuffer->width()
        , pimpl->channelFrameBuffer->height() );

    /* Configure proper OpenGL state.
     */
    base::RenderState rs( rt.renderer.glContext() );
    rs.setBlend( true );
    rs.setDepthTest( false );
    rs.setDepthWrite( false );

    /* Copy depth buffer from output to dedicated frame buffer.
     */
    const unsigned int outputFramebufferId = base::Framebuffer::currentId();
    base::Framebuffer::copy
        ( outputFramebufferId
        , pimpl->channelFrameBuffer->id
        , outputViewport
        , framebufferViewport
        , GL_DEPTH_BUFFER_BIT );

    /* For each channel: First render the channel-specific MIP result to the dedicated framebuffer,
     * than render the result to the output framebuffer w.r.t. the channel function.
     */
    for( auto channelItr = pimpl->channels.begin(); channelItr != pimpl->channels.end(); ++channelItr )
    {
        pimpl->currentChannel = *channelItr;

        /* Render to dedicated framebuffer.
         */
        CARNA_RENDER_TO_FRAMEBUFFER( *pimpl->channelFrameBuffer,

            base::RenderState rs2( rt.renderer.glContext() );
            rs2.setBlendEquation( GL_MAX );
            rs2.setDepthTest( true );

            glClearColor( 0, 0, 0, 0 );
            rt.renderer.glContext().clearBuffers( GL_COLOR_BUFFER_BIT );

            framebufferViewport.makeActive();
            RayMarchingStage::renderPass( vt, rt, framebufferViewport );
            framebufferViewport.done();

        );

        /* Render result to output framebuffer.
         */
        base::RenderState rs2( rt.renderer.glContext() );
        rs2.setBlendFunction( pimpl->currentChannel->function );

        pimpl->channelColorBuffer->bind( 0 );
        rt.renderer.renderTexture( 0, true );
    }

    /* Denote that we're finished with rendering.
     */
    pimpl->currentChannel = nullptr;
}


void MIPStage::createSamplers( const std::function< void( unsigned int, base::Sampler* ) >& registerSampler )
{
    base::Sampler* const huVolumeSampler = new base::Sampler();
    huVolumeSampler->setMinFilter( base::Sampler::FILTER_LINEAR );
    huVolumeSampler->setMagFilter( base::Sampler::FILTER_LINEAR );
    huVolumeSampler->setWrapModeR( base::Sampler::WRAP_MODE_CLAMP );
    huVolumeSampler->setWrapModeS( base::Sampler::WRAP_MODE_CLAMP );
    huVolumeSampler->setWrapModeT( base::Sampler::WRAP_MODE_CLAMP );
    registerSampler( ROLE_HU_VOLUME, huVolumeSampler );
}


const base::ShaderProgram& MIPStage::loadShader()
{
    return base::ShaderManager::instance().acquireShader( "mip" );
}


const std::string& MIPStage::uniformName( unsigned int role ) const
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


void MIPStage::configureShader( base::GLContext& glc )
{
    CARNA_ASSERT( pimpl->currentChannel != nullptr );
    const MIPChannel& ch = *pimpl->currentChannel;

    base::ShaderUniform< float >( "minIntensity", Details::huvToIntensity( ch.huRange.first ) ).upload();
    base::ShaderUniform< float >( "maxIntensity", Details::huvToIntensity( ch.huRange.last  ) ).upload();
    base::ShaderUniform< base::math::Vector4f >( "color", ch.color ).upload();
}



}  // namespace Carna :: VolumeRenderings

}  // namespace Carna
