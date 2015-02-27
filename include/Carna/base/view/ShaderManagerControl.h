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

#ifndef SHADERMANAGERCONTROL_H_6014714286
#define SHADERMANAGERCONTROL_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/view/ShaderControl.h>
#include <string>

/** \file   ShaderManagerControl.h
  * \brief  Defines \ref Carna::base::view::ShaderManagerControl.
  */

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// ShaderManagerControl
// ----------------------------------------------------------------------------------
    
/** \brief
  * Adapter-like implementation of \ref ShaderControl that delegates to \ref ShaderManager.
  *
  * \author Leonid Kostrykin
  * \date 27.2.2015
  */
class ShaderManagerControl : public ShaderControl
{

    const ShaderProgram* myShader;

public:

    const std::string shaderName;

    ShaderManagerControl( const std::string& shaderName );

    virtual void uploadResources() override;

    virtual void deleteResources() override;

    virtual const ShaderProgram& shader() const override;

}; // ShaderManagerControl



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // SHADERMANAGERCONTROL_H_6014714286
