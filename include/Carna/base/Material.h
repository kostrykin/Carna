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
#include <Carna/base/GeometryFeature.h>
#include <Carna/base/ShaderManager.h>
#include <Carna/base/ShaderUniform.h>
#include <Carna/base/noncopyable.h>
#include <string>

/** \file   Material.h
  * \brief  Defines \ref Carna::base::Material.
  */

namespace Carna
{

namespace base
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

    const ShaderProgram* shader;
    
protected:

    friend class GeometryFeature;

    virtual ~Material();

public:

    const std::string shaderName;

    void addParameter( ShaderUniformBase* );
    
    template< typename ParameterType >
    void setParameter( const std::string& name, const ParameterType& value );

    void clearParameters();

    void removeParameter( const std::string& name );

    /** \brief
      * Instantiates. Call \ref release when you do not need the object any longer.
      */
    static Material& create( const std::string& shaderName );

    virtual bool controlsSameVideoResource( const GeometryFeature& ) const override;

    // ------------------------------------------------------------------------------
    // Material :: VideoResourceAcquisition
    // ------------------------------------------------------------------------------

    class VideoResourceAcquisition : public GeometryFeature::VideoResourceAcquisition
    {
    
    public:
    
        VideoResourceAcquisition( Material& material );
    
        virtual ~VideoResourceAcquisition();

        void activate( RenderState& rs, GLContext& glc ) const;

        const ShaderProgram& shader() const;
    
        Material& material;
    
    }; // Material :: VideoResourceAcquisition
    
    virtual VideoResourceAcquisition* acquireVideoResource( GLContext& glc ) override;

}; // Material

    
template< typename ParameterType >
void Material::setParameter( const std::string& name, const ParameterType& value )
{
    addParameter( new ShaderUniform< typename ShaderUniformType< ParameterType >::UniformType >( name, value ) );
}



}  // namespace Carna :: base

}  // namespace Carna

#endif // MATERIAL_H_6014714286
