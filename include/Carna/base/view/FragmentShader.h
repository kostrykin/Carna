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

#ifndef FRAGMENTSHADER_H_6014714286
#define FRAGMENTSHADER_H_6014714286

/** \file   FragmentShader.h
  * \brief  Defines \ref Carna::base::view::FragmentShader.
  *
  * \author Leonid Kostrykin
  * \date   2009 - 2011
  */

#include "Carna/base/view/Shader.h"

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// FragmentShader
// ----------------------------------------------------------------------------------

/** \brief  \c GL_FRAGMENT_SHADER implementation of Shader
  *
  * Merged from GAL library (http://evoid.de)
  *
  * \see    Shader
  * \see    VertexShader
  * \see    ShaderProgram
  *
  * \author Leonid Kostrykin
  * \date   2009 - 2011
  */
class CARNA_EXPORT FragmentShader : public Shader
{

public:

    /** \brief  Acquires a new GL shader-object.
      *
      * \throw  std::runtime_error  Either the GL shader-object acquisition failed
      *                             or there was an error in the source code. See
      *                             description for details.
      */
    explicit FragmentShader( const std::string& src );

}; // FragmentShader



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // FRAGMENTSHADER_H_6014714286
