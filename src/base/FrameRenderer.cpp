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

#include <Carna/base/glew.h>
#include <Carna/base/glError.h>
#include <Carna/base/FrameRenderer.h>
#include <Carna/base/Camera.h>
#include <Carna/base/RenderTask.h>
#include <Carna/base/RenderStage.h>
#include <Carna/base/Node.h>
#include <Carna/base/Viewport.h>
#include <Carna/base/ShaderManager.h>
#include <Carna/base/ShaderUniform.h>
#include <Carna/base/Vertex.h>
#include <Carna/base/Mesh.h>
#include <Carna/base/VertexBuffer.h>
#include <Carna/base/IndexBuffer.h>
#include <Carna/base/Sampler.h>
#include <vector>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// createFullFrameQuadSampler
// ----------------------------------------------------------------------------------

static Sampler* createFullFrameQuadSampler()
{
    Sampler* const sampler = new Sampler();

    sampler->setMinFilter( Sampler::FILTER_LINEAR );
    sampler->setMagFilter( Sampler::FILTER_LINEAR );

    sampler->setWrapModeR( Sampler::WRAP_MODE_CLAMP );
    sampler->setWrapModeS( Sampler::WRAP_MODE_CLAMP );
    sampler->setWrapModeT( Sampler::WRAP_MODE_CLAMP );

    return sampler;
}



// ----------------------------------------------------------------------------------
// glContextMadeCurrent
// ----------------------------------------------------------------------------------

static GLContext& glContextMadeCurrent( GLContext& glc )
{
    glc.makeCurrent();
    return glc;
}



// ----------------------------------------------------------------------------------
// createFullFrameQuadMesh
// ----------------------------------------------------------------------------------

static MeshBase& createFullFrameQuadMesh()
{
    /* Lets create clipping coordinates directly,
     * s.t. we won't need to pass any matrices to the shader.
     */
    VertexBase vertices[ 4 ];
    uint8_t indices[ 4 ];

    vertices[ 0 ].x = -1;
    vertices[ 0 ].y = -1;
    indices [ 0 ] = 0;

    vertices[ 1 ].x = +1;
    vertices[ 1 ].y = -1;
    indices [ 1 ] = 1;

    vertices[ 2 ].x = +1;
    vertices[ 2 ].y = +1;
    indices [ 2 ] = 2;

    vertices[ 3 ].x = -1;
    vertices[ 3 ].y = +1;
    indices [ 3 ] = 3;

    return Mesh< VertexBase, uint8_t >::create
        ( IndexBufferBase::PRIMITIVE_TYPE_TRIANGLE_FAN, vertices, 4, indices, 4 );
}



// ----------------------------------------------------------------------------------
// FrameRenderer :: Details
// ----------------------------------------------------------------------------------

struct FrameRenderer::Details
{
    Details( GLContext& glContext, unsigned int width, unsigned int height );

    std::vector< RenderStage* > stages;

    unsigned int width, height;
    
    bool fitSquare;

    mutable bool reshaped;

    std::unique_ptr< Viewport > viewport;

    GLContext* const glContext;

    const std::unique_ptr< Sampler > fullFrameQuadSampler;

    MeshBase& fullFrameQuadMesh;
    std::unique_ptr< MeshBase::VideoResourceAcquisition > fullFrameQuadMeshVR;

    const ShaderProgram& fullFrameQuadShader;

    float backgroundColor[ 4 ];
    bool backgroundColorChanged;
};


FrameRenderer::Details::Details( GLContext& glContext, unsigned int width, unsigned int height )
    : width( width )
    , height( height )
    , reshaped( true )
    , glContext( &glContextMadeCurrent( glContext ) )
    , fullFrameQuadSampler( createFullFrameQuadSampler() )
    , fullFrameQuadMesh( createFullFrameQuadMesh() )
    , fullFrameQuadMeshVR( new MeshBase::VideoResourceAcquisition( fullFrameQuadMesh ) )
    , fullFrameQuadShader( ShaderManager::instance().acquireShader( "full_frame_quad" ) )
    , backgroundColorChanged( true )
{
    backgroundColor[ 0 ] = 0;
    backgroundColor[ 1 ] = 0;
    backgroundColor[ 2 ] = 0;
    backgroundColor[ 3 ] = 0;
}



// ----------------------------------------------------------------------------------
// FrameRenderer :: RenderTextureParams
// ----------------------------------------------------------------------------------

FrameRenderer::RenderTextureParams::RenderTextureParams( unsigned int unit )
    : unit( unit )
    , useDefaultSampler( true )
    , useDefaultShader( true )
    , textureUniformName( "colorMap" )
    , alphaFactor( 1 )
{
}



// ----------------------------------------------------------------------------------
// FrameRenderer
// ----------------------------------------------------------------------------------

