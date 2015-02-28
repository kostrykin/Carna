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

#include <Carna/base/view/ShaderResource.h>
#include <Carna/base/view/ShaderManager.h>
#include <Carna/base/CarnaException.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// ShaderResource
// ----------------------------------------------------------------------------------

ShaderResource::ShaderResource( const std::string& shaderName )
    : shaderName( shaderName )
{
}


ShaderResource& ShaderResource::create( const std::string& shaderName )
{
    return *new ShaderResource( shaderName );
}


void ShaderResource::acquireVideoResource()
{
    CARNA_ASSERT( myShader == nullptr );
    GeometryAggregate::acquireVideoResource();
    myShader = &ShaderManager::instance().acquireShader( shaderName );
}


void ShaderResource::releaseVideoResource()
{
    CARNA_ASSERT( myShader != nullptr );
    ShaderManager::instance().releaseShader( *myShader );
    GeometryAggregate::releaseVideoResource();
}


const ShaderProgram& ShaderResource::shader() const
{
    CARNA_ASSERT( myShader != nullptr );
    return *myShader;
}


bool ShaderResource::controlsSameVideoResource( const GeometryAggregate& other ) const
{
    const ShaderResource* const smcOther = dynamic_cast< const ShaderResource* >( &other );
    if( smcOther == nullptr )
    {
        return false;
    }
    else
    {
        return smcOther->shaderName == shaderName;
    }
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
