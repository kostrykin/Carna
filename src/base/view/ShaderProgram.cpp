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


int ShaderProgram::getUniformLocation( const std::string& name ) const
{
    const GLint location = glGetUniformLocation( id, name.c_str() );
    if( location == NULL_UNIFORM_LOCATION )
    {
        Log::instance().record( Log::warning, "Uniform \"" + name + "\" is undeclared in shader." );
    }
    return location;
}


void ShaderProgram::putUniform3f( const std::string& param, float x, float y, float z )
{
    const ShaderProgram& shader = GLContext::current().shader();
    int uniformLocation = shader.getUniformLocation( param );
    if( uniformLocation != NULL_UNIFORM_LOCATION )
    {
        glUniform3f( uniformLocation, x, y, z );
    }
}


void ShaderProgram::putUniform4f( const std::string& param, const Vector4f& v )
{
    const ShaderProgram& shader = GLContext::current().shader();
    int uniformLocation = shader.getUniformLocation( param );
    if( uniformLocation != NULL_UNIFORM_LOCATION )
    {
        glUniform4f( shader.getUniformLocation( param ), v.x(), v.y(), v.z(), v.w() );
    }
}


void ShaderProgram::putUniform2f( const std::string& param, float x, float y )
{
    const ShaderProgram& shader = GLContext::current().shader();
    int uniformLocation = shader.getUniformLocation( param );
    if( uniformLocation != NULL_UNIFORM_LOCATION )
    {
        glUniform2f( shader.getUniformLocation( param ), x, y );
    }
}


void ShaderProgram::putUniform1f( const std::string& param, float x )
{
    const ShaderProgram& shader = GLContext::current().shader();
    int uniformLocation = shader.getUniformLocation( param );
    if( uniformLocation != NULL_UNIFORM_LOCATION )
    {
        glUniform1f( shader.getUniformLocation( param ), x );
    }
}


void ShaderProgram::putUniform1i( const std::string& param, signed int x )
{
    const ShaderProgram& shader = GLContext::current().shader();
    int uniformLocation = shader.getUniformLocation( param );
    if( uniformLocation != NULL_UNIFORM_LOCATION )
    {
        glUniform1i( shader.getUniformLocation( param ), x );
    }
}


void ShaderProgram::putUniform1u( const std::string& param, unsigned int x )
{
    const ShaderProgram& shader = GLContext::current().shader();
    int uniformLocation = shader.getUniformLocation( param );
    if( uniformLocation != NULL_UNIFORM_LOCATION )
    {
        glUniform1ui( shader.getUniformLocation( param ), x );
    }
}


void ShaderProgram::putUniform4x4f( const std::string& param, const Matrix4f& m )
{
    const ShaderProgram& shader = GLContext::current().shader();
    int uniformLocation = shader.getUniformLocation( param );
    if( uniformLocation != NULL_UNIFORM_LOCATION )
    {
        glUniformMatrix4fv( shader.getUniformLocation( param ), 1, false, m.data() );
    }
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
