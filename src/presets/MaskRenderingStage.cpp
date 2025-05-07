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

#include <LibCarna/presets/MaskRenderingStage.hpp>
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
// MaskRenderingStage :: Details
// ----------------------------------------------------------------------------------

struct MaskRenderingStage::Details
{

    Details();

    base::Color color;
    bool filling;

    std::unique_ptr< base::Texture< 2 > > accumulationColorBuffer;
    std::unique_ptr< base::Framebuffer  > accumulationFrameBuffer;

    const base::ShaderProgram* edgeDetectShader;

    base::math::Vector2f textureSteps;

    std::unique_ptr< base::Sampler > labelMapSampler;

}; // MaskRenderingStage :: Details


MaskRenderingStage::Details::Details()
    : color( MaskRenderingStage::DEFAULT_COLOR )
    , filling( MaskRenderingStage::DEFAULT_FILLING )
    , edgeDetectShader( nullptr )
{
}



// ----------------------------------------------------------------------------------
// MaskRenderingStage
// ----------------------------------------------------------------------------------

const base::Color  MaskRenderingStage::DEFAULT_COLOR = base::Color::GREEN;
const unsigned int MaskRenderingStage::DEFAULT_ROLE_MASK = 2;
const bool         MaskRenderingStage::DEFAULT_FILLING = true;


MaskRenderingStage::MaskRenderingStage( unsigned int geometryType, unsigned int maskRole )
    : VolumeRenderingStage( geometryType )
    , pimpl( new Details() )
    , maskRole( maskRole )
{
}


MaskRenderingStage::~MaskRenderingStage()
{
    if( pimpl->edgeDetectShader != nullptr )
    {
        activateGLContext();
        base::ShaderManager::instance().releaseShader( *pimpl->edgeDetectShader );
    }
}


const base::Color& MaskRenderingStage::color() const
{
    return pimpl->color;
}


void MaskRenderingStage::setColor( const base::Color& color )
{
    pimpl->color = color;
}


bool MaskRenderingStage::isFilling() const
{
    return pimpl->filling;
}


void MaskRenderingStage::setFilling( bool filling )
{
    pimpl->filling = filling;
}


void MaskRenderingStage::reshape( base::FrameRenderer& fr, unsigned int width, unsigned int height )
{
    base::RenderStage::reshape( fr, width, height );
    pimpl->accumulationColorBuffer.reset( base::Framebuffer::createRenderTexture( true ) );
    pimpl->accumulationFrameBuffer.reset( new base::Framebuffer( width, height, *pimpl->accumulationColorBuffer ) );
    pimpl->textureSteps = base::math::Vector2f( 1.f / ( width - 1 ), 1.f / ( height - 1 ) );
}


unsigned int MaskRenderingStage::loadVideoResources()
{
    pimpl->edgeDetectShader = &base::ShaderManager::instance().acquireShader( "mr_edgedetect" );
    pimpl->labelMapSampler.reset( new base::Sampler
        ( base::Sampler::WRAP_MODE_CLAMP, base::Sampler::WRAP_MODE_CLAMP, base::Sampler::WRAP_MODE_CLAMP
        , base::Sampler::FILTER_NEAREST, base::Sampler::FILTER_NEAREST ) );
    return VolumeRenderingStage::loadVideoResources();
}


void MaskRenderingStage::renderPass
    ( const base::math::Matrix4f& vt
    , base::RenderTask& rt
    , const base::Viewport& outputViewport )
{
    if( !pimpl->filling )
    {
        /* First, render the projected mask intensities.
         */
        const base::Viewport framebufferViewport( *pimpl->accumulationFrameBuffer );
        LIBCARNA_RENDER_TO_FRAMEBUFFER( *pimpl->accumulationFrameBuffer,

            /* Configure OpenGL state for accumulation pass.
             */
            base::RenderState rs;
            rs.setDepthTest( true );
            rs.setDepthWrite( true );

            glClearColor( 0, 0, 0, 0 );
            rt.renderer.glContext().clearBuffers( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

            framebufferViewport.makeActive();
            VolumeRenderingStage::renderPass( vt, rt, framebufferViewport );
            framebufferViewport.done();

        );

        /* Render result to output framebuffer.
         */
        base::RenderState rs;
        rs.setBlendFunction( base::BlendFunction( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) );
        rs.setBlend( true );

        base::FrameRenderer::RenderTextureParams params( 0 );
        if( !pimpl->filling )
        {
            rs.setDepthTest ( false );
            rs.setDepthWrite( false );
            rt.renderer.glContext().setShader( *pimpl->edgeDetectShader );
            base::ShaderUniform< base::math::Vector4f >( "color", pimpl->color ).upload();
            base::ShaderUniform< base::math::Vector2f >( "steps", pimpl->textureSteps ).upload();
            params.useDefaultShader = false;
            params.textureUniformName = "labelMap";
            params.useDefaultSampler = false;
            pimpl->labelMapSampler->bind( params.unit );
        }
        pimpl->accumulationColorBuffer->bind( 0 );
        rt.renderer.renderTexture( params );
    }
    else
    {
        /* Render directly to output framebuffer.
         */
        base::RenderState rs;
        rs.setDepthTest( true );
        rs.setDepthWrite( true );
        VolumeRenderingStage::renderPass( vt, rt, outputViewport );
    }
}


void MaskRenderingStage::createVolumeSamplers( const std::function< void( unsigned int, base::Sampler* ) >& registerSampler )
{
    /* Create sampler for the mask texture.
     */
    registerSampler( maskRole, new base::Sampler
        ( base::Sampler::WRAP_MODE_CLAMP, base::Sampler::WRAP_MODE_CLAMP, base::Sampler::WRAP_MODE_CLAMP
        , base::Sampler::FILTER_NEAREST, base::Sampler::FILTER_NEAREST ) );
}


const base::ShaderProgram& MaskRenderingStage::acquireShader()
{
    return base::ShaderManager::instance().acquireShader( "mr" );
}


const std::string& MaskRenderingStage::uniformName( unsigned int role ) const
{
    const static std::string ROLE_MASK_NAME = "mask";
    if( role == maskRole )
    {
        return ROLE_MASK_NAME;
    }
    else
    {
        LIBCARNA_FAIL( "Unknown role: " + base::text::lexical_cast< std::string >( role ) );
    }
}


void MaskRenderingStage::configureShader()
{
    base::ShaderUniform< bool >( "ignoreColor", !pimpl->filling ).upload();
    if( pimpl->filling )
    {
        base::ShaderUniform< base::math::Vector4f >( "color", pimpl->color ).upload();
    }
}


void MaskRenderingStage::configureShader( const base::Renderable& )
{
}



}  // namespace LibCarna :: presets

}  // namespace LibCarna
