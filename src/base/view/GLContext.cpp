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
#include <Carna/base/view/GLContext.h>
#include <Carna/base/view/ShaderProgram.h>
#include <Carna/base/CarnaException.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// GLContext
// ----------------------------------------------------------------------------------

GLContext* GLContext::myCurrent = nullptr;


GLContext::GLContext( bool isDoubleBuffered )
    : isDoubleBuffered( isDoubleBuffered )
    , myShader( nullptr )
{
    CARNA_ASSERT( glewInit() == GLEW_OK );
    if( myCurrent == nullptr )
    {
        myCurrent = this;
    }
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
    myShader = &shader;
    if( !isActive() )
    {
        makeActive();
    }
    glUseProgram( shader.id );
}


const ShaderProgram& GLContext::shader() const
{
    CARNA_ASSERT( myShader != nullptr );
    return *myShader;
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
