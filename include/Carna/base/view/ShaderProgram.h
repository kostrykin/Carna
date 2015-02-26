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

#ifndef SHADERPROGRAM_H_6014714286
#define SHADERPROGRAM_H_6014714286

/** \file   ShaderProgram.h
  * \brief  Defines \ref Carna::base::view::ShaderProgram
  *
  * \author Leonid Kostrykin
  * \date   2009 - 2015
  */

#include <Carna/base/math.h>
#include <Carna/base/noncopyable.h>
#include <stack>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// ShaderProgram
// ----------------------------------------------------------------------------------

/** \brief  Represents a shader-program object.
  *
  * Merged from GAL library (http://evoid.de)
  *
  * \see    Shader
  * \see    VertexShader
  * \see    FragmentShader
  *
  * \author Leonid Kostrykin
  * \date   2009 - 2011
  */
class CARNA_LIB ShaderProgram
{

    NON_COPYABLE

public:

    /** \brief  Acquires a new GL shader-program object.
      *
      * Forbidden during the acquisition of a Painter object.
      *
      * \throw  std::runtime_error  Either the GL shader-program object acquisition
      *                             failed, a Painter object is currently acquired,
      *                             or the linking failed. See description for details.
      */
    ShaderProgram( const Shader& vertexShader, const Shader& fragmentShader );

    /** \brief  Releases the associated GL shader-program object.
      */
    virtual ~ShaderProgram();

    /** \brief  Holds the shader program ID.
      */
    const unsigned int id;

    /** \brief  Uploads uniform to currently bound shader.
      *
      * \throws AssertionFailure Specified uniform is not defined in shader.
      */
    static void putUniform1f( const std::string& param, float x );

    /** \brief  Uploads uniform to currently bound shader.
      *
      * \throws AssertionFailure Specified uniform is not defined in shader.
      */
    static void putUniform2f( const std::string& param, float x, float y );

    /** \brief  Uploads uniform to currently bound shader.
      *
      * \throws AssertionFailure Specified uniform is not defined in shader.
      */
    static void putUniform3f( const std::string& param, float x, float y, float z );

    /** \brief  Uploads uniform to currently bound shader.
      *
      * \throws AssertionFailure Specified uniform is not defined in shader.
      */
    static void putUniform4f( const std::string& param, const math::Vector4f& );

    /** \brief  Uploads uniform to currently bound shader.
      *
      * \throws AssertionFailure Specified uniform is not defined in shader.
      */
    static void putUniform1i( const std::string& param, signed int );

    /** \brief  Uploads uniform to currently bound shader.
      *
      * \throws AssertionFailure Specified uniform is not defined in shader.
      */
    static void putUniform1u( const std::string& param, unsigned int );

    /** \brief  Uploads uniform to currently bound shader.
      *
      * \throws AssertionFailure Specified uniform is not defined in shader.
      */
    static void putUniform4x4f( const std::string& param, const math::Matrix4f& );

private:

    /** \brief  Queries this shader for compilation errors.
      *
      * \throws std::runtime_error  when shader compilation failed.
      */
    void checkErrors() const;

    /** \brief  Releases the associated GL shader-object.
      *
      * \warning
      * Invoking this method puts this object in an invalid state!
      * It is only allowed be used for cleaning up when a constructor needs to
      * throw an exception, and from the destructor.
      */
    void release();

    const static int NULL_UNIFORM_LOCATION = -1;

    /** \brief  Queries \c glGetUniformLocation and eventually throws exceptions.
      *
      * \throws AssertionFailure Specified uniform is not defined in shader.
      */
    int getUniformLocation( const std::string& name ) const;

}; // ShaderProgram



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // SHADERPROGRAM_H_6014714286
