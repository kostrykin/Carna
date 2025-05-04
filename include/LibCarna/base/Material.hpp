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

#ifndef MATERIAL_H_6014714286
#define MATERIAL_H_6014714286

#include <LibCarna/LibCarna.hpp>
#include <LibCarna/base/GeometryFeature.hpp>
#include <LibCarna/base/ShaderManager.hpp>
#include <LibCarna/base/ShaderUniform.hpp>
#include <LibCarna/base/noncopyable.hpp>
#include <string>

/** \file
  * \brief
  * Defines \ref LibCarna::base::Material.
  */

namespace LibCarna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Material
// ----------------------------------------------------------------------------------
    
/** \brief
  * Specifies the \ref ShaderProgram "shader" and it's configuration that are to be
  * used for rendering a \ref Geometry node with a \ref MeshRenderingStage. Custom
  * implementations could also tweak the \ref RenderState "render state".
  *
  * \section RenderingPipeline Rendering Pipeline
  *
  * The core part of a material is the shader, which requires a basic understanding
  * of the *rendering pipeline*. A very good explanation can be found here:
  * http://www.lighthouse3d.com/tutorials/glsl-core-tutorial/pipeline33/
  * Nevertheless, lets pass through the most important aspects of the pipeline. The
  * pipeline consists of stages, that are distinguished by whether they are
  * programmable or not. Programmable stages are called *shaders*.
  *
  *  1. The first stage always is the *vertex shader* that is invoked once for each
  *     vertex from the vertex buffer. It takes one vertex per invocation as input,
  *     transforms it and writes it to the output. Typically the output vertices are
  *     written as \ref ClippingCoordinates "clipping coordinates".
  *  2. Within the *primitive assembly stage* the vertices are assembled to
  *     *primitives* according to the index buffer.
  *  3. The optional *geometry shader* is invoked once for each primitive. For each
  *     invocation, it writes an arbitrary number of primitives as output. This means
  *     that the geometry shader can be used for culling primitives as well as for
  *     the generation of new ones.
  *  4. The *rasterization and interpolation* stage maps the primitives on top of a
  *     grid, whose cells are called *fragments*. Fragments are basically pixels, but
  *     they are given in \ref WindowCoordinates "window coordinates", hence they
  *     have a depth component in particular.
  *  5. The *fragment shader* is invoked once for each fragment of the pritimive. It
  *     either writes the fragment's attributes like its color and its depth, or it
  *     discards the fragment. It is neither possible to generate new fragments, nor
  *     to change other fragment's coordinates but its depth.
  *  6. The last stage maps the primitive's fragments to the color buffer's *pixels*
  *     by application of \ref RenderState::setBlendFunction "alpha blending". It
  *     also performs tests, like the
  *     \ref RenderState::setDepthTestFunction "depth test", on each fragment.
  *
  * Note that the depth test might be performed within stage 4 already, if stage 5
  * does not alter the precomputed fragment depth. This is usually faster, because
  * less fragments need to be processed by stage 5.
  *
  * \section CustomMaterials Custom Materials
  *
  * As already mentioned in the section above, the core part of a material is its
  * shader. A shader, actually the *shader program*, consists of a *vertex shader*
  * and a *fragment shader* at least. It may also have a *fragment shader*, which is
  * optional. All three of these are explained in the section above.
  *
  * So the first step in creating a new material most likely will be the
  * implementation of a vertex and a fragment shader.
  *
  * \subsection GLSL_Vert Vertex Shaders
  *
  * A typical vertex shader looks like this:
  *
  *     \code{.unparsed}
  *     #version 330
  *     uniform mat4 modelViewProjection;
  *     layout( location = 0 ) in vec4 inPosition;
  *     void main()
  *     {
  *         vec4 clippingCoordinates = modelViewProjection * inPosition;
  *         gl_Position = clippingCoordinates;
  *     }
  *     \endcode
  *
  * Line 1 specifies the GLSL version that the compiler should use to build the code.
  * Line 2 declares a variable, whose value will be set from code that runs on the
  * CPU. The keyword `uniform` reflects the fact the value will be the same for each
  * processed vertex. Line 3 declares the \ref CustomVertexFormats "vertex format".
  * The `main` function is invoked for each processed vertex. The example code above
  * assumes that the vertices are stored in
  * \ref CoordinateTransformations "model space" within the vertex buffer and
  * transforms them to \ref ClippingCoordinates "clipping coordinates".
  *
  * Following uniform variables will be set by the `%Material` class automatically,
  * if declared:
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
  * \subsection GLSL_Frag Fragment Shaders
  *
  * Lets take a look at an exemplary fragment shader:
  *
  *     \code{.unparsed}
  *     #version 330
  *     uniform vec4 color;
  *     out vec4 gl_FragColor;
  *     void main()
  *     {
  *         gl_FragColor = vec4( color );
  *     }
  *     \endcode
  *
  * Line 1 declares the GLSL version, just like it was did for vertex shaders. Vertex
  * and fragment shaders, that are going to be used together as a single
  * \ref ShaderProgram "shader program" must also use the same GLSL version. Line 2
  * declares a variable, whose value will be set
  * \ref setParameter "when the shader is triggered". Note that the keyword `uniform`
  * reflects the same intuitive meaning as it did in the vertex shader example above.
  * Line 3 declares a variable, that the fragment shader will write its output to.
  * You won't need a different declaration than this, unless you are going to render
  * to multiple color buffers at once, which is an advanced technique. Just like for
  * vertex shaders, the `main` function of the fragment shader will be executed once
  * for each fragment. The shader code above colors the whole rendered object in a
  * \a uniform color.
  *
  * \subsection ShaderMaterialIntegration Integration into Materials
  *
  * The `%Material` class acquires its shader objects from the \ref ShaderManager,
  * which ensures that each shader is built just once and not each time someone
  * requests it. So everything one has to do is to ensure that the `%ShaderManager`
  * will be able to locate the shader sources:
  *
  *     \code
  *     using namespace LibCarna;
  *     base::ShaderManager::instance().setSource( "myShader.vert", srcVert );
  *     base::ShaderManager::instance().setSource( "myShader.frag", srcFrag );
  *     \endcode
  *
  * This needs to be done *before* the material first tries to acquire the shader,
  * i.e. before the geometry node, the material is attached to, is rendered.
  *
  * The suffixes `.vert` and `.frag` are important. You will be able to reference the
  * shader from a `%Material` object by setting its `shaderName` to `myShader`.
  *
  * \author Leonid Kostrykin
  */
