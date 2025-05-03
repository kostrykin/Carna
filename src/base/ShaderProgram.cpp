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
#include <LibCarna/base/ShaderProgram.h>
#include <LibCarna/base/Shader.h>
#include <LibCarna/base/ShaderCompilationError.h>
#include <LibCarna/base/Log.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// ShaderProgram :: Factory :: Details
// ----------------------------------------------------------------------------------

struct ShaderProgram::Factory::Details
{
    const Shader* vertShader;
    const Shader* geomShader;
    const Shader* fragShader;
    Details();
};


ShaderProgram::Factory::Details::Details()
    : vertShader( nullptr )
    , geomShader( nullptr )
    , fragShader( nullptr )
{
}



// ----------------------------------------------------------------------------------
// ShaderProgram :: Factory
// ----------------------------------------------------------------------------------

ShaderProgram::Factory::Factory()
    : pimpl( new Details() )
{
}


ShaderProgram::Factory::~Factory()
{
}


void ShaderProgram::Factory::setVertexShader( const Shader& shader )
{
    LIBCARNA_ASSERT( shader.type == Shader::TYPE_VERTEX_SHADER );
    pimpl->vertShader = &shader;
}


void ShaderProgram::Factory::setGeometryShader( const Shader& shader )
{
    LIBCARNA_ASSERT( shader.type == Shader::TYPE_GEOMETRY_SHADER );
    pimpl->geomShader = &shader;
}


void ShaderProgram::Factory::setFragmentShader( const Shader& shader )
{
    LIBCARNA_ASSERT( shader.type == Shader::TYPE_FRAGMENT_SHADER );
    pimpl->fragShader = &shader;
}


ShaderProgram* ShaderProgram::Factory::create() const
{
    LIBCARNA_ASSERT_EX( pimpl->vertShader != nullptr, "No vertex shader set!" );
    LIBCARNA_ASSERT_EX( pimpl->fragShader != nullptr, "No fragment shader set!" );
    
    ShaderProgram* const shaderProgram = new ShaderProgram();
    try
    {
        /* Compose the shader program.
         */
        const unsigned int id = shaderProgram->id;
        glAttachShader( id, pimpl->vertShader->id );
        glAttachShader( id, pimpl->fragShader->id );
        if( pimpl->geomShader != nullptr )
        {
            glAttachShader( id, pimpl->geomShader->id );
        }
        
        /* Link the shader program.
         */
        glLinkProgram( id );
        shaderProgram->checkErrors();
        return shaderProgram;
    }
    catch( ... )
    {
        delete shaderProgram;
        throw;
    }
}



// ----------------------------------------------------------------------------------
// ShaderProgram
// ----------------------------------------------------------------------------------

ShaderProgram::ShaderProgram()
    : id( glCreateProgram() )
{
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
    
    int linkStatus;
    glGetProgramiv( id, GL_LINK_STATUS, &linkStatus );
    if( linkStatus != GL_TRUE )
    {
        throw ShaderCompilationError( "Linking failed!" );
    }
}


ShaderProgram::~ShaderProgram()
{
    if( id )
    {
        glDeleteProgram( id );
    }
}



}  // namespace Carna :: base

}  // namespace Carna
