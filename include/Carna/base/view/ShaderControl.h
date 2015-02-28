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

#ifndef SHADERCONTROL_H_6014714286
#define SHADERCONTROL_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/view/VideoResourceControl.h>
#include <memory>

/** \file   ShaderControl.h
  * \brief  Defines \ref Carna::base::view::ShaderControl.
  */

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// ShaderControl
// ----------------------------------------------------------------------------------
    
/** \brief
  * Abstract base \ref VideoResourceControl for \ref ShaderProgram instances.
  *
  * \see
  * \ref ShaderManagerControl is an adapter-like implementation of this class
  * that utilizes \ref ShaderManager.
  *
  * \author Leonid Kostrykin
  * \date 27.2.2015
  */
class ShaderControl : public VideoResourceControl
{

public:

    virtual const ShaderProgram& shader() const = 0;

}; // ShaderControl



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // SHADERCONTROL_H_6014714286