class LIBCARNA Material : public GeometryFeature
{

    NON_COPYABLE

    struct Details;
    const std::unique_ptr< Details > pimpl;

    const ShaderProgram* shader;
    
protected:

    friend class GeometryFeature;
    
    /** \brief
      * Instantiates.
      */
    Material( const std::string& shaderName );
    
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
      * when the material is \ref ManagedInterface::activate "activated".
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
      * Tells whether a paramter named \a name exists.
      */
    bool hasParameter( const std::string& name ) const;
    
    /** \brief
      * References the paramter named \a name.
      * \pre `hasParameter(name) == true`
      */
    const ShaderUniformBase& parameter( const std::string& name ) const;

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
    // Material :: ManagedInterface
    // ------------------------------------------------------------------------------
    
    /** \brief
      * Represents an acquisition of video resources from a particular \ref Material.
      * This realizes the RAII idiom.
      *
      * \author Leonid Kostrykin
      */
    class LIBCARNA ManagedInterface : public GeometryFeature::ManagedInterface
    {
    
    public:
    
        /** \brief
          * Acquires the video resources from \a material.
          *
          * \copydetails GeometryFeature::ManagedInterface::ManagedInterface(GeometryFeature&)
          */
        ManagedInterface( Material& material );
    
        /** \copydoc GeometryFeature::ManagedInterface::~ManagedInterface()
          */
        virtual ~ManagedInterface();

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
    
    }; // Material :: ManagedInterface

    // ------------------------------------------------------------------------------
    
    virtual ManagedInterface* acquireVideoResource() override;

}; // Material

    
template< typename ParameterType >
void Material::setParameter( const std::string& name, const ParameterType& value )
{
    addParameter( new ShaderUniform< ParameterType >( name, value ) );
}



}  // namespace LibCarna :: base

}  // namespace LibCarna

#endif // MATERIAL_H_6014714286
