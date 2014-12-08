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
#include <Carna/base/view/Shader.h>
#include <Carna/base/view/ShaderCompilationError.h>
#include <cstdlib>
#include <memory>
#include <sstream>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// Shader
// ----------------------------------------------------------------------------------

Shader::Shader( GLuint type, const std::string& src )
    : id( glCreateShader( type ) )
{
    try
    {
        CARNA_ASSERT_EX( id != 0, "failed to acquire shader object" );

        const char* pcSrc = src.c_str();
        const GLint nSrcLength = src.length();

        glShaderSource( id, 1, &pcSrc, &nSrcLength );
        glCompileShader( id );

        // check errors

        GLint nInfoLogLength;
        glGetShaderiv( id, GL_INFO_LOG_LENGTH, &nInfoLogLength );
        if( nInfoLogLength > 1 )
        {
            const std::unique_ptr< GLchar > buf( new GLchar[ nInfoLogLength ] );
            glGetShaderInfoLog( id, nInfoLogLength, 0, buf.get() );
            const std::string err( buf.get() );
            if( err != "No errors." )
            {
                throw ShaderCompilationError( err );
            }
        }
    }
    catch( ... )
    {
        release();
        throw;
    }
}


Shader::~Shader()
{
    release();
}


void Shader::release()
{
    if( id )
    {
        glDeleteShader( id );
    }
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
