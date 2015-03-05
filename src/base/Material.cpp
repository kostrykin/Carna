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

#include <Carna/base/Material.h>
#include <Carna/base/ShaderManager.h>
#include <Carna/base/GLContext.h>
#include <Carna/base/CarnaException.h>
#include <map>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Material :: Details
// ----------------------------------------------------------------------------------

struct Material::Details
{

    std::map< std::string, ShaderUniformBase* > uniforms;

}; // Material :: Details



// ----------------------------------------------------------------------------------
// Material
// ----------------------------------------------------------------------------------

Material::Material( const std::string& shaderName )
    : pimpl( new Details() )
    , myShader( nullptr )
    , shaderName( shaderName )
{
}


Material::~Material()
{
    clearUniforms();
}


Material& Material::create( const std::string& shaderName )
{
    return *new Material( shaderName );
}


void Material::acquireVideoResource()
{
    GeometryFeature::acquireVideoResource();
    if( myShader == nullptr )
    {

        myShader = &ShaderManager::instance().acquireShader( shaderName );
    }
}


void Material::releaseVideoResource()
{
    GeometryFeature::releaseVideoResource();
    if( videoResourceAcquisitionsCount() == 0 )
    {
        ShaderManager::instance().releaseShader( *myShader );
        myShader = nullptr;
    }
}


const ShaderProgram& Material::shader() const
{
    CARNA_ASSERT( myShader != nullptr );
    return *myShader;
}


bool Material::controlsSameVideoResource( const GeometryFeature& other ) const
{
    const Material* const smcOther = dynamic_cast< const Material* >( &other );
    if( smcOther == nullptr )
    {
        return false;
    }
    else
    {
        return smcOther->shaderName == shaderName;
    }
}


void Material::addUniform( ShaderUniformBase* uniform )
{
    CARNA_ASSERT( !uniform->name.empty() );
    const auto uniformItr = pimpl->uniforms.find( uniform->name );
    if( uniformItr != pimpl->uniforms.end() )
    {
        delete uniformItr->second;
    }
    pimpl->uniforms[ uniform->name ] = uniform;
}


void Material::clearUniforms()
{
    for( auto uniformItr = pimpl->uniforms.begin(); uniformItr != pimpl->uniforms.end(); ++uniformItr )
    {
        delete uniformItr->second;
    }
    pimpl->uniforms.clear();
}


void Material::removeUniform( const std::string& name )
{
    const auto uniformItr = pimpl->uniforms.find( name );
    if( uniformItr != pimpl->uniforms.end() )
    {
        delete uniformItr->second;
        pimpl->uniforms.erase( uniformItr );
    }
}


void Material::activate( RenderState& rs, GLContext& glc ) const
{
    /* Ensure the shader is activated.
     */
    glc.setShader( shader() );

    /* Upload uniform variables.
     */
    for( auto uniformItr = pimpl->uniforms.begin(); uniformItr != pimpl->uniforms.end(); ++uniformItr )
    {
        uniformItr->second->upload();
    }
}



}  // namespace Carna :: base

}  // namespace Carna
