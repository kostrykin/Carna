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

#include <Carna/base/view/RendererSharedData.h>
#include <Carna/base/view/ShaderBundle.h>
#include <Carna/base/view/ShaderProgram.h>
#include <Carna/base/model/Volume.h>
#include <Carna/base/CarnaException.h>
#include <QDebug>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// RendererSharedData
// ----------------------------------------------------------------------------------

unsigned int RendererSharedData::referenceCounter = 0;

std::unique_ptr< RendererSharedData > RendererSharedData::instance = std::unique_ptr< RendererSharedData >();


RendererSharedData& RendererSharedData::acquire()
{
    CARNA_ASSERT( ( instance == nullptr ) == ( referenceCounter == 0 ) );

    if( referenceCounter == 0 )
    {
        instance.reset( new RendererSharedData() );
    }

    ++referenceCounter;

    return *instance;
}


void RendererSharedData::release()
{
    CARNA_ASSERT( instance != nullptr && referenceCounter > 0 );

    if( --referenceCounter == 0 )
    {
        instance.reset();
    }
}


RendererSharedData::RendererSharedData()
{
}


RendererSharedData::~RendererSharedData()
{
    const unsigned int shaders_count = shaders.size();
    for( auto it = shaders.begin(); it != shaders.end(); ++it )
    {
        delete it->second;
    }
    qDebug() << "Renderer released" << shaders_count << "shaders.";
}


ShaderProgram& RendererSharedData::getShader( const std::string& id )
{
    return getShader
        ( ShaderBundle::vertexShaderFileById( id )
        , ShaderBundle::fragmentShaderFileById( id ) );
}


ShaderProgram& RendererSharedData::getShader
    ( const std::string& vertex_shader_file
    , const std::string& fragment_shader_file )
{
    std::stringstream id_stream;
    id_stream << vertex_shader_file << ":" << fragment_shader_file;
    const std::string id = id_stream.str();

    std::map< std::string, ShaderBundle* >::iterator it = shaders.find( id );

    if( it != shaders.end() )
    {
        return it->second->program();
    }
    else
    {
        ShaderBundle* shader = new ShaderBundle( vertex_shader_file, fragment_shader_file );
        shaders[ id ] = shader;
        qDebug() << "Renderer installed shader:" << QString::fromStdString( id );
        return shader->program();
    }
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
