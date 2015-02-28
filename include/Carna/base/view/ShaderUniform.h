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
  * \brief  Defines \ref Carna::base::view::ShaderUniform.
  */

namespace Carna
{

namespace base
{

namespace view
{



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
// ShaderUniform
// ----------------------------------------------------------------------------------

template< typename Type >
class ShaderUniform : public ShaderUniformBase
{

public:

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    typedef typename Type Type;

    ShaderUniform( const std::string& name, const Type& type );

    Type value;

protected:

    virtual void uploadTo( int location ) const override;

}; // ShaderUniform


template< typename Type >
ShaderUniform< Type >::ShaderUniform( const std::string& name, const Type& value )
    : ShaderUniformBase( name )
    , value( value )
{
}


template< typename Type >
void ShaderUniform< Type >::uploadTo( int location ) const
{
    uploadUniform< Type >( location, value );
}



// ----------------------------------------------------------------------------------
// uploadUniform
// ----------------------------------------------------------------------------------

template< typename Type >
void uploadUniform( int location, const Type& value )
{
    static_assert( false, "Unimplemented ShaderUniform<T> type parameter T." );
}


template< >
void CARNA_LIB uploadUniform< int >( int location, const int& value );


template< >
void CARNA_LIB uploadUniform< unsigned int >( int location, const unsigned int& value );


template< >
void CARNA_LIB uploadUniform< float >( int location, const float& value );


template< >
void CARNA_LIB uploadUniform< math::Vector2f >( int location, const math::Vector2f& value );


template< >
void CARNA_LIB uploadUniform< math::Vector3f >( int location, const math::Vector3f& value );


template< >
void CARNA_LIB uploadUniform< math::Vector4f >( int location, const math::Vector4f& value );


template< >
void CARNA_LIB uploadUniform< math::Matrix4f >( int location, const math::Matrix4f& value );



}  // namespace Carna :: base view

}  // namespace Carna :: base

}  // namespace Carna

#endif // SHADERUNIFORM_H_6014714286
