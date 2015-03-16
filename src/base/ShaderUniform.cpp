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
#include <Carna/base/ShaderUniform.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// ShaderUniformBase
// ----------------------------------------------------------------------------------

ShaderUniformBase::ShaderUniformBase( const std::string& name )
    : name( name )
{
}


ShaderUniformBase::~ShaderUniformBase()
{
}


int ShaderUniformBase::location( const ShaderProgram& shader ) const
{
    const GLint location = glGetUniformLocation( shader.id, name.c_str() );
    return location;
}


void ShaderUniformBase::upload() const
{
    GLContext& glc = GLContext::current();
    const int loc = location( glc.shader() );
    if( loc != NULL_UNIFORM_LOCATION )
    {
        uploadTo( loc );
    }
}



// ----------------------------------------------------------------------------------
// uploadUniform
// ----------------------------------------------------------------------------------

void uploadUniform( int location, const int value )
{
    glUniform1i( location, value );
}


void uploadUniform( int location, const unsigned int value )
{
    glUniform1ui( location, value );
}


void uploadUniform( int location, const float value )
{
    glUniform1f( location, value );
}


void uploadUniform( int location, const math::Vector2f& value )
{
    glUniform2f( location, value.x(), value.y() );
}


void uploadUniform( int location, const math::Vector3f& value )
{
    glUniform3f( location, value.x(), value.y(), value.z() );
}


void uploadUniform( int location, const math::Vector4f& value )
{
    glUniform4f( location, value.x(), value.y(), value.z(), value.w() );
}


void uploadUniform( int location, const math::Matrix4f& value )
{
    glUniformMatrix4fv( location, 1, false, value.data() );
}



}  // namespace Carna :: base

}  // namespace Carna
