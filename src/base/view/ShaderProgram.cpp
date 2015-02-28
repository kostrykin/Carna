/*
 *  Copyright (C) 2010 - 2014 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#include <Carna/base/view/glew.h>
#include <Carna/base/view/ShaderProgram.h>
#include <Carna/base/view/Shader.h>
#include <Carna/base/view/ShaderCompilationError.h>
#include <Carna/base/Log.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// ShaderProgram
// ----------------------------------------------------------------------------------

ShaderProgram::ShaderProgram( const Shader& vertexShader, const Shader& fragmentShader )
    : id( glCreateProgram() )
{
    try
    {
        CARNA_ASSERT( vertexShader.type == Shader::TYPE_VERTEX_SHADER );
        CARNA_ASSERT( fragmentShader.type == Shader::TYPE_FRAGMENT_SHADER );
        CARNA_ASSERT_EX( id != 0, "failed to acquire shader program object" );

        glAttachShader( id, vertexShader.id );
        glAttachShader( id, fragmentShader.id );
        glLinkProgram( id );

        checkErrors();
    }
    catch( ... )
    {
        release();
        throw;
    }
}


void ShaderProgram::checkErrors() const
{
    GLint nInfoLogLength;
    glGetProgramiv( id, GL_INFO_LOG_LENGTH, &nInfoLogLength );
    if( nInfoLogLength > 1 )
    {
        const std::unique_ptr< GLchar > buf( new GLchar[ nInfoLogLength ] );
        glGetProgramInfoLog( id, nInfoLogLength, 0, buf.get() );
        const std::string err( buf.get() );
        if( err != "No errors." )
        {
            throw ShaderCompilationError( err );
        }
    }
}


ShaderProgram::~ShaderProgram()
{
    release();
}


void ShaderProgram::release()
{
    if( id )
    {
        glDeleteProgram( id );
    }
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
