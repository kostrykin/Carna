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

#ifndef RENDERERSHAREDDATA_H_6014714286
#define RENDERERSHAREDDATA_H_6014714286

/** \file   RendererSharedData.h
  * \brief  Defines \ref Carna::base::view::RendererSharedData.
  */

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>
#include <map>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// RendererSharedData
// ----------------------------------------------------------------------------------

/** \brief  Data shared between all \ref Renderer instances
  *
  * \author Leonid Kostrykin
  * \date   1.10.12 - 6.3.13
  */
class RendererSharedData
{

    NON_COPYABLE

    RendererSharedData();

    static unsigned int referenceCounter;

    static std::unique_ptr< RendererSharedData > instance;

public:

    ~RendererSharedData();


    static RendererSharedData& acquire();

    static void release();
    

    ShaderProgram& getShader( const std::string& id );

    ShaderProgram& getShader
        ( const std::string& vertex_shader_file
        , const std::string& fragment_shader_file );


private:

    std::map< std::string, ShaderBundle* > shaders;

}; // RendererSharedData



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // RENDERERSHAREDDATA_H_6014714286
