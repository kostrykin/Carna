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
  * \see    ShaderProgram
  *
  * \author Leonid Kostrykin
  * \date   2009 - 2015
  */
class CARNA_LIB Shader
{

    NON_COPYABLE

    /** \brief  Releases the associated GL shader-object.
      *
      * \warning
      * Invoking this method puts this object in an invalid state!
      * It is only allowed be used for cleaning up when a constructor needs to
      * throw an exception, and from the destructor.
      */
    void release();

public:

    const static unsigned int TYPE_VERTEX_SHADER;
    const static unsigned int TYPE_FRAGMENT_SHADER;

    /** \brief  Acquires a new GL shader-object.
      *
      * \param  type    usually \ref TYPE_VERTEX_SHADER or \ref TYPE_FRAGMENT_SHADER
      * \param  src     the shader source code
      *
      * \throw  std::runtime_error  Either the GL-shader object acquisition failed
      *                             or there was an error in the source code. See
      *                             description for more details.
      */
    Shader( unsigned int type, const std::string& src );

    /** \brief  Releases the associated GL shader-object.
      */
    ~Shader();

    /** \brief  Identifies the GL shader-object.
      */
    const unsigned int id;

    const unsigned int type;

}; // Shader



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // SHADER_H_6014714286
