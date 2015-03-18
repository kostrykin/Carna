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

#include <Carna/base/ShaderManager.h>
#include <Carna/base/Shader.h>
#include <Carna/base/ShaderProgram.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// ShaderManager :: Details
// ----------------------------------------------------------------------------------

struct ShaderManager::Details
{

    struct ShaderInfo
    {
        ShaderProgram* shader;
        Shader* vertexShader;
        Shader* fragmentShader;
        unsigned int acquisitionsCount;
    };

    std::map< std::string, std::string > sources;
    std::map< std::string, ShaderInfo > loadedShaders;
    std::map< ShaderProgram*, std::string > loadedShaderNames;

    const ShaderProgram& loadShader( const std::string& name );

}; // ShaderManager :: Details


const ShaderProgram& ShaderManager::Details::loadShader( const std::string& name )
{
    const std::string* srcVertPtr;
    const std::string* srcFragPtr;

    /* Look up the shader sources.
     */
    const auto srcVertItr = sources.find( name + ".vert" );
    const auto srcFragItr = sources.find( name + ".frag" );
    if( srcVertItr != sources.end() && srcFragItr != sources.end() )
    {
        srcVertPtr = &*srcVertItr;
        srcFragPtr = &*srcFragItr;
    }
    else
    {
        srcVertPtr = &res::string( name + "_vert" );
        srcFragPtr = &res::string( name + "_frag" );
    }

    /* Build the shader sources.
     */
    const std::string& srcVert = *srcVertPtr;
    const std::string& srcFrag = *srcFragPtr;

    ShaderInfo info;
    info.acquisitionsCount = 1;
    info.vertexShader = new Shader( Shader::TYPE_VERTEX_SHADER, srcVert );
    info.fragmentShader = new Shader( Shader::TYPE_FRAGMENT_SHADER, srcFrag );
    info.shader = new ShaderProgram( *info.vertexShader, *info.fragmentShader );

    loadedShaders[ name ] = info;
    loadedShaderNames[ info.shader ] = name;

    return *info.shader;
}



// ----------------------------------------------------------------------------------
// ShaderManager
// ----------------------------------------------------------------------------------

ShaderManager::ShaderManager()
    : pimpl( new Details() )
{
}


ShaderManager::~ShaderManager()
{
}


const ShaderProgram& ShaderManager::acquireShader( const std::string& shaderName )
{
    const auto infoItr = pimpl->loadedShaders.find( shaderName );
    if( infoItr == pimpl->loadedShaders.end() )
    {
        return pimpl->loadShader( shaderName );
    }
    else
    {
        ++infoItr->second.acquisitionsCount;
        return *infoItr->second.shader;
    }
}

void ShaderManager::releaseShader( const ShaderProgram& shader )
{
    const auto nameItr = pimpl->loadedShaderNames.find( const_cast< ShaderProgram* >( &shader ) );
    CARNA_ASSERT_EX( nameItr != pimpl->loadedShaderNames.end(), "Shader not loaded!" );
    const std::string& shaderName = nameItr->second;
    Details::ShaderInfo& info = pimpl->loadedShaders[ shaderName ];
    if( --info.acquisitionsCount == 0 )
    {
        delete info.shader;
        pimpl->loadedShaders.erase( shaderName );
        pimpl->loadedShaderNames.erase( nameItr );
    }
}


void ShaderManager::setSource( const std::string& srcName, const std::string& src )
{
    pimpl->sources[ srcName ] = src;
}


void ShaderManager::removeSource( const std::string& srcName )
{
    pimpl->sources.erase( srcName );
}



}  // namespace Carna :: base

}  // namespace Carna
