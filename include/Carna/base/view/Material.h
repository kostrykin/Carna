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

#ifndef MATERIAL_H_6014714286
#define MATERIAL_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/view/GeometryFeature.h>
#include <Carna/base/view/ShaderManager.h>
#include <Carna/base/noncopyable.h>
#include <string>

/** \file   Material.h
  * \brief  Defines \ref Carna::base::view::Material.
  */

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// Material
// ----------------------------------------------------------------------------------
    
/** \brief
  * Specifies the shader and it's configuration that are to be used for rendering a
  * \ref Geometry node with a \ref MeshRenderingStage.
  *
  * \author Leonid Kostrykin
  * \date 27.2.2015
  */
class CARNA_LIB Material : public GeometryFeature
{

    NON_COPYABLE

    Material( const std::string& shaderName );

    struct Details;
    const std::unique_ptr< Details > pimpl;

    const ShaderProgram* myShader;

public:

    virtual ~Material();

    const std::string shaderName;

    /** \brief
      * Instantiates. Call \ref release when you do not need the object any longer.
      */
    static Material& create( const std::string& shaderName );

    virtual void acquireVideoResource() override;

    virtual void releaseVideoResource() override;

    const ShaderProgram& shader() const;

    virtual bool controlsSameVideoResource( const GeometryFeature& ) const override;

    void activate( RenderState&, GLContext& ) const;

    void addUniform( ShaderUniformBase* );

    void clearUniforms();

    void removeUniform( const std::string& name );

}; // Material



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // MATERIAL_H_6014714286
