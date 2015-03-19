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

#ifndef SHADERMANAGER_H_6014714286
#define SHADERMANAGER_H_6014714286

/** \file   ShaderManager.h
  * \brief  Defines \ref Carna::base::ShaderManager.
  */

#include <Carna/Carna.h>
#include <Carna/base/Singleton.h>
#include <Carna/base/noncopyable.h>
#include <string>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// ShaderManager
// ----------------------------------------------------------------------------------

/** \brief
  * Ensures that each \ref ShaderProgram "shader" is built just once and not each
  * time someone requests it.
  *
  * \author Leonid Kostrykin
  * \date   22.2.15 - 18.3.15
  */
class CARNA_LIB ShaderManager : public Singleton< ShaderManager >
{

    NON_COPYABLE

    struct Details;
    const std::unique_ptr< Details > pimpl;

protected:

    friend class Singleton< ShaderManager >;

    /** \brief
      * Instantiates.
      */
    ShaderManager();

public:

    /** \brief
      * Deletes.
      *
      * \attention
      * Shaders that have been acquired previously, but were not released yet, are
      * leaked. An error will be \ref Log "logged".
      */
    virtual ~ShaderManager();

    /** \brief
      * References shader named \a shaderName.
      *
      * Compiles the shader first, if it wasn't built yet. For compilation, the
      * method looks for \ref setSource "shader sources" named \a shaderName,
      * suffixed with `.vert` and `.frag` respectively. If no such sources were
      * specified, the method assumes that shader sources with appropriate names were
      * built in into the library.
      */
    const ShaderProgram& acquireShader( const std::string& shaderName );

    /** \brief
      * Releases \ref acquireShader "previously acquired" \a shader.
      *
      * \pre \a shader has been acquired previously.
      */
    void releaseShader( const ShaderProgram& shader );

    /** \brief
      * Sets shader sources named \a srcName to \a src.
      *
      * Overrides any different shader sources with same name that might have been
      * set previously.
      */
    void setSource( const std::string& srcName, const std::string& src );

    /** \brief
      * Removes any previously set shader sources named \a srcName.
      */
    void removeSource( const std::string& srcName );

}; // ShaderManager



}  // namespace Carna :: base

}  // namespace Carna

#endif // SHADERMANAGER_H_6014714286
