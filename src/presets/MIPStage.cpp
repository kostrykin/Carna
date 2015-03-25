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

    std::unique_ptr< base::Texture< 2 > > channelColorBuffer;
    std::unique_ptr< base::Framebuffer  > channelFrameBuffer;

    static inline float huvToIntensity( base::HUV huv )
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
    : VolumeRenderingStage( geometryType )
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


std::size_t MIPStage::channelsCount() const
{
    return pimpl->channels.size();
}


MIPChannel& MIPStage::channel( std::size_t channelIndex )
{
    CARNA_ASSERT( channelIndex < channelsCount() );
    return *pimpl->channels[ channelIndex ];
}


const MIPChannel& MIPStage::channel( std::size_t channelIndex ) const
{
    CARNA_ASSERT( channelIndex < channelsCount() );
    return *pimpl->channels[ channelIndex ];
}


void MIPStage::reshape( const base::FrameRenderer& fr, unsigned int width, unsigned int height )
{
    base::GeometryStage< base::Renderable::BackToFront >::reshape( fr, width, height );
    pimpl->channelColorBuffer.reset( base::Framebuffer::createRenderTexture() );
    pimpl->channelFrameBuffer.reset( new base::Framebuffer( width, height, *pimpl->channelColorBuffer ) );
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
    const base::Viewport framebufferViewport( *pimpl->channelFrameBuffer );
    const unsigned int outputFramebufferId = base::Framebuffer::currentId();
    base::Framebuffer::copyDepthAttachment
        ( outputFramebufferId
        , pimpl->channelFrameBuffer->id
        , outputViewport
        , framebufferViewport );

    /* For each channel: First render the channel-specific MIP result to the dedicated framebuffer,
     * than render the result to the output framebuffer w.r.t. the channel function.
     */
    for( auto channelItr = pimpl->channels.begin(); channelItr != pimpl->channels.end(); ++channelItr )
    {
        pimpl->currentChannel = *channelItr;

        /* Render to dedicated framebuffer.
         */
        CARNA_RENDER_TO_FRAMEBUFFER( *pimpl->channelFrameBuffer,

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
        rs.setBlendFunction( pimpl->currentChannel->function() );

        pimpl->channelColorBuffer->bind( 0 );
        rt.renderer.renderTexture( base::FrameRenderer::RenderTextureParams( 0 ) );
    }

    /* Denote that we're finished with rendering.
     */
    pimpl->currentChannel = nullptr;
}


void MIPStage::createSamplers( const std::function< void( unsigned int, base::Sampler* ) >& registerSampler )
{
    registerSampler( ROLE_HU_VOLUME, new base::Sampler
        ( base::Sampler::WRAP_MODE_CLAMP, base::Sampler::WRAP_MODE_CLAMP, base::Sampler::WRAP_MODE_CLAMP
        , base::Sampler::FILTER_LINEAR, base::Sampler::FILTER_LINEAR ) );
}


const base::ShaderProgram& MIPStage::acquireShader()
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


void MIPStage::configureShader()
{
    CARNA_ASSERT( pimpl->currentChannel != nullptr );
    const MIPChannel& ch = *pimpl->currentChannel;

    base::ShaderUniform< float >( "minIntensity", Details::huvToIntensity( ch.huRange.first ) ).upload();
    base::ShaderUniform< float >( "maxIntensity", Details::huvToIntensity( ch.huRange.last  ) ).upload();
    base::ShaderUniform< base::math::Vector4f >( "color", ch.color ).upload();
}



}  // namespace Carna :: VolumeRenderings

}  // namespace Carna
