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

#include <Carna/base/view/glew.h>
#include <Carna/base/view/glError.h>
#include <Carna/base/view/FrameRenderer.h>
#include <Carna/base/view/RenderTask.h>
#include <Carna/base/view/RenderStage.h>
#include <Carna/base/view/Node.h>
#include <Carna/base/view/Viewport.h>
#include <Carna/base/view/ShaderManager.h>
#include <Carna/base/view/ShaderUniform.h>
#include <Carna/base/view/Vertex.h>
#include <Carna/base/view/Mesh.h>
#include <Carna/base/view/VertexBuffer.h>
#include <Carna/base/view/IndexBuffer.h>
#include <Carna/base/view/Sampler.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// acquireFullFrameQuadShader
// ----------------------------------------------------------------------------------

static const ShaderProgram& acquireFullFrameQuadShader( GLContext& glContext )
{
    glContext.makeActive();
    return ShaderManager::instance().acquireShader( "full_frame_quad" );
}



// ----------------------------------------------------------------------------------
// createFullFrameQuadSampler
// ----------------------------------------------------------------------------------

static Sampler* createFullFrameQuadSampler( GLContext& glContext )
{
    glContext.makeActive();
    Sampler* const sampler = new Sampler();

    sampler->setMinFilter( Sampler::FILTER_LINEAR );
    sampler->setMagFilter( Sampler::FILTER_LINEAR );

    sampler->setWrapModeR( Sampler::WRAP_MODE_CLAMP );
    sampler->setWrapModeS( Sampler::WRAP_MODE_CLAMP );
    sampler->setWrapModeT( Sampler::WRAP_MODE_CLAMP );

    return sampler;
}



// ----------------------------------------------------------------------------------
// createFullFrameQuadMesh
// ----------------------------------------------------------------------------------

static MeshBase* createFullFrameQuadMesh( GLContext& glContext )
{
    glContext.makeActive();
    typedef Mesh< VertexBase, uint8_t > FullFrameQuadMesh;
    FullFrameQuadMesh* const mesh = new FullFrameQuadMesh( IndexBufferBase::PRIMITIVE_TYPE_TRIANGLE_FAN );

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

    mesh->vertexBuffer().copy( vertices, 4 );
    mesh->indexBuffer().copy( indices, 4 );

    return mesh;
}



// ----------------------------------------------------------------------------------
// FrameRenderer
// ----------------------------------------------------------------------------------

FrameRenderer::FrameRenderer( GLContext& glContext, unsigned int width, unsigned int height, bool fitSquare )
    : myWidth( width )
    , myHeight( height )
    , reshaped( true )
    , myGlContext( &glContext )
    , fullFrameQuadSampler( createFullFrameQuadSampler( glContext ) )
    , fullFrameQuadMesh( createFullFrameQuadMesh( glContext ) )
    , fullFrameQuadShader( acquireFullFrameQuadShader( glContext ) )
{
    myViewport.reset( new Viewport( *this, fitSquare ) );
}


FrameRenderer::~FrameRenderer()
{
    /* Context is activated by 'clearStages'. The context must be activated s.t. the
     * quad mesh and shader can be cleaned up properly.
     */
    clearStages();
    ShaderManager::instance().releaseShader( fullFrameQuadShader );
}


GLContext& FrameRenderer::glContext() const
{
    return *myGlContext;
}


std::size_t FrameRenderer::stages() const
{
    return myStages.size();
}


void FrameRenderer::appendStage( RenderStage* rs )
{
    myStages.push_back( rs );
}

    
void FrameRenderer::clearStages()
{
    myGlContext->makeActive();
    std::for_each( myStages.begin(), myStages.end(), std::default_delete< RenderStage >() );
    myStages.clear();
}


RenderStage& FrameRenderer::stageAt( std::size_t position ) const
{
    return *myStages[ position ];
}


unsigned int FrameRenderer::width() const
{
    return myWidth;
}


unsigned int FrameRenderer::height() const
{
    return myHeight;
}


void FrameRenderer::reshape( unsigned int width, unsigned int height, bool fitSquare )
{
    myWidth = width;
    myHeight = height;
    myViewport.reset( new Viewport( *this, fitSquare ) );
    reshaped = true;
}


void FrameRenderer::render( Camera& cam, Node& root ) const
{
    render( cam, root, *myViewport );
}


void FrameRenderer::render( Camera& cam, Node& root, const Viewport& vp ) const
{
    // update world transforms
    root.updateWorldTransform();

    // reshape render stages' buffers
    for( auto rsItr = myStages.begin(); rsItr != myStages.end(); ++rsItr )
    {
        RenderStage& rs = **rsItr;
        
        // ensure buffers are properly sized
        if( reshaped || !rs.isInitialized() )
        {
            rs.reshape( *this, *myViewport );
        }

        // notify stages of beginning frame
        rs.prepareFrame( root );
    }
    
    // mark that all buffer sizes have been established
    reshaped = false;
    
    // render frame
    vp.makeActive();
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    RenderTask task( *this, cam.projection(), cam.viewTransform() );
    task.render( vp );

    REPORT_GL_ERROR;
}


void FrameRenderer::renderTexture( unsigned int unit, bool useDefaultSampler, bool useDefaultShader, const std::string uniformName ) const
{
    if( useDefaultSampler )
    {
        fullFrameQuadSampler->bind( unit );
    }
    if( useDefaultShader )
    {
        myGlContext->setShader( fullFrameQuadShader );
    }

    ShaderUniform< int >( uniformName, unit ).upload();
    fullFrameQuadMesh->render();
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
