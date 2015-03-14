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
// GLContext :: OnGLShutdownListener
// ----------------------------------------------------------------------------------

GLContext::OnGLShutdownListener::~OnGLShutdownListener()
{
}



// ----------------------------------------------------------------------------------
// GLContext
// ----------------------------------------------------------------------------------

GLContext* currentGLContext = nullptr;

const unsigned int GLContext::DEPTH_BUFFER_BIT = GL_DEPTH_BUFFER_BIT;
const unsigned int GLContext::COLOR_BUFFER_BIT = GL_COLOR_BUFFER_BIT;

typedef std::set< GLContext::OnGLShutdownListener* > OnGLShutdownListeners;
static OnGLShutdownListeners onGLShutdownListeners = OnGLShutdownListeners();

typedef std::set< GLContext* > GLContextInstances;
static GLContextInstances glContextInstances = GLContextInstances();


GLContext::GLContext( bool isDoubleBuffered )
    : isDoubleBuffered( isDoubleBuffered )
    , myShader( nullptr )
    , myRenderState( new RenderState() )
    , wasContextShutdown( false )
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
    CARNA_ASSERT_EX
        ( wasContextShutdown
        , "You must invoke GLContext::shutdownContext() before destroying the implementation!" );
    
    glContextInstances.erase( this );
    if( currentGLContext == this )
    {
        /* The destroyed context was the current one. Since we want there always to
         * be a 'current' context and since we want 'currentGLContext' to be
         * synchronized, we will pick some arbitary and make it current.
         */
        if( glContextInstances.empty() )
        {
            currentGLContext = nullptr;
        }
        else
        {
            GLContext* const nextGLContext = *glContextInstances.begin();
            nextGLContext->makeCurrent();
        }
    }
}


void GLContext::shutdownContext()
{
    if( glContextInstances.size() == 1 )
    {
        GLContext& lastGLContext = *this;
        lastGLContext.makeCurrent();
        std::for_each( onGLShutdownListeners.begin(), onGLShutdownListeners.end(),
            [ &lastGLContext ]( GLContext::OnGLShutdownListener* listener )
            {
                listener->onGLShutdown( lastGLContext );
            }
        );
    }
    wasContextShutdown = true;
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


void GLContext::addOnGLShutdownListener( GLContext::OnGLShutdownListener& onGLShutdownListener )
{
    onGLShutdownListeners.insert( &onGLShutdownListener );
}


void GLContext::removeOnGLShutdownListener( const GLContext::OnGLShutdownListener& onGLShutdownListener )
{
    onGLShutdownListeners.erase( const_cast< OnGLShutdownListener* >( &onGLShutdownListener ) );
}



}  // namespace Carna :: base

}  // namespace Carna
