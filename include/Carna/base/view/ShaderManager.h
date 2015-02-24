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
  * \brief  Defines \ref Carna::base::view::ShaderManager.
  */

#include <Carna/Carna.h>
#include <Carna/base/Singleton.h>
#include <Carna/base/noncopyable.h>
#include <string>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// ShaderManager
// ----------------------------------------------------------------------------------

class CARNA_LIB ShaderManager : public Singleton< ShaderManager >
{

    NON_COPYABLE

    struct Details;
    const std::unique_ptr< Details > pimpl;

protected:

    friend class Carna::base::Singleton< ShaderManager >;

    ShaderManager();

public:

    virtual ~ShaderManager();

    const ShaderProgram& acquireShader( const std::string& shaderName );

    void releaseShader( const ShaderProgram& shader );

}; // ShaderManager



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // SHADERMANAGER_H_6014714286
