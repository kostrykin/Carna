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
  * \section CustomMaterials Custom Materials
  *
  * \todo finish this
  *
  * Following uniform variables are available:
  *
  *   - `modelView` takes the model-view transform. This is the concatenation of the
  *     \ref Camera::viewTransform "view transform" and the
  *     \ref Spatial::worldTransform "world transform". It maps from model space to
  *     view space.
  *   - `projection` takes the \ref Camera::projection "projection matrix" that maps
  *     from view space to clipping coordinates.
  *   - `modelViewProjection` is the concatenation of `projection` and `modelView`.
  *     It maps from model space to clipping coordinates.
  *
  * The coordinate systems and their relations to each other are explained
  * \ref CoordinateSystems "here".
  *
  * \author Leonid Kostrykin
  * \date   22.2.15 - 18.3.15
  */
class CARNA_LIB Material : public GeometryFeature
{

    NON_COPYABLE
        
    /** \brief
      * Instantiates.
      */
    Material( const std::string& shaderName );

    struct Details;
    const std::unique_ptr< Details > pimpl;

    const ShaderProgram* shader;
    
protected:

    friend class GeometryFeature;
    
    /** \brief
      * Deletes.
      */
    virtual ~Material();

public:

    /** \brief
      * Identifies this material's shader.
      */
    const std::string shaderName;

    /** \brief
      * Records \a uniform as shader parameter. This will be uploaded to the shader
      * when the material is \ref VideoResourceAcquisition::activate "activated".
      *
      * Any previously set parameters with the same
      * \ref ShaderUniformBase::name "name" are overriden.
      */
    void addParameter( ShaderUniformBase* uniform );
    
    /** \overload
      */
    template< typename ParameterType >
    void setParameter( const std::string& name, const ParameterType& value );

    /** \brief
      * Removes all previously set parameters.
      */
    void clearParameters();

    /** \brief
      * Removes the parameter named \a name if it exists.
      */
    void removeParameter( const std::string& name );

    /** \brief
      * Instantiates. Call \ref release when you do not need the object any longer.
      */
    static Material& create( const std::string& shaderName );

    /** \copydoc GeometryFeature::controlsSameVideoResource(const GeometryFeature&) const
      *
      * This implementation checks whether \a other also is a `%Material` and if yes,
      * whether its \ref shaderName is the same.
      */
    virtual bool controlsSameVideoResource( const GeometryFeature& other ) const override;

    // ------------------------------------------------------------------------------
    // Material :: VideoResourceAcquisition
    // ------------------------------------------------------------------------------
    
    /** \brief
      * Represents an acquisition of video resources from a particular \ref Material.
      * This realizes the RAII idiom.
      *
      * \author Leonid Kostrykin
      * \date   22.2.15 - 18.3.15
      */
    class CARNA_LIB VideoResourceAcquisition : public GeometryFeature::VideoResourceAcquisition
    {
    
    public:
    
        /** \brief
          * Acquires the video resources from \a material.
          *
          * \copydetails GeometryFeature::VideoResourceAcquisition::VideoResourceAcquisition(GeometryFeature&)
          */
        VideoResourceAcquisition( Material& material );
    
        /** \copydoc GeometryFeature::VideoResourceAcquisition::~VideoResourceAcquisition()
          */
        virtual ~VideoResourceAcquisition();

        /** \brief
          * Activates \ref material "this material" by setting the
          * \ref shader "proper shader" on the current context, configuring it and
          * tweaking the \a renderState.
          */
        void activate( RenderState& renderState ) const;

        /** \brief
          * References the shader of \ref material "this material".
          */
        const ShaderProgram& shader() const;
    
        /** \brief
          * References the material.
          */
        Material& material;
    
    }; // Material :: VideoResourceAcquisition
    
    virtual VideoResourceAcquisition* acquireVideoResource() override;

}; // Material

    
template< typename ParameterType >
void Material::setParameter( const std::string& name, const ParameterType& value )
{
    addParameter( new ShaderUniform< typename ShaderUniformType< ParameterType >::UniformType >( name, value ) );
}



}  // namespace Carna :: base

}  // namespace Carna

#endif // MATERIAL_H_6014714286
