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

#include <LibCarna/base/glew.h>
#include <LibCarna/base/glError.h>
#include <LibCarna/base/GLContext.h>
#include <LibCarna/base/ShaderProgram.h>
#include <LibCarna/base/RenderState.h>
#include <LibCarna/base/CarnaException.h>
#include <set>
#include <stack>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// GLContext :: Details
// ----------------------------------------------------------------------------------

struct GLContext::Details
{
    Details();
    const ShaderProgram* shader;
    std::unique_ptr< RenderState > defaultRenderState;
    std::stack< const RenderState* > renderStates;
};


GLContext::Details::Details()
    : shader( nullptr )
{
}



// ----------------------------------------------------------------------------------
// GLContext
// ----------------------------------------------------------------------------------

GLContext* currentGLContext = nullptr;

const unsigned int GLContext::DEPTH_BUFFER_BIT = GL_DEPTH_BUFFER_BIT;
const unsigned int GLContext::COLOR_BUFFER_BIT = GL_COLOR_BUFFER_BIT;

typedef std::set< const GLContext* > GLContextSet;
static GLContextSet glContextInstances = GLContextSet();


GLContext::GLContext( bool isDoubleBuffered )
    : pimpl( new Details() )
    , isDoubleBuffered( isDoubleBuffered )
{
    pimpl->defaultRenderState.reset( new RenderState( *this ) );
    RenderState& defaultRenderState = *pimpl->defaultRenderState;

    LIBCARNA_GLEW_INIT;
    glContextInstances.insert( this );
    currentGLContext = this;

    /* Setup depth-write, depth-test and depth compare function.
     */
    defaultRenderState.setDepthWrite( true );
    defaultRenderState.setDepthTest( true );
    defaultRenderState.setDepthTestFunction( GL_LEQUAL );

    /* Setup blending.
     */
    defaultRenderState.setBlend( false );
    defaultRenderState.setBlendFunction( BlendFunction( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) );
    defaultRenderState.setBlendEquation( GL_FUNC_ADD );

    /* Setup back-face culling.
     */
    defaultRenderState.setCullFace( RenderState::cullBack );
    defaultRenderState.setFrontFace( true );

    /* Enable support for 'gl_PointSize' in shader.
     */
    defaultRenderState.setPointSize( -1 );

    /* Set default render state.
     */
    defaultRenderState.commit();
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
    
    
void GLContext::pushRenderState( const RenderState& rs )
{
    pimpl->renderStates.push( &rs );
}


void GLContext::popRenderState()
{
    LIBCARNA_ASSERT( !pimpl->renderStates.empty() );
    pimpl->renderStates.pop();
}


const RenderState& GLContext::currentRenderState() const
{
    LIBCARNA_ASSERT( !pimpl->renderStates.empty() );
    return *pimpl->renderStates.top();
}


GLContext& GLContext::current()
{
    LIBCARNA_ASSERT( currentGLContext != nullptr );
    return *currentGLContext;
}


bool GLContext::isCurrent() const
{
    return currentGLContext == this;
}


void GLContext::makeCurrent() const
{
    currentGLContext = const_cast< GLContext* >( this );
    this->activate();
}


void GLContext::setShader( const ShaderProgram& shader )
{
    if( pimpl->shader != &shader )
    {
        LIBCARNA_ASSERT( isCurrent() );
        pimpl->shader = &shader;
        glUseProgram( shader.id );
        REPORT_GL_ERROR;
    }
}


const ShaderProgram& GLContext::shader() const
{
    LIBCARNA_ASSERT( pimpl->shader != nullptr );
    return *pimpl->shader;
}


void GLContext::clearBuffers( unsigned int flags )
{
    LIBCARNA_ASSERT( isCurrent() );
    RenderState rs;
    if( flags & GL_DEPTH_BUFFER_BIT )
    {
        rs.setDepthWrite( true );
    }
    glClear( flags );
}



}  // namespace Carna :: base

}  // namespace Carna
