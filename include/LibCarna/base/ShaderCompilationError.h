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

#ifndef SHADERCOMPILATIONERROR_H_6014714286
#define SHADERCOMPILATIONERROR_H_6014714286

/** \file   ShaderCompilationError.h
  * \brief  Defines \ref Carna::base::ShaderCompilationError.
  */

#include <LibCarna/base/CarnaException.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// ShaderCompilationError
// ----------------------------------------------------------------------------------
    
/** \brief
  * Thrown when shader compilation fails.
  *
  * \author Leonid Kostrykin
  * \date   5.3.13 - 19.3.13
  */
class LIBCARNA ShaderCompilationError : public CarnaException
{

public:

    /** \brief
      * Instantiates.
      *
      * \param details
      *     describes the compilation failure.
      */
    explicit ShaderCompilationError( const std::string& details );
    
    /** \overload
      *
      * \param details
      *     describes the compilation failure.
      *
      * \param vertexShaderFile
      *     is the name of the vertex shader file.
      *
      * \param fragmentShaderFile
      *     is the name of the fragment shader file.
      */
    ShaderCompilationError
        ( const std::string& details
        , const std::string& vertexShaderFile
        , const std::string& fragmentShaderFile );

}; // ShaderCompilationError



}  // namespace Carna :: base

}  // namespace Carna

#endif // SHADERCOMPILATIONERROR_H_6014714286
