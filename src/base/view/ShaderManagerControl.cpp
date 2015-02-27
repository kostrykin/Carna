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

#include <Carna/base/view/ShaderManagerControl.h>
#include <Carna/base/view/ShaderManager.h>
#include <Carna/base/CarnaException.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// ShaderManagerControl
// ----------------------------------------------------------------------------------

ShaderManagerControl::ShaderManagerControl( const std::string& shaderName )
    : shaderName( shaderName )
{
}


void ShaderManagerControl::uploadResources()
{
    CARNA_ASSERT( myShader == nullptr );
    myShader = &ShaderManager::instance().acquireShader( shaderName );
}


void ShaderManagerControl::deleteResources()
{
    CARNA_ASSERT( myShader != nullptr );
    ShaderManager::instance().releaseShader( *myShader );
}


const ShaderProgram& ShaderManagerControl::shader() const
{
    CARNA_ASSERT( myShader != nullptr );
    return *myShader;
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
