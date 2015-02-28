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

#ifndef SHADERRESOURCE_H_6014714286
#define SHADERRESOURCE_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/view/GeometryAggregate.h>
#include <Carna/base/view/ShaderManager.h>
#include <string>

/** \file   ShaderResource.h
  * \brief  Defines \ref Carna::base::view::ShaderResource.
  */

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// ShaderResource
// ----------------------------------------------------------------------------------
    
/** \brief
  * Adapter-like implementation of \ref ShaderControl that delegates to \ref ShaderManager.
  *
  * \author Leonid Kostrykin
  * \date 27.2.2015
  */
class ShaderResource : public GeometryAggregate
{

    const ShaderProgram* myShader;

    ShaderResource( const std::string& shaderName );

public:

    const std::string shaderName;

    /** \brief
      * Instantiates. Call \ref release when you do not need the object any longer.
      */
    static ShaderResource& create( const std::string& shaderName );

    virtual void acquireVideoResource() override;

    virtual void releaseVideoResource() override;

    const ShaderProgram& shader() const;

    virtual bool controlsSameVideoResource( const GeometryAggregate& ) const override;

}; // ShaderResource



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // SHADERRESOURCE_H_6014714286
