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

#ifndef VERTEXSHADER_H_6014714286
#define VERTEXSHADER_H_6014714286

/** \file   VertexShader.h
  * \brief  Defines \ref Carna::base::view::VertexShader.
  *
  * Merged from GAL library (http://evoid.de)
  *
  * \author Leonid Kostrykin
  * \date   2009 - 2011
  */

#include <Carna/base/view/Shader.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// VertexShader
// ----------------------------------------------------------------------------------

/** \brief  \c GL_VERTEX_SHADER implementation of Shader
  *
  * Merged from GAL library (http://evoid.de)
  *
  * \see    Shader
  * \see    FragmentShader
  * \see    ShaderProgram
  *
  * \author Leonid Kostrykin
  * \date   2009 - 2011
  */
class CARNA_EXPORT VertexShader : public Shader
{

public:

    /** \brief  Acquires a new GL shader-object.
      *
      * \throw  std::runtime_error  Either the GL shader-object acquisition failed
      *                             or there was an error in the source code. See
      *                             description for more details.
      */
    explicit VertexShader( const std::string& src );

}; // VertexShader



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // VERTEXSHADER_H_6014714286
