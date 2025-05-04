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

#ifndef SHADER_H_6014714286
#define SHADER_H_6014714286

/** \file
  * \brief
  * Defines \ref LibCarna::base::Shader.
  */

#include <string>
#include <LibCarna/LibCarna.hpp>
#include <LibCarna/base/noncopyable.hpp>

namespace LibCarna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Shader
// ----------------------------------------------------------------------------------

/** \brief
  * Maintains an OpenGL shader object. Realizes the RAII-idiom.
  *
  * \see
  * The concept of shaders is explained \ref RenderingPipeline "here".
  *
  * \author Leonid Kostrykin
  */
class LIBCARNA Shader
{

    NON_COPYABLE

    /** \brief
      * Deletes the maintained OpenGL shader object.
      *
      * \warning
      * Invoking this method puts this object in an invalid state! It is only allowed
      * be used for cleaning up when a constructor needs to throw an exception, and
      * from the destructor.
      */
    void release();

public:

    const static unsigned int TYPE_VERTEX_SHADER;   ///< Indicates shader object of `GL_VERTEX_SHADER`   type.
    const static unsigned int TYPE_FRAGMENT_SHADER; ///< Indicates shader object of `GL_FRAGMENT_SHADER` type.
    const static unsigned int TYPE_GEOMETRY_SHADER; ///< Indicates shader object of `GL_GEOMETRY_SHADER` type.

    /** \brief
      * Compiles \a src and creates new OpenGL shader object of \a type.
      *
      * \param type
      *     is usually `TYPE_VERTEX_SHADER`, `TYPE_GEOMETRY_SHADER` or
      *     `TYPE_FRAGMENT_SHADER`.
      *
      * \param src
      *     references the shader source code.
      *
      * \throw AssertionFailure thrown when creation of OpenGL shader object fails.
      * \throw ShaderCompilationError thrown when shader compilation fails.
      */
    Shader( unsigned int type, const std::string& src );

    /** \brief
      * Deletes the maintained OpenGL shader object.
      */
    ~Shader();

    /** \brief
      * Identifies the maintained OpenGL shader object.
      */
    const unsigned int id;

    /** \brief
      * Holds the type of the maintained OpenGL shader object. This is
      * `TYPE_VERTEX_SHADER`, `TYPE_GEOMETRY_SHADER` or `TYPE_FRAGMENT_SHADER`
      * usually.
      */
    const unsigned int type;

}; // Shader



}  // namespace LibCarna :: base

}  // namespace LibCarna

#endif // SHADER_H_6014714286
