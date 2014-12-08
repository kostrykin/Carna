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
#include <Carna/base/view/VertexShader.h>
#include <Carna/base/view/FragmentShader.h>
#include <Carna/base/view/ShaderCompilationError.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// ShaderProgram
// ----------------------------------------------------------------------------------

std::stack< unsigned int > ShaderProgram::shaderIdStack;


ShaderProgram::ShaderProgram( VertexShader& vs )
    : id( glCreateProgram() )
{
    try
    {
        CARNA_ASSERT_EX( vs.id != 0, "failed to acquire shader program object" );

        glAttachShader( id, vs.id );
        glLinkProgram( id );

        checkErrors();
    }
    catch( ... )
    {
        release();
        throw;
    }
}


ShaderProgram::ShaderProgram( VertexShader& vs, FragmentShader& fs )
    : id( glCreateProgram() )
{
    try
    {
        CARNA_ASSERT_EX( vs.id != 0, "failed to acquire shader program object" );

        glAttachShader( id, vs.id );
        glAttachShader( id, fs.id );
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



// ----------------------------------------------------------------------------------
// ShaderProgram :: Binding
// ----------------------------------------------------------------------------------

ShaderProgram::Binding::Binding( const ShaderProgram& sp )
{
    shaderIdStack.push( sp.id );
    glUseProgram( shaderIdStack.top() );
}


ShaderProgram::Binding::~Binding()
{
    shaderIdStack.pop();
    glUseProgram( shaderIdStack.empty() ? 0 : shaderIdStack.top() );
}


int ShaderProgram::Binding::getUniformLocation( const std::string& name ) const
{
    const GLint location = glGetUniformLocation( shaderIdStack.top(), name.c_str() );

    CARNA_ASSERT_EX
        ( location != NULL_UNIFORM_LOCATION
        , "uniform '" << name << "' not defined in shader" );

    return location;
}


void ShaderProgram::Binding::putUniform3f( const std::string& param, float x, float y, float z )
{
    glUniform3f( getUniformLocation( param ), x, y, z );
}


void ShaderProgram::Binding::putUniform3f( const std::string& param, const Vector& v )
{
    putUniform3f( param, v[ 0 ], v[ 1 ], v[ 2 ] );
}


void ShaderProgram::Binding::putUniform2f( const std::string& param, float x, float y )
{
    glUniform2f( getUniformLocation( param ), x, y );
}


void ShaderProgram::Binding::putUniform1f( const std::string& param, float x )
{
    glUniform1f( getUniformLocation( param ), x );
}


void ShaderProgram::Binding::putUniform1i( const std::string& param, signed int x )
{
    glUniform1i( getUniformLocation( param ), x );
}


void ShaderProgram::Binding::putUniform1u( const std::string& param, unsigned int x )
{
    glUniform1ui( getUniformLocation( param ), x );
}


void ShaderProgram::Binding::putUniform3x3( const std::string& param, const Transformation& m )
{
    float a[ 9 ];
    for( int i = 0; i < 9; ++i )
    {
        a[ i ] = m.getTransformationMatrix()( i % 3, i / 3 );
    }

    glUniformMatrix3fv( getUniformLocation( param ), 1, false, a );
}


void ShaderProgram::Binding::putUniform4x4( const std::string& param, const Transformation& m )
{
    float a[ 16 ];
    for( int i = 0; i < 16; ++i )
    {
        a[ i ] = m.getTransformationMatrix()( i % 4, i / 4 );
    }

    glUniformMatrix4fv( getUniformLocation( param ), 1, false, a );
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
