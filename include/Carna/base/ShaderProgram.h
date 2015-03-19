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
  * \brief  Defines \ref Carna::base::ShaderProgram
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



// ----------------------------------------------------------------------------------
// ShaderProgram
// ----------------------------------------------------------------------------------

/** \brief
  * Maintains an OpenGL shader program. Realizes the RAII-idiom.
  *
  * \see
  * The concept of shaders is explained \ref RenderingPipeline "here".
  *
  * \author Leonid Kostrykin
  * \date   2009 - 2015
  */
class CARNA_LIB ShaderProgram
{

    NON_COPYABLE

public:

    /** \brief
      * Creates new OpenGL shader program.
      *
      * \throw AssertionFailure
      *     thrown when creation of OpenGL program or the linking of the shader
      *     objects fails.
      */
    ShaderProgram( const Shader& vertexShader, const Shader& fragmentShader );

    /** \brief
      * Deletes the maintained OpenGL shader program.
      */
    virtual ~ShaderProgram();

    /** \brief
      * Holds the shader program ID.
      */
    const unsigned int id;

private:

    /** \brief
      * Queries this shader program for errors.
      */
    void checkErrors() const;

    /** \brief
      * Deletes the maintained OpenGL shader program.
      *
      * \warning
      * Invoking this method puts this object in an invalid state! It is only allowed
      * be used for cleaning up when a constructor needs to throw an exception, and
      * from the destructor.
      */
    void release();

}; // ShaderProgram



}  // namespace Carna :: base

}  // namespace Carna

#endif // SHADERPROGRAM_H_6014714286
