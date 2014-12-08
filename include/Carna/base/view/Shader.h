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

#ifndef SHADER_H_6014714286
#define SHADER_H_6014714286

/** \file   Shader.h
  * \brief  Defines \ref Carna::base::view::Shader.
  *
  * \author Leonid Kostrykin
  * \date   2009 - 2011
  */

#include <string>
#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// Shader
// ----------------------------------------------------------------------------------

/** \brief  Represents a shader object.
  *
  * Merged from GAL library (http://evoid.de)
  *
  * \see    VertexShader
  * \see    FragmentShader
  * \see    ShaderProgram
  *
  * \author Leonid Kostrykin
  * \date   2009 - 2011
  */
class CARNA_EXPORT Shader
{

    NON_COPYABLE

protected:

    /** \brief  Acquires a new GL shader-object.
      *
      * \param  type    usually GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
      * \param  src     the shader source code
      *
      * \throw  std::runtime_error  Either the GL-shader object acquisition failed
      *                             or there was an error in the source code. See
      *                             description for more details.
      */
    Shader( unsigned int type, const std::string& src );


    /** \brief  Releases the associated GL shader-object.
      *
      * \warning
      * Invoking this method puts this object in an invalid state!
      * It is only allowed be used for cleaning up when a constructor needs to
      * throw an exception, and from the destructor.
      */
    void release();


public:

    /** \brief  Releases the associated GL shader-object.
      */
    virtual ~Shader();


    /** \brief  Identifies the GL shader-object.
      */
    const unsigned int id;

}; // Shader



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // SHADER_H_6014714286
