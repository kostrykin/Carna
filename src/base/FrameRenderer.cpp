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

#include <LibCarna/base/glew.h>
#include <LibCarna/base/glError.h>
#include <LibCarna/base/FrameRenderer.h>
#include <LibCarna/base/Camera.h>
#include <LibCarna/base/RenderTask.h>
#include <LibCarna/base/RenderStage.h>
#include <LibCarna/base/Node.h>
#include <LibCarna/base/Viewport.h>
#include <LibCarna/base/ShaderManager.h>
#include <LibCarna/base/ShaderUniform.h>
#include <LibCarna/base/Vertex.h>
#include <LibCarna/base/Mesh.h>
#include <LibCarna/base/VertexBuffer.h>
#include <LibCarna/base/IndexBuffer.h>
#include <LibCarna/base/Sampler.h>
#include <LibCarna/base/Stopwatch.h>
#include <LibCarna/base/Composition.h>
#include <vector>

namespace LibCarna
{

namespace base
{



// ----------------------------------------------------------------------------------
// createFullFrameQuadSampler
// ----------------------------------------------------------------------------------

static Sampler* createFullFrameQuadSampler()
{
    return new Sampler
        ( Sampler::WRAP_MODE_CLAMP, Sampler::WRAP_MODE_CLAMP, Sampler::WRAP_MODE_CLAMP
        , Sampler::FILTER_LINEAR, Sampler::FILTER_LINEAR );
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

static MeshBase* createFullFrameQuadMesh()
{
    typedef PVertex VertexType;
    typedef uint8_t IndexType;
    
    /* Lets create clipping coordinates directly,
     * s.t. we won't need to pass any matrices to the shader.
     */
    VertexType vertices[ 4 ];
     IndexType  indices[ 4 ];

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

    /* Create vertex buffer.
     */
    VertexBuffer< VertexType >* const vertexBuffer = new VertexBuffer< VertexType >();
    vertexBuffer->copy( vertices, 4 );
    
    /* Create index buffer.
     */
    IndexBuffer< IndexType >* const indexBuffer = new IndexBuffer< IndexType >( IndexBufferBase::PRIMITIVE_TYPE_TRIANGLE_FAN );
    indexBuffer->copy( indices, 4 );
     
    /* Compose the mesh.
     */
    return new Mesh< VertexType, IndexType >
        ( new Composition< VertexBufferBase >( vertexBuffer )
        , new Composition<  IndexBufferBase >(  indexBuffer ) );
}



// ----------------------------------------------------------------------------------
// circular_buffer
// ----------------------------------------------------------------------------------

template< typename T >
class circular_buffer
{

    std::vector< T > buffer;
    unsigned int nextIdx;
    bool saturated;

public:

    explicit circular_buffer( std::size_t size );
    void push( const T& );
    std::size_t size() const;
    const T& operator[]( std::size_t idx ) const;
    
}; // circular_buffer


template< typename T >
circular_buffer< T >::circular_buffer( std::size_t size )
    : buffer( size )
    , nextIdx( 0 )
    , saturated( false )
{
}


template< typename T >
void circular_buffer< T >::push( const T& val )
{
    buffer[ nextIdx ] = val;
    nextIdx = ( ++nextIdx ) % buffer.size();
    saturated = saturated || nextIdx == 0;
}


template< typename T >
std::size_t circular_buffer< T >::size() const
{
    return saturated ? buffer.size() : nextIdx;
}


template< typename T >
const T& circular_buffer< T >::operator[]( std::size_t idx ) const
{
    LIBCARNA_ASSERT( idx < size() );
    if( saturated )
    {
        return buffer[ ( nextIdx + idx ) % buffer.size() ];
    }
    else
    {
        return buffer[ idx ];
    }
}



// ----------------------------------------------------------------------------------
// FrameRenderer :: Details
// ----------------------------------------------------------------------------------

struct FrameRenderer::Details
{
    Details( GLContext& glContext, unsigned int width, unsigned int height );

    unsigned int width, height;
    bool fitSquare;
    mutable bool reshaped;

    std::unique_ptr< Viewport > viewport;

    GLContext* const glContext;

    const std::unique_ptr< Sampler > fullFrameQuadSampler;
    const std::unique_ptr< MeshBase > fullFrameQuadMesh;
    const ShaderProgram& fullFrameQuadShader;

    float backgroundColor[ 4 ];
    bool backgroundColorChanged;
    
    math::Statistics< double > fpsStatistics;
    circular_buffer< double > fpsData;
    void updateFpsStatistics();
};


FrameRenderer::Details::Details( GLContext& glContext, unsigned int width, unsigned int height )
    : width( width )
    , height( height )
    , reshaped( true )
    , glContext( &glContextMadeCurrent( glContext ) ) // makes 'glContext' current
    , fullFrameQuadSampler( createFullFrameQuadSampler() )
    , fullFrameQuadMesh( createFullFrameQuadMesh() )
    , fullFrameQuadShader( ShaderManager::instance().acquireShader( "full_frame_quad" ) )
    , backgroundColorChanged( true )
    , fpsStatistics( 0, 0 )
    , fpsData( 10 )
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
    pimpl->viewport.reset( new Viewport( width, height, fitSquare ) );
    pimpl->fitSquare = fitSquare;
}


FrameRenderer::~FrameRenderer()
{
    /* Context is activated by 'clearStages'. The context must be activated s.t. the
     * quad mesh and shader can be cleaned up properly.
     */
    clearStages();
    ShaderManager::instance().releaseShader( pimpl->fullFrameQuadShader );
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

    
void FrameRenderer::clearStages()
{
    pimpl->glContext->makeCurrent();
    RenderStageSequence::clearStages();
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
    pimpl->viewport.reset( new Viewport( width, height, fitSquare ) );
    pimpl->reshaped = true;
}


void FrameRenderer::reshape( unsigned int width, unsigned int height )
{
    reshape( width, height, pimpl->fitSquare );
}


void FrameRenderer::render( Camera& cam ) const
{
    Node& root = cam.findRoot();
    LIBCARNA_ASSERT( static_cast< Spatial* >( &root ) != static_cast< Spatial* >( &cam ) );
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
    /* Start time measurement.
     */
    Stopwatch stopwatch;

    /* Check for errors.
     */
    REPORT_GL_ERROR;

    /* Reshape render stages' buffers.
     */
    for( std::size_t rsIdx = 0; rsIdx < stages(); ++rsIdx )
    {
        RenderStage& rs = stageAt( rsIdx );
        
        /* Ensure buffers are properly sized.
         */
        if( pimpl->reshaped || !rs.isInitialized() )
        {
            /* This 'const_cast' is okay, because 'RenderStage' objects are clearly
             * stated to be components of a 'FrameRenderer', thus "it runs in the
             * family" as long as an immutable 'RenderStage' reference not induces a
             * mutable 'FrameRenderer' reference.
             */
            FrameRenderer& fr = const_cast< FrameRenderer& >( *this );
            rs.reshape( fr, pimpl->viewport->width(), pimpl->viewport->height() );
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
    
    /* Stop and evaluate time measurement.
     */
    const double time = stopwatch.result();
    const double fps  = 1 / time;
    pimpl->fpsData.push( fps );
    pimpl->fpsStatistics = math::Statistics< double >( pimpl->fpsData.size(),
        [&]( std::size_t idx )->double
        {
            return pimpl->fpsData[ idx ];
        }
    );
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
    pimpl->fullFrameQuadMesh->render();
}


const math::Statistics< double >& FrameRenderer::framesPerSecond() const
{
    return pimpl->fpsStatistics;
}



}  // namespace LibCarna :: base

}  // namespace LibCarna
