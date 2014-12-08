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

#ifndef SHADER_BUNDLE_H_6014714286
#define SHADER_BUNDLE_H_6014714286

/** \file   ShaderBundle.h
  * \brief  Defines \ref Carna::base::view::ShaderBundle.
  */

#include <Carna/Carna.h>
#include <string>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// ShaderBundle
// ----------------------------------------------------------------------------------

/** \brief  Composes a \ref VertexShader and a \ref FragmentShader along with the resulting \ref ShaderProgram.
  *
  * \author Leonid Kostrykin
  * \date   2011 - 2012
  */
class CARNA_EXPORT ShaderBundle
{

public:

    /** \brief	Loads the shaders from resource file.
      *
      * The shaders loaded are <code>/shaders/shader_id.vert</code>
      * and <code>/shaders/shader_id.frag</code>.
      *
      * \throws std::runtime_error   when the files are empty, cannot be opened or the
      *                              shader code contains errors.
      */
    explicit ShaderBundle( const std::string& shader_id );

    /** \brief	Loads the shaders from given files.
     *
     * \throws std::runtime_error   when the files are empty, cannot be opened or the
     *                              shader code contains errors.
     */
    ShaderBundle( const std::string& vertex_shader_file, const std::string& fragment_shader_file );

    /** \brief	Releases the shaders.
      */
    ~ShaderBundle();


    /** \brief	References the compiled shader program.
      */
    ShaderProgram& program()
    {
        return *shaderProgram;
    }

    
    /** \brief	Predicts the shader ID decomposition performed by
      *         \ref ShaderBundle::ShaderBundle(const std::string&) "this constructor".
      */
    static std::string vertexShaderFileById( const std::string& id );
    
    /** \brief	Predicts the shader ID decomposition performed by
      *         \ref ShaderBundle::ShaderBundle(const std::string&) "this constructor".
      */
    static std::string fragmentShaderFileById( const std::string& id );


private:

    std::unique_ptr< VertexShader   > vertexShader;
    std::unique_ptr< FragmentShader > fragmentShader;
    std::unique_ptr< ShaderProgram  > shaderProgram;


    void init( const std::string& vshader_file, const std::string& fshader_file );

}; // ShaderBundle



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // SHADER_BUNDLE_H_6014714286
