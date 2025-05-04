/*
 *  Copyright (C) 2010 - 2016 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 * 
 * 
 *  Copyright (C) 2021 - 2025 Leonid Kostrykin
 * 
 */

#include <LibCarna/base/ShaderManager.h>
#include <LibCarna/base/Shader.h>
#include <LibCarna/base/ShaderProgram.h>
#include <LibCarna/base/Log.h>
#include <sstream>

namespace LibCarna
{

namespace base
{



// ----------------------------------------------------------------------------------
// ShaderManager :: Details
// ----------------------------------------------------------------------------------

struct ShaderManager::Details : public Log::OnShutdownListener
{
    Details();

    struct ShaderInfo
    {
        std::unique_ptr< Shader > vertShader;
        std::unique_ptr< Shader > geomShader;
        std::unique_ptr< Shader > fragShader;
        std::unique_ptr< ShaderProgram > shader;
        unsigned int acquisitionsCount;
    };

    std::map< std::string, std::string > sources;
    std::map< std::string, ShaderInfo* > loadedShaders;
    std::map< ShaderProgram*, std::string > loadedShaderNames;

    const ShaderProgram& loadShader( const std::string& name );

    bool hasLogShutDown;
    void logLeakedShaders() const;
    virtual void onLogShutdown() override;
};


ShaderManager::Details::Details()
    : hasLogShutDown( false )
{
}


const ShaderProgram& ShaderManager::Details::loadShader( const std::string& name )
{
    const std::string* srcVertPtr = nullptr;
    const std::string* srcGeomPtr = nullptr;
    const std::string* srcFragPtr = nullptr;

    /* Look up the shader sources.
     */
    const auto srcVertItr = sources.find( name + ".vert" );
    const auto srcGeomItr = sources.find( name + ".geom" );
    const auto srcFragItr = sources.find( name + ".frag" );
    if( srcVertItr != sources.end() && srcFragItr != sources.end() )
    {
        srcVertPtr = &srcVertItr->second;
        srcFragPtr = &srcFragItr->second;
    }
    else
    {
        srcVertPtr = &res::string( name + "_vert" );
        srcFragPtr = &res::string( name + "_frag" );
    }
    
    /* Assure the required sources were found.
     */
    LIBCARNA_ASSERT_EX( srcVertPtr != nullptr,   "Vertex shader sources \"" + name + "\" not found!" );
    LIBCARNA_ASSERT_EX( srcFragPtr != nullptr, "Fragment shader sources \"" + name + "\" not found!" );
    
    /* Also look up the optional geometry shader definition.
     */
    if( srcGeomItr != sources.end() )
    {
        srcGeomPtr = &srcGeomItr->second;
    }
    else
    {
        const auto srcGeomItr = res::strings.find( name + "_geom" );
        if( srcGeomItr != res::strings.end() )
        {
            srcGeomPtr = srcGeomItr->second;
        }
    }
    
    /* Prepare creation of new shader program.
     */
    ShaderInfo* const info = new ShaderInfo();
    info->acquisitionsCount = 1;
    loadedShaders[ name ] = info;
    ShaderProgram::Factory factory;

    /* Build shader sources for vertex and fragment shader.
     */
    const std::string& srcVert = *srcVertPtr;
    const std::string& srcFrag = *srcFragPtr;
    
    info->vertShader.reset( new Shader( Shader::TYPE_VERTEX_SHADER  , srcVert ) );
    info->fragShader.reset( new Shader( Shader::TYPE_FRAGMENT_SHADER, srcFrag ) );
    
    factory.setVertexShader  ( *info->vertShader );
    factory.setFragmentShader( *info->fragShader );
    
    /* Build geometry shader sources if they were found.
     */
    if( srcGeomPtr != nullptr )
    {
        const std::string& srcGeom = *srcGeomPtr;
        info->geomShader.reset( new Shader( Shader::TYPE_GEOMETRY_SHADER, srcGeom ) );
        factory.setGeometryShader( *info->geomShader );
    }
    
    /* Build the shader program.
     */
    info->shader.reset( factory.create() );
    loadedShaderNames[ info->shader.get() ] = name;
    return *info->shader;
}


void ShaderManager::Details::logLeakedShaders() const
{
    if( !hasLogShutDown )
    {
        if( !loadedShaders.empty() )
        {
            std::stringstream msg;
            msg << loadedShaders.size() << " shaders leaked!" << std::endl;
            for( auto nameItr = loadedShaders.begin(); nameItr != loadedShaders.end(); ++nameItr )
            {
                const std::string& name = nameItr->first;
                msg << "    \"" << name << "\"" << std::endl;
            }
            Log::instance().record( Log::error, msg.str() );
        }
    }
}


void ShaderManager::Details::onLogShutdown()
{
    logLeakedShaders();
    hasLogShutDown = true;
}



// ----------------------------------------------------------------------------------
// ShaderManager
// ----------------------------------------------------------------------------------

ShaderManager::ShaderManager()
    : pimpl( new Details() )
{
    Log::instance().addOnShutdownListener( *pimpl );
}


ShaderManager::~ShaderManager()
{
    pimpl->logLeakedShaders();
    if( pimpl->hasLogShutDown )
    {
        Log::instance().removeOnShutdownListener( *pimpl );
    }
}


const ShaderProgram& ShaderManager::acquireShader( const std::string& shaderName )
{
    LIBCARNA_ASSERT( !shaderName.empty() );
    const auto infoItr = pimpl->loadedShaders.find( shaderName );
    if( infoItr == pimpl->loadedShaders.end() )
    {
        return pimpl->loadShader( shaderName );
    }
    else
    {
        ++infoItr->second->acquisitionsCount;
        return *infoItr->second->shader;
    }
}

void ShaderManager::releaseShader( const ShaderProgram& shader )
{
    const auto nameItr = pimpl->loadedShaderNames.find( const_cast< ShaderProgram* >( &shader ) );
    LIBCARNA_ASSERT_EX( nameItr != pimpl->loadedShaderNames.end(), "Shader not loaded!" );
    const std::string& shaderName = nameItr->second;
    Details::ShaderInfo& info = *pimpl->loadedShaders[ shaderName ];
    if( --info.acquisitionsCount == 0 )
    {
        delete &info;
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



}  // namespace LibCarna :: base

}  // namespace LibCarna
