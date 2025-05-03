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

#include <LibCarna/base/Material.h>
#include <LibCarna/base/ShaderManager.h>
#include <LibCarna/base/GLContext.h>
#include <LibCarna/base/CarnaException.h>
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
// Material :: ManagedInterface
// ----------------------------------------------------------------------------------

Material::ManagedInterface::ManagedInterface( Material& material )
    : GeometryFeature::ManagedInterface( material )
    , material( material )
{
    if( material.videoResourceAcquisitionsCount() == 1 )
    {
        /* Acquire the shader.
         */
        material.shader = &ShaderManager::instance().acquireShader( material.shaderName );
    }
}


Material::ManagedInterface::~ManagedInterface()
{
    if( material.videoResourceAcquisitionsCount() == 1 )
    {
        /* Release the shader.
         */
        ShaderManager::instance().releaseShader( *material.shader );
        material.shader = nullptr;
    }
}


const ShaderProgram& Material::ManagedInterface::shader() const
{
    LIBCARNA_ASSERT( material.shader != nullptr );
    return *material.shader;
}


void Material::ManagedInterface::activate( RenderState& rs ) const
{
    /* Ensure the shader is activated.
     */
    GLContext& glc = GLContext::current();
    glc.setShader( shader() );

    /* Upload uniform variables.
     */
    for( auto uniformItr = material.pimpl->uniforms.begin(); uniformItr != material.pimpl->uniforms.end(); ++uniformItr )
    {
        uniformItr->second->upload();
    }
}



// ----------------------------------------------------------------------------------
// Material
// ----------------------------------------------------------------------------------

Material::Material( const std::string& shaderName )
    : pimpl( new Details() )
    , shader( nullptr )
    , shaderName( shaderName )
{
}


Material::~Material()
{
    clearParameters();
}


Material& Material::create( const std::string& shaderName )
{
    return *new Material( shaderName );
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


void Material::addParameter( ShaderUniformBase* uniform )
{
    LIBCARNA_ASSERT( !uniform->name.empty() );
    const auto uniformItr = pimpl->uniforms.find( uniform->name );
    if( uniformItr != pimpl->uniforms.end() )
    {
        delete uniformItr->second;
    }
    pimpl->uniforms[ uniform->name ] = uniform;
}


void Material::clearParameters()
{
    for( auto uniformItr = pimpl->uniforms.begin(); uniformItr != pimpl->uniforms.end(); ++uniformItr )
    {
        delete uniformItr->second;
    }
    pimpl->uniforms.clear();
}


void Material::removeParameter( const std::string& name )
{
    const auto uniformItr = pimpl->uniforms.find( name );
    if( uniformItr != pimpl->uniforms.end() )
    {
        delete uniformItr->second;
        pimpl->uniforms.erase( uniformItr );
    }
}


bool Material::hasParameter( const std::string& name ) const
{
    const auto uniformItr = pimpl->uniforms.find( name );
    return uniformItr != pimpl->uniforms.end();
}


const ShaderUniformBase& Material::parameter( const std::string& name ) const
{
    const auto uniformItr = pimpl->uniforms.find( name );
    LIBCARNA_ASSERT( uniformItr != pimpl->uniforms.end() );
    return *uniformItr->second;
}


Material::ManagedInterface* Material::acquireVideoResource()
{
    return new ManagedInterface( *this );
}



}  // namespace Carna :: base

}  // namespace Carna
