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
#include <Carna/base/GLContext.h>
#include <Carna/base/ShaderProgram.h>
#include <Carna/base/RenderState.h>
#include <Carna/base/CarnaException.h>
#include <set>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// GLContext
// ----------------------------------------------------------------------------------

GLContext* currentGLContext = nullptr;

const unsigned int GLContext::DEPTH_BUFFER_BIT = GL_DEPTH_BUFFER_BIT;
const unsigned int GLContext::COLOR_BUFFER_BIT = GL_COLOR_BUFFER_BIT;

typedef std::set< const GLContext* > GLContextSet;
static GLContextSet glContextInstances = GLContextSet();


GLContext::GLContext( bool isDoubleBuffered )
    : isDoubleBuffered( isDoubleBuffered )
    , myShader( nullptr )
#pragma warning( push )
#pragma warning( disable:4355 )
    /* It is okay to use 'this' in class initialization list, as long as it is not
     * used to access any members that may not have been initialized yet.
     */
    , myRenderState( RenderState::createDefaultRenderState( *this ) )
#pragma warning( pop )
{
    CARNA_GLEW_INIT;
    glContextInstances.insert( this );
    if( currentGLContext == nullptr )
    {
        currentGLContext = this;
    }

    /* Setup depth-write, depth-test and depth compare function.
     */
    myRenderState->setDepthWrite( true );
    myRenderState->setDepthTest( true );
    myRenderState->setDepthTestFunction( GL_LEQUAL );

    /* Setup blending.
     */
    myRenderState->setBlend( false );
    myRenderState->setBlendFunction( BlendFunction( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) );
    myRenderState->setBlendEquation( GL_FUNC_ADD );

    /* Setup back-face culling.
     */
    myRenderState->setCullFace( RenderState::cullBack );
    myRenderState->setFrontFace( true );

    /* Set default render state.
     */
    myRenderState->commit();
    renderStates.push( myRenderState.get() );
}


GLContext::~GLContext()
{
    glContextInstances.erase( this );
    if( currentGLContext == this )
    {
        /* The destroyed context was the current one. Since we want there always to
         * be a 'current' context and since we want 'currentGLContext' to be
         * synchronized, we will pick some arbitrary and make it current.
         */
        if( glContextInstances.empty() )
        {
            currentGLContext = nullptr;
        }
        else
        {
            const GLContext* const nextGLContext = *glContextInstances.begin();
            nextGLContext->makeCurrent();
        }
    }
}


GLContext& GLContext::current()
{
    CARNA_ASSERT( currentGLContext != nullptr );
    return *currentGLContext;
}


bool GLContext::isCurrent() const
{
    return currentGLContext == this;
}


void GLContext::setShader( const ShaderProgram& shader )
{
    if( myShader != nullptr || myShader != &shader )
    {
        CARNA_ASSERT( isCurrent() );
        myShader = &shader;
        glUseProgram( shader.id );
    }
}


const ShaderProgram& GLContext::shader() const
{
    CARNA_ASSERT( myShader != nullptr );
    return *myShader;
}


void GLContext::clearBuffers( unsigned int flags )
{
    CARNA_ASSERT( isCurrent() );
    RenderState rs( *this );
    if( flags & GL_DEPTH_BUFFER_BIT )
    {
        rs.setDepthWrite( true );
    }
    glClear( flags );
}



}  // namespace Carna :: base

}  // namespace Carna
