/*
 *  Copyright (C) 2010 - 2016 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 * 
 * 
 *  Copyright (C) 2021 - 2025 Leonid Kostrykin
 * 
 */

#ifndef SHADERUNIFORM_H_6014714286
#define SHADERUNIFORM_H_6014714286

#include <LibCarna/LibCarna.h>
#include <LibCarna/base/math.h>

/** \file   Mesh.h
  * \brief  Defines \ref LibCarna::base::ShaderUniformType,
  *                 \ref LibCarna::base::ShaderUniformBase and
  *                 \ref LibCarna::base::ShaderUniform.
  */

namespace LibCarna
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
    /** \brief
      * Maps \ref ShaderUniform::Value "ShaderUniform value types" to itself.
      */
    typedef ValueType UniformType;
};


/** \brief
  * Maps the \ref ShaderUniform value type \ref Color to uploaded type
  * \ref math::Vector4f.
  */
template< >
struct ShaderUniformType< Color >
{
    /** \brief
      * Maps the \ref ShaderUniform value type \ref Color to uploaded type
      * \ref math::Vector4f.
      */
    typedef math::Vector4f UniformType;
};



// ----------------------------------------------------------------------------------
// ShaderUniformBase
// ----------------------------------------------------------------------------------

/** \brief
  * Type-independent abstract \ref ShaderUniform base class.
  *
  * \author Leonid Kostrykin
  */
class LIBCARNA ShaderUniformBase
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
      * Uploads this uniform to the current shader. Returns `true` if the current
      * shader has a matching uniform defined or `false` otherwise.
      */
    bool upload() const;

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

/** \cond false
  * Excluded from Doxygen -->
  */

void LIBCARNA uploadUniform( int location, const int value );

void LIBCARNA uploadUniform( int location, const unsigned int value );

void LIBCARNA uploadUniform( int location, const float value );

void LIBCARNA uploadUniform( int location, const math::Vector2f& value );

void LIBCARNA uploadUniform( int location, const math::Vector3f& value );

void LIBCARNA uploadUniform( int location, const math::Vector4f& value );

void LIBCARNA uploadUniform( int location, const math::Matrix3f& value );

void LIBCARNA uploadUniform( int location, const math::Matrix4f& value );

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



}  // namespace LibCarna :: base

}  // namespace LibCarna

#endif // SHADERUNIFORM_H_6014714286