FrameRenderer::FrameRenderer( GLContext& glContext, unsigned int width, unsigned int height, bool fitSquare )
    : pimpl( new Details( glContext, width, height ) )
{
    pimpl->viewport.reset( new Viewport( *this, fitSquare ) );
    pimpl->fitSquare = fitSquare;
}


FrameRenderer::~FrameRenderer()
{
    /* Context is activated by 'clearStages'. The context must be activated s.t. the
     * quad mesh and shader can be cleaned up properly.
     */
    clearStages();
    ShaderManager::instance().releaseShader( pimpl->fullFrameQuadShader );
    pimpl->fullFrameQuadMesh.release();
}


void FrameRenderer::setBackgroundColor( const math::Vector4f& bc )
{
    pimpl->backgroundColor[ 0 ] = bc.x();
    pimpl->backgroundColor[ 1 ] = bc.y();
    pimpl->backgroundColor[ 2 ] = bc.z();
    pimpl->backgroundColor[ 3 ] = bc.w();
    pimpl->backgroundColorChanged = true;
}


GLContext& FrameRenderer::glContext() const
{
    return *pimpl->glContext;
}


std::size_t FrameRenderer::stages() const
{
    return pimpl->stages.size();
}


void FrameRenderer::appendStage( RenderStage* rs )
{
    pimpl->stages.push_back( rs );
}

    
void FrameRenderer::clearStages()
{
    pimpl->glContext->makeCurrent();
    std::for_each( pimpl->stages.begin(), pimpl->stages.end(), std::default_delete< RenderStage >() );
    pimpl->stages.clear();
}


RenderStage& FrameRenderer::stageAt( std::size_t position ) const
{
    return *pimpl->stages[ position ];
}


unsigned int FrameRenderer::width() const
{
    return pimpl->width;
}


unsigned int FrameRenderer::height() const
{
    return pimpl->height;
}


void FrameRenderer::reshape( unsigned int width, unsigned int height, bool fitSquare )
{
    pimpl->width = width;
    pimpl->height = height;
    pimpl->fitSquare = fitSquare;
    pimpl->viewport.reset( new Viewport( *this, fitSquare ) );
    pimpl->reshaped = true;
}


void FrameRenderer::reshape( unsigned int width, unsigned int height )
{
    reshape( width, height, pimpl->fitSquare );
}


void FrameRenderer::render( Camera& cam ) const
{
    Node& root = cam.findRoot();
    CARNA_ASSERT( static_cast< Spatial* >( &root ) != static_cast< Spatial* >( &cam ) );
    render( cam, root );
}


void FrameRenderer::render( Camera& cam, Node& root ) const
{
    /* Update world transforms.
     */
    root.updateWorldTransform();

    glContext().makeCurrent();
    render( cam, root, *pimpl->viewport );
}


const Viewport& FrameRenderer::viewport() const
{
    return *pimpl->viewport;
}


void FrameRenderer::render( Camera& cam, Node& root, const Viewport& vp ) const
{
    /* Check for errors.
     */
    REPORT_GL_ERROR;

    /* Reshape render stages' buffers.
     */
    for( auto rsItr = pimpl->stages.begin(); rsItr != pimpl->stages.end(); ++rsItr )
    {
        RenderStage& rs = **rsItr;
        
        /* Ensure buffers are properly sized.
         */
        if( pimpl->reshaped || !rs.isInitialized() )
        {
            rs.reshape( *this, *pimpl->viewport );
        }

        /* Notify stages of beginning frame.
         */
        rs.prepareFrame( root );
    }
    
    /* Mark that all buffer sizes have been established.
     */
    pimpl->reshaped = false;

    /* Clear buffers.
     */
    glClearColor
        ( pimpl->backgroundColor[ 0 ]
        , pimpl->backgroundColor[ 1 ]
        , pimpl->backgroundColor[ 2 ]
        , pimpl->backgroundColor[ 3 ] );
    
    /* Render frame.
     */
    RenderTask task( *this, cam.projection(), cam.viewTransform() );
    task.render( vp, GLContext::COLOR_BUFFER_BIT | GLContext::DEPTH_BUFFER_BIT );

    /* Check for errors.
     */
    REPORT_GL_ERROR;
}


void FrameRenderer::renderTexture( const RenderTextureParams& params ) const
{
    if( params.useDefaultSampler )
    {
        pimpl->fullFrameQuadSampler->bind( params.unit );
    }
    if( params.useDefaultShader )
    {
        pimpl->glContext->setShader( pimpl->fullFrameQuadShader );
        ShaderUniform< float >( "alphaFactor", params.alphaFactor ).upload();
    }

    ShaderUniform< int >( params.textureUniformName, params.unit ).upload();
    pimpl->fullFrameQuadMeshVR->render();
}



}  // namespace Carna :: base

}  // namespace Carna
