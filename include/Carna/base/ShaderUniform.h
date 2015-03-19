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
  * \brief  Defines \ref Carna::base::ShaderUniformType,
  *                 \ref Carna::base::ShaderUniformBase and
  *                 \ref Carna::base::ShaderUniform.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// ShaderUniformType
// ----------------------------------------------------------------------------------

/** \brief
  * Maps \ref ShaderUniform::Value "ShaderUniform value types" to actually uploaded
  * data types. This general case maps `T` to `T`.
  */
template< typename ValueType >
struct ShaderUniformType
{
    typedef ValueType UniformType;
};


/** \brief
  * Maps the \ref ShaderUniform value type \ref Color to uploaded type \ref math::Vector4f.
  */
template< >
struct ShaderUniformType< Color >
{
    typedef math::Vector4f UniformType;
};



// ----------------------------------------------------------------------------------
// ShaderUniformBase
// ----------------------------------------------------------------------------------

/** \brief
  * Type-independent abstract \ref ShaderUniform base class.
  *
  * \author Leonid Kostrykin
  * \date   1.9.14 - 14.3.15
  */
class CARNA_LIB ShaderUniformBase
{

public:

    /** \brief
      * Instantiates.
      */
    ShaderUniformBase( const std::string& name );

    /** \brief
      * Does nothing.
      */
    virtual ~ShaderUniformBase();

    /** \brief
      * Holds the name of this uniform.
      */
    std::string name;

    /** \brief
      * Uploads this uniform to the current shader.
      */
    void upload() const;

protected:

    /** \brief
      * Uploads this uniform to \a location.
      */
    virtual void uploadTo( int location ) const = 0;

private:

    const static int NULL_UNIFORM_LOCATION = -1;

    int location( const ShaderProgram& ) const;

}; // ShaderUniformBase



// ----------------------------------------------------------------------------------
// uploadUniform
// ----------------------------------------------------------------------------------

/** \cond 0
  * Excluded from Doxygen -->
  */

void CARNA_LIB uploadUniform( int location, const int value );

void CARNA_LIB uploadUniform( int location, const unsigned int value );

void CARNA_LIB uploadUniform( int location, const float value );

void CARNA_LIB uploadUniform( int location, const math::Vector2f& value );

void CARNA_LIB uploadUniform( int location, const math::Vector3f& value );

void CARNA_LIB uploadUniform( int location, const math::Vector4f& value );

void CARNA_LIB uploadUniform( int location, const math::Matrix4f& value );

/** <-- Excluded from Doxygen
  * \endcond
  */



// ----------------------------------------------------------------------------------
// ShaderUniform
// ----------------------------------------------------------------------------------

/** \brief
  * Implements \ref ShaderUniformBase class for particular \a ValueType.
  *
  * \param ValueType
  *     specifies the accepted value type.
  *
  * The particular `glUniform` function used for upload is chosen by the mapping
  * \ref ShaderUniformType and its specializations induce.
  *
  * \author Leonid Kostrykin
  * \date   22.2.15 - 14.3.15
  */
template< typename ValueType >
class ShaderUniform : public ShaderUniformBase
{

public:

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    /** \brief
      * Holds the accepted value type.
      */
    typedef ValueType Value;

    /** Instantiates.
      */
    ShaderUniform( const std::string& name, const ValueType& value );

    /** Holds the value of this uniform.
      */
    ValueType value;

protected:

    virtual void uploadTo( int location ) const override;

}; // ShaderUniform


template< typename ValueType >
ShaderUniform< ValueType >::ShaderUniform( const std::string& name, const ValueType& value )
    : ShaderUniformBase( name )
    , value( value )
{
}


template< typename ValueType >
void ShaderUniform< ValueType >::uploadTo( int location ) const
{
    typedef typename ShaderUniformType< ValueType >::UniformType UniformType;
    uploadUniform( location, static_cast< const UniformType& >( value ) );
}



}  // namespace Carna :: base

}  // namespace Carna

#endif // SHADERUNIFORM_H_6014714286
