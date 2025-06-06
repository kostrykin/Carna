﻿/*
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

#include <LibCarna/presets/MIPStage.hpp>
#include <LibCarna/base/glew.hpp>
#include <LibCarna/base/ShaderManager.hpp>
#include <LibCarna/base/ShaderUniform.hpp>
#include <LibCarna/base/Framebuffer.hpp>
#include <LibCarna/base/Viewport.hpp>
#include <LibCarna/base/RenderState.hpp>
#include <LibCarna/base/math.hpp>
#include <LibCarna/base/LibCarnaException.hpp>
#include <vector>
#include <algorithm>

namespace LibCarna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// MIPStage :: Details
// ----------------------------------------------------------------------------------

struct MIPStage::Details
{

    Details();

    std::unique_ptr< base::Texture< 2 > > projectionColorBuffer;
    std::unique_ptr< base::Framebuffer  > projectionFrameBuffer;

    const base::ShaderProgram* colorizationShader;
    
    const static unsigned int COLORMAP_TEXTURE_UNIT = base::Texture< 0 >::SETUP_UNIT + 1;

}; // MIPStage :: Details


MIPStage::Details::Details()
    : colorizationShader( nullptr )
{
}



// ----------------------------------------------------------------------------------
// MIPStage
// ----------------------------------------------------------------------------------

MIPStage::MIPStage( unsigned int geometryType, unsigned int colorMapResolution )
    : VolumeRenderingStage( geometryType )
    , pimpl( new Details() )
    , colorMap( colorMapResolution )
{
}


MIPStage::~MIPStage()
{
    activateGLContext();

    if( pimpl->colorizationShader != nullptr )
    {
        base::ShaderManager::instance().releaseShader( *pimpl->colorizationShader );
    }
}


void MIPStage::reshape( base::FrameRenderer& fr, unsigned int width, unsigned int height )
{
    base::RenderStage::reshape( fr, width, height );
    pimpl->projectionColorBuffer.reset( base::Framebuffer::createRenderTexture( true ) );
    pimpl->projectionFrameBuffer.reset( new base::Framebuffer( width, height, *pimpl->projectionColorBuffer ) );
}


unsigned int MIPStage::loadVideoResources()
{
    pimpl->colorizationShader = &base::ShaderManager::instance().acquireShader( "mip_colorization" );
    VolumeRenderingStage::loadVideoResources();
    return Details::COLORMAP_TEXTURE_UNIT + 1;
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
    const base::Viewport framebufferViewport( *pimpl->projectionFrameBuffer );
    const unsigned int outputFramebufferId = base::Framebuffer::currentId();
    base::Framebuffer::copyDepthAttachment
        ( outputFramebufferId
        , pimpl->projectionFrameBuffer->id
        , outputViewport
        , framebufferViewport );

    /* First render the projection of the intensities into the dedicated framebuffer.
     */
    LIBCARNA_RENDER_TO_FRAMEBUFFER( *pimpl->projectionFrameBuffer,

        base::RenderState rs;
        rs.setBlendEquation( GL_MAX );
        rs.setDepthTest( true );

        glClearColor( -1, 0, 0, 0 );
        rt.renderer.glContext().clearBuffers( GL_COLOR_BUFFER_BIT );

        framebufferViewport.makeActive();
        VolumeRenderingStage::renderPass( vt, rt, framebufferViewport );
        framebufferViewport.done();

    );

    /* Render result to output framebuffer w.r.t. the color map.
     */
    rs.setBlendFunction( base::BlendFunction( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) );

    rt.renderer.glContext().setShader( *pimpl->colorizationShader );
    base::ShaderUniform< int >( "colorMap", Details::COLORMAP_TEXTURE_UNIT ).upload();
    base::ShaderUniform< float >( "minIntensity", colorMap.minimumIntensity() ).upload();
    base::ShaderUniform< float >( "maxIntensity", colorMap.maximumIntensity() ).upload();
    colorMap.bind( Details::COLORMAP_TEXTURE_UNIT );

    pimpl->projectionColorBuffer->bind( 0 );
    base::FrameRenderer::RenderTextureParams params( 0 );
    params.useDefaultShader = false;
    params.textureUniformName = "mip";
    rt.renderer.renderTexture( params );
}


void MIPStage::createVolumeSamplers( const std::function< void( unsigned int, base::Sampler* ) >& registerSampler )
{
    registerSampler( ROLE_INTENSITIES, new base::Sampler
        ( base::Sampler::WRAP_MODE_CLAMP, base::Sampler::WRAP_MODE_CLAMP, base::Sampler::WRAP_MODE_CLAMP
        , base::Sampler::FILTER_LINEAR, base::Sampler::FILTER_LINEAR ) );
}


const base::ShaderProgram& MIPStage::acquireShader()
{
    return base::ShaderManager::instance().acquireShader( "mip" );
}


const std::string& MIPStage::uniformName( unsigned int role ) const
{
    const static std::string ROLE_INTENSITIES_NAME = "intensities";
    switch( role )
    {

    case ROLE_INTENSITIES:
        return ROLE_INTENSITIES_NAME;

    default:
        LIBCARNA_FAIL( "unknown role" );

    }
}


void MIPStage::configureShader()
{
}


void MIPStage::configureShader( const base::Renderable& renderable )
{
    LIBCARNA_ASSERT( renderable.geometry().hasFeature( ROLE_INTENSITIES ) );
}



}  // namespace LibCarna :: VolumeRenderings

}  // namespace LibCarna
