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

#include <Carna/base/view/ShaderBundle.h>
#include <Carna/base/view/VertexShader.h>
#include <Carna/base/view/FragmentShader.h>
#include <Carna/base/view/ShaderProgram.h>
#include <Carna/base/view/ShaderCompilationError.h>
#include <Carna/base/Text.h>
#include <sstream>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// ShaderBundle
// ----------------------------------------------------------------------------------

ShaderBundle::ShaderBundle( const std::string& shader_id )
{
    init( vertexShaderFileById( shader_id ), fragmentShaderFileById( shader_id ) );
}


ShaderBundle::ShaderBundle( const std::string& vshader_file
                          , const std::string& fshader_file )
{
    init( vshader_file, fshader_file );
}


ShaderBundle::~ShaderBundle()
{
}


std::string ShaderBundle::vertexShaderFileById( const std::string& id )
{
    std::stringstream ss;
    ss << ":/shaders/" << id << ".vert";
    return ss.str();
}


std::string ShaderBundle::fragmentShaderFileById( const std::string& id  )
{
    std::stringstream ss;
    ss << ":/shaders/" << id << ".frag";
    return ss.str();
}


void ShaderBundle::init( const std::string& vshader_file
                       , const std::string& fshader_file )
{
    try
    {
        const std::string src_vshader = Text::cat( vshader_file );
        const std::string src_fshader = Text::cat( fshader_file );

        if( src_vshader.empty() || src_fshader.empty() )
        {
            throw ShaderCompilationError( "empty shader definition" );
        }

        vertexShader  .reset( new VertexShader  ( src_vshader ) );
        fragmentShader.reset( new FragmentShader( src_fshader ) );
        shaderProgram .reset( new ShaderProgram ( *vertexShader, *fragmentShader ) );
    }
    catch( ShaderCompilationError& ex )
    {
        throw ShaderCompilationError( ex.details, vshader_file, fshader_file );
    }
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
