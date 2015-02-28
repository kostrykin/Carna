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

#include <Carna/VolumeRenderings/MIP/MIPStage.h>
#include <Carna/VolumeRenderings/MIP/Channel.h>
#include <Carna/base/view/glew.h>
#include <Carna/base/view/ShaderManager.h>
#include <Carna/base/view/Framebuffer.h>
#include <Carna/base/view/RenderTexture.h>
#include <Carna/base/view/Viewport.h>
#include <Carna/base/view/RenderState.h>
#include <Carna/base/math.h>
#include <Carna/base/CarnaException.h>
#include <vector>
#include <algorithm>

namespace Carna
{

namespace VolumeRenderings
{

namespace MIP
{



// ----------------------------------------------------------------------------------
// MIPStage :: Details
// ----------------------------------------------------------------------------------

struct MIPStage::Details
{

    Details();

    Channel* currentChannel;
    std::vector< Channel* > channels;

    std::unique_ptr< base::view::RenderTexture > channelColorBuffer;
    std::unique_ptr< base::view::Framebuffer   > channelFrameBuffer;

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

MIPStage::MIPStage( int geometryType )
    : RayMarchingStage( geometryType )
    , pimpl( new Details() )
{
}


MIPStage::~MIPStage()
{
    clearChannels();
}


void MIPStage::appendChannel( Channel* channel )
{
    CARNA_ASSERT( std::find( pimpl->channels.begin(), pimpl->channels.end(), channel ) == pimpl->channels.end() );
    pimpl->channels.push_back( channel );
}


Channel* MIPStage::removeChannel( const Channel& channel )
{
    const auto channelItr = std::find( pimpl->channels.begin(), pimpl->channels.end(), const_cast< Channel* >( &channel ) );
    CARNA_ASSERT( channelItr != pimpl->channels.end() );
    pimpl->channels.erase( channelItr );
    return *channelItr;
}


void MIPStage::ascendChannel( const Channel& channel )
{
    const auto channelItr = std::find( pimpl->channels.begin(), pimpl->channels.end(), const_cast< Channel* >( &channel ) );
    CARNA_ASSERT( channelItr != pimpl->channels.end() );
    if( channelItr != pimpl->channels.begin() )
    {
        std::swap( *channelItr, *( channelItr - 1 ) );
    }
}


void MIPStage::clearChannels()
{
    std::for_each( pimpl->channels.begin(), pimpl->channels.end(), std::default_delete< Channel >() );
    pimpl->channels.clear();
}


void MIPStage::reshape( const base::view::FrameRenderer& fr, const base::view::Viewport& vp )
{
    base::view::GeometryStage< base::view::Renderable::DepthOrder< base::view::Renderable::ORDER_BACK_TO_FRONT > >::reshape( fr, vp );
    pimpl->channelColorBuffer.reset( new base::view::RenderTexture( vp.width, vp.height ) );
    pimpl->channelFrameBuffer.reset( new base::view::Framebuffer( *pimpl->channelColorBuffer ) );
}


void MIPStage::renderPass
    ( const base::math::Matrix4f& vt
    , base::view::RenderTask& rt
    , const base::view::Viewport& vp )
{
    const base::view::Viewport framebufferViewport
        ( vp, 0, 0
        , pimpl->channelFrameBuffer->width()
        , pimpl->channelFrameBuffer->height() );

    /* Configure proper OpenGL state.
     */
    base::view::RenderState rs( rt.renderer.glContext() );
    rs.setBlend( true );
    rs.setDepthTest( false );
    rs.setDepthWrite( false );

    /* For each channel: First render the channel-specific MIP result to the dedicated framebuffer,
     * than render the result to the output framebuffer w.r.t. the channel function.
     */
    for( auto channelItr = pimpl->channels.begin(); channelItr != pimpl->channels.end(); ++channelItr )
    {
        pimpl->currentChannel = *channelItr;

        /* Render to dedicated framebuffer.
         */
        CARNA_RENDER_TO_FRAMEBUFFER( *pimpl->channelFrameBuffer,

            base::view::RenderState rs2( rt.renderer.glContext() );
            rs2.setBlendEquation( GL_MAX );

            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

            framebufferViewport.makeActive();
            RayMarchingStage::renderPass( vt, rt, framebufferViewport );
            framebufferViewport.done();

        );

        /* Render result to output framebuffer.
         */
        base::view::RenderState rs2( rt.renderer.glContext() );
        rs2.setBlendFunction( pimpl->currentChannel->function );

        pimpl->channelColorBuffer->bind( 0 );
        rt.renderer.renderTexture( 0, true );
    }

    /* Denote that we're finished with rendering.
     */
    pimpl->currentChannel = nullptr;
}


void MIPStage::createSamplers( const std::function< void( unsigned int, base::view::Sampler* ) >& registerSampler )
{
    base::view::Sampler* const huVolumeSampler = new base::view::Sampler();
    huVolumeSampler->setMinFilter( base::view::Sampler::FILTER_LINEAR );
    huVolumeSampler->setMagFilter( base::view::Sampler::FILTER_LINEAR );
    huVolumeSampler->setWrapModeR( base::view::Sampler::WRAP_MODE_CLAMP );
    huVolumeSampler->setWrapModeS( base::view::Sampler::WRAP_MODE_CLAMP );
    huVolumeSampler->setWrapModeT( base::view::Sampler::WRAP_MODE_CLAMP );
    registerSampler( ROLE_HU_VOLUME, huVolumeSampler );
}


const base::view::ShaderProgram& MIPStage::loadShader()
{
    return base::view::ShaderManager::instance().acquireShader( "mip" );
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


void MIPStage::configureShader( base::view::GLContext& glc )
{
    CARNA_ASSERT( pimpl->currentChannel != nullptr );
    const Channel& ch = *pimpl->currentChannel;

    base::view::ShaderProgram::putUniform1f( "minIntensity", Details::huvToIntensity( ch.huRange.first ) );
    base::view::ShaderProgram::putUniform1f( "maxIntensity", Details::huvToIntensity( ch.huRange.last  ) );
    base::view::ShaderProgram::putUniform4f( "color", ch.color );
}



}  // namespace Carna :: VolumeRenderings :: MIP

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna
