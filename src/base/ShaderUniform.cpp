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


int ShaderUniformBase::location( const GLContext& glc ) const
{
    const ShaderProgram& shader = glc.shader();
    const GLint location = glGetUniformLocation( shader.id, name.c_str() );
    return location;
}


void ShaderUniformBase::upload() const
{
    GLContext& glc = GLContext::current();
    const int loc = location( glc );
    if( loc != NULL_UNIFORM_LOCATION )
    {
        uploadTo( loc );
    }
}



// ----------------------------------------------------------------------------------
// uploadUniform
// ----------------------------------------------------------------------------------

template< >
void uploadUniform< int >( int location, const int& value )
{
    glUniform1i( location, value );
}


template< >
void uploadUniform< unsigned int >( int location, const unsigned int& value )
{
    glUniform1ui( location, value );
}


template< >
void uploadUniform< float >( int location, const float& value )
{
    glUniform1f( location, value );
}


template< >
void uploadUniform< math::Vector2f >( int location, const math::Vector2f& value )
{
    glUniform2f( location, value.x(), value.y() );
}


template< >
void uploadUniform< math::Vector3f >( int location, const math::Vector3f& value )
{
    glUniform3f( location, value.x(), value.y(), value.z() );
}


template< >
void uploadUniform< math::Vector4f >( int location, const math::Vector4f& value )
{
    glUniform4f( location, value.x(), value.y(), value.z(), value.w() );
}


template< >
void uploadUniform< math::Matrix4f >( int location, const math::Matrix4f& value )
{
    glUniformMatrix4fv( location, 1, false, value.data() );
}



}  // namespace Carna :: base

}  // namespace Carna
