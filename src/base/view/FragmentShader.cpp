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

#include <Carna/base/view/glew.h>
#include <Carna/base/view/FragmentShader.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// FragmentShader
// ----------------------------------------------------------------------------------

FragmentShader::FragmentShader( const std::string& src )
    : Shader( GL_FRAGMENT_SHADER, src )
{
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
