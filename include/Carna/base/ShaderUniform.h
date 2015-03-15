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

#ifndef SHADERUNIFORM_H_6014714286
#define SHADERUNIFORM_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/math.h>

/** \file   Mesh.h
  * \brief  Defines \ref Carna::base::ShaderUniform.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// ShaderUniformType
// ----------------------------------------------------------------------------------

template< typename ParameterType >
struct ShaderUniformType
{
    typedef ParameterType UniformType;
};


template< >
struct ShaderUniformType< Color >
{
    typedef math::Vector4f UniformType;
};



// ----------------------------------------------------------------------------------
// ShaderUniformBase
// ----------------------------------------------------------------------------------

class CARNA_LIB ShaderUniformBase
{

public:

    ShaderUniformBase( const std::string& name );

    virtual ~ShaderUniformBase();

    std::string name;

    void upload() const;

protected:

    virtual void uploadTo( int location ) const = 0;

private:

    const static int NULL_UNIFORM_LOCATION = -1;

    int location( const GLContext& glc ) const;

}; // ShaderUniformBase



// ----------------------------------------------------------------------------------
// uploadUniform
// ----------------------------------------------------------------------------------

void CARNA_LIB uploadUniform( int location, const int value );

void CARNA_LIB uploadUniform( int location, const unsigned int value );

void CARNA_LIB uploadUniform( int location, const float value );

void CARNA_LIB uploadUniform( int location, const math::Vector2f& value );

void CARNA_LIB uploadUniform( int location, const math::Vector3f& value );

void CARNA_LIB uploadUniform( int location, const math::Vector4f& value );

void CARNA_LIB uploadUniform( int location, const math::Matrix4f& value );



// ----------------------------------------------------------------------------------
// ShaderUniform
// ----------------------------------------------------------------------------------

template< typename UniformType >
class ShaderUniform : public ShaderUniformBase
{

public:

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    typedef UniformType Type;

    ShaderUniform( const std::string& name, const Type& type );

    Type value;

protected:

    virtual void uploadTo( int location ) const override;

}; // ShaderUniform


template< typename UniformType >
ShaderUniform< UniformType >::ShaderUniform( const std::string& name, const Type& value )
    : ShaderUniformBase( name )
    , value( value )
{
}


template< typename UniformType >
void ShaderUniform< UniformType >::uploadTo( int location ) const
{
    uploadUniform( location, value );
}



}  // namespace Carna :: base

}  // namespace Carna

#endif // SHADERUNIFORM_H_6014714286
