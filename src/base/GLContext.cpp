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

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// GLContext
// ----------------------------------------------------------------------------------

GLContext* GLContext::myCurrent = nullptr;

const unsigned int GLContext::DEPTH_BUFFER_BIT = GL_DEPTH_BUFFER_BIT;
const unsigned int GLContext::COLOR_BUFFER_BIT = GL_COLOR_BUFFER_BIT;


GLContext::GLContext( bool isDoubleBuffered )
    : isDoubleBuffered( isDoubleBuffered )
    , myShader( nullptr )
    , myRenderState( new RenderState() )
{
    CARNA_GLEW_INIT;
    if( myCurrent == nullptr )
    {
        myCurrent = this;
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
    if( myCurrent == this )
    {
        myCurrent = nullptr;
    }
}


GLContext& GLContext::current()
{
    CARNA_ASSERT( myCurrent != nullptr );
    return *myCurrent;
}


bool GLContext::isActive() const
{
    return myCurrent == this;
}


void GLContext::setShader( const ShaderProgram& shader )
{
    if( myShader != nullptr || myShader != &shader )
    {
        myShader = &shader;
        if( !isActive() )
        {
            makeActive();
        }
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
    if( !isActive() )
    {
        makeActive();
    }

    RenderState rs( *this );
    if( flags & GL_DEPTH_BUFFER_BIT )
    {
        rs.setDepthWrite( true );
    }

    glClear( flags );
}



}  // namespace Carna :: base

}  // namespace Carna
