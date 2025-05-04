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

#ifndef VOLUMERENDERINGSTAGE_H_6014714286
#define VOLUMERENDERINGSTAGE_H_6014714286

#include <LibCarna/base/GeometryStage.h>
#include <LibCarna/base/Renderable.h>
#include <LibCarna/LibCarna.h>
#include <map>

/** \file   VolumeRenderingStage.h
  * \brief  Defines \ref Carna::presets::VolumeRenderingStage.
  */

namespace Carna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// VolumeRenderingStage
// ----------------------------------------------------------------------------------

/** \brief
  * Defines abstract base class for \ref base::RenderStage "rendering stages" that
  * render volume geometries in the scene.
  *
  * The necessity to evaluate \f$f\left(x_i\right)\f$ for equidistant samples
  * \f$x_i\f$ of a ray, that is shot from each pixel of the screen into the volume
  * geometries of the scene, is common to the typical volume rendering applications
  * like *digital radiograph reconstructs*, *maximum intensity projections* or
  * *direct volume renderings*.
  *
  * \section VolumeRenderingApproach Approach
  *
  * The approach, that probably seems most natural, is the following. By rendering a
  * bounding box around the volume, we would get an invocation of a fragment shader
  * for each such pixel whose ray intersects the volume. The next step would be to
  * somehow obtain the first and last intersection locations \f$x_1\f$ and \f$x_n\f$
  * respectively within the shader. Finally one would iterate over the equidistant
  * samples \f$x_i\f$ and evaluate them through \f$f\f$.
  *
  * This actually is a very wide-spread approach to volume rendering, that is called
  * *ray marching*. It has two disadvantages. First, the looping within the fragment
  * shader comes at the cost of heavy dynamic branching, that slows down the GPU a
  * lot. Second, it requires quite a lot of shader logic to be implemented
  * redundantly for each application. For this two reasons, this class uses a
  * different approach, but one that is certainly inspired by ray marching.
  *
  * \image html VolumeRenderingApproach.png "on the left: the ray marching approach - on the right: algorithm implemented here"
  *
  * The idea is to use *precomputed* sample locations. Therefor the samples \f$x_i\f$
  * of different rays but with same \f$i\f$ are identified as *slices*. The creation
  * of a mesh that consists of \f$n\f$ slices is easy. The challenge is to position
  * and scale that mesh s.t. it covers the whole volume, i.e. its bounding box like
  * illustrated in the figure above, irrespectively of the view direction. The mesh
  * is oriented s.t. the slices become orthogonal to the viewing direction. It is
  * positioned s.t. its center matches the center of the bounding box. The scaling is
  * a little tricky: We stretch it s.t. it becomes the minimum boundary box to the
  * volume's *bounding ellipsoid*.
  *
  * \section VolumeRenderingAlgorithm Algorithm
  *
  * This is an abstract base class for volume rendering stages. The algorithm
  * constructs a sequence of slices. These slices are rendered at the location of
  * every \ref base::Geometry node \ref GeometryTypes "that this stage processes".
  * The rendering is done s.t. the slices always face exactly towards the camera.
  * Implementations of this class must provide shader that satisfies specific
  * requirements.
  *
  * \see
  * A short introduction to shaders is given in the \ref CustomMaterials section.
  *
  * It is within the responsibility of this shader to transform the vertices of the
  * slices s.t. they are put in place correctly. Furthermore it is of course the job
  * of the shader to do the texturing of the slices. This abstract class supports the
  * shader in accomplishing both requirements, as explained below.
  *
  * \subsection VolumeRenderingTransformations Transformations
  *
  * The vertices of the slices mesh are defined in *tangent space* coordinates. The
  * z-coordinate is the same for the vertices of a single slice and grows for slices
  * that will be rendered closer to the camera. As usual, rendering requires the
  * transformations of vertices to \ref CoordinateSystems "eye space and beyond".
  * This is done in two steps:
  *
  *  1. The vertices are transformed to the *model space* of the geometry node. These
  *     coordinates will also be used for texturing, as explained later.
  *  2. From *model space* the coordinates are than transformed to
  *     \ref ClippingCoordinates "clipping coordinates" as it is always done.
  *
  * \subsubsection VolumeRenderingTangentModel Tangent to Model
  *
  * This class computes a matrix that accomplishes the transformation from tangent
  * space to model space. This is pretty easy: The vector of the view direction,
  * transformed to model space by the inverse model-view matrix, is picked as the
  * the *normal* vector of the tangent space, i.e. the z-component basis vector.
  *
  * Note that we do not have any requirements upon how the vertices shall be rotated
  * within the plane, the normal vector defines, as long as their rotation is
  * consistent with the texturing. Thus an
  * \ref base::math::orthogonal3f "arbitrary orthogonal vector" is constructed, that
  * is taken as the *tangent*, i.e. the x-component basis vector. The *bitangent*
  * vector for the y-component is than easily obtained by the cross product of the
  * others two.
  *
  * The finished *tangent-model matrix* is uploaded to the shader as an uniform named
  * `tangentModel` from type `mat4`.
  *
  * \subsubsection VolumeRenderingModelTexture Model to Texture
  *
  * The volumetric data that is attached to a geometry node is always assumed to be
  * centered within the node. Furthermore the data is scaled s.t. it shapes an
  * unitary cube. This is convenient because OpenGL requires us to specify texture
  * coordinates within \f$\left[0, 1\right]^3\f$. This is why the transformation of
  * model coordinates to texture coordinates requires us to translate the model
  * coordinates by \f$\left(0{,}5, 0{,}5, 0{,}5\right)^\mathrm T\f$ first.
  *
  * One has to know that a texture, when it is applied to a triangle or a quad,
  * induces a grid of *equally* sized cells. The question arises, where
  * exactly the values from the texture were mapped to. The answer
  * is, that they are mapped to the cell *centers* and not the grid intersections, as
  * one might expect. As a consequence, not only the values in between of the texture
  * values are interpolated, but also the values on the edges of the texture. These
  * extrapolated edges are usually not expected by applications. This class
  * \ref base::ManagedTexture3D::textureCoordinatesCorrection "obtains a matrix" that
  * shifts and scales the texture coordinates s.t. these edges are avoided. It
  * creates the illusion as if the texture values were actually mapped to the grid
  * intersections.
  *
  * The matrix, that results from the concatenation of the translation and the
  * edges-correction matrix, is uploaded to the shader as an uniform named
  * `modelTexture` from type `mat4`.
  *
  * \subsection VolumeRenderingOrder Rendering
  *
  * Usually volume data is partitioned into smaller textures. This reduces the
  * probability of out-of-memory exceptions due to memory fragmentation. The
  * \ref helpers::VolumeGridHelper class performs such a partitioning. This means
  * that we will often render not only single volumes but *grids* where each cell is
  * made up by a volume that needs to be rendered. The algorithm presented here suits
  * this use-case without compromises. However, additional measures need to be taken
  * to avoid artifacts when adjacent cells, i.e. volumes, are rendered. We will go
  * through the two types of artifacts that may arise, look at their causes and how
  * this class avoids them.
  *
  * \subsubsection VolumeRenderingAdjacencyArtifacts Adjacency Artifacts
  *
  * The first type of artifacts encounters when \f$f\left(x_i\right)\f$ are somehow
  * accumulated, like for digital radiograph reconstructs or direct volume
  * renderings. As the figure below illustrates, voxels on the volume edges may be 
  * sampled with a higher rate, because the same voxel is processed twice, once when
  * rendering the left volume and once when rendering the right one. This causes
  * faulty accumulated results.
  *
  * \image html VolumeRenderingArtifacts1.png "on the left: artifacts when rendering without depth test/write - on the right: how these artifacts are caused"
  *
  * The solution is rather simple:
  *
  *   - We ensure that for each volume the slices are rendered from back to front.
  *     This is easy to achieve when payed attention to during the mesh creation.
  *   - We also ensure that the volumes are rendered from back to front, like it is
  *     a common practice for rendering transparent geometries.
  *   - Finally we turn on depth testing and depth writing. This ensures that edge
  *     voxels are only processed once.
  *
  * \subsubsection VolumeRenderingOcclusionArtifacts Occlusion Artifacts
  *
  * The other type of artifacts arises straight from the solution for the first one,
  * depending on how the depth sorting is accomplished. Common implementations
  * compute object distances by evaluating the distance to their centers. If this
  * method was used here, we would observe the artifacts illustrated in the figure
  * below when rendering grid cells of different sizes.
  *
  * \image html VolumeRenderingArtifacts2.png "on the left: artifacts when using distances to centers - on the right: how these artifacts are caused"
  *
  * The term *watershed* in the picture above refers to an analogy: If the eye is
  * located on the left side, than the left cell is considered closer, otherwise the
  * right cell. The numbers refer to the (reverse) recognized depth order, i.e. the
  * order of rendering the volumes. The watershed, computed from the
  * center-distances, is perfectly fine for equally sized cells, but becomes wrong if
  * one cell is smaller: Here the watershed is shifted to left of where it actually
  * should be. The left cell is rendered first, because the right one is considered
  * closer. This prevents the red-shaded area of the right cell from being rendered
  * afterwards, because the depth test fails in this area.
  *
  * The solution is to use a different distance measuring. Instead of computing the
  * distances to the cell centers, we compute the *actual* distances to the cells.
  * The \ref base::Renderable::DepthOrder "depth-sorting implementation" does this if
  * an appropriate \ref base::BoundingBox "bounding box" is set upon the geometry
  * node. The \ref helpers::VolumeGridHelper class configures such bounding boxes for
  * you.
  *
  * \section VolumeRenderingHowToImplementat How to Implement
  *
  * It is important to have an idea of how shaders access textures. For each texture
  * that a shader is able to read from, it must have an uniform variable declared. It
  * is in the responsibility of the CPU-side program to map such variables to texture
  * units. The uniform variable will then reflect the texture that is currently bound
  * to that unit. For geometry nodes that \ref GeometryTypes "this stage processes",
  * it looks for such \ref GeometryFeatures "geometry features" that are from type
  * \ref base::ManagedTexture3D. It then queries the names of the uniform variables,
  * that these textures shall be linked with, from the implementation.
  *
  * \subsection VolumeRenderingDeriving Deriving the Class
  *
  * There are four functions that must be implemented:
  *
  *   - \ref acquireShader acquires the shader to be used for
  *     \ref VolumeRenderingAlgorithm "rendering the slices".
  *   - \ref uniformName maps the volume texture roles to uniform variable names.
  *   - \ref configureShader performs arbitrary setup of the shader.
  *   - \ref createVolumeSamplers creates \ref base::Sampler "texture samplers" and
  *     assigns them to the roles that they should be used with.
  *
  * Furthermore, you might want to override \ref renderPass. The default
  * implementation invokes the volume rendering algorithm, as it is described above.
  * It is a typical practice for implementations of this class to override this
  * method s.t. it setups a different render target, than invokes the default
  * implementation to render to this target, and finally to process the results by
  * rendering them back to the actually configured output buffer. If alpha blending
  * is used while doing the volume rendering to a dedicated buffer, than this step is
  * frequently referred to as *accumulation*. For example, the \ref MIPStage
  * accumulates using the `GL_MAX`
  * \ref base::RenderState::setBlendEquation "blend equation", or the \ref DRRStage
  * accumulates with `GL_ADD` to compute an integral.
  *
  * \subsection VolumeRenderingShader Writing the Shader
  *
  * Below are a few hints on how to implement the shader. Refer to the
  * \ref ShaderMaterialIntegration section for hints on how to actually load the
  * shader.
  * 
  * \subsubsection VolumeRenderingVertexShader Vertex Shader
  *
  * The vertex shader must declare the following GLSL version, uniform variables and
  * \ref CustomVertexFormats "vertex formats":
  *
  *     \code
  *     #version 330
  *     uniform mat4 tangentModel;
  *     uniform mat4 modelViewProjection;
  *
  *     layout( location = 0 ) in vec4 inPosition;
  *     \endcode
  *
  * Furthermore, it must pass the model-space coordinates to the fragment shader,
  * although the name of the variable used for this is arbitrary:
  *
  *     \code
  *     out vec4 modelSpaceCoordinates;
  *     \endcode
  *
  * Most of the time you should be able to stick to this implementation:
  *
  *     \code
  *     void main()
  *     {
  *         modelSpaceCoordinates = tangentModel * inPosition;
  *         vec4 clippingCoordinates = modelViewProjection * modelSpaceCoordinates;
  *         gl_Position = clippingCoordinates;
  *     }
  *     \endcode
  *
  * \subsubsection VolumeRenderingFragmentShader Fragment Shader
  *
  * The fragment shader must declare the following GLSL version and uniform
  * variables, along with a varying for the model-space coordinates, whose name must
  * match the name used in the vertex shader:
  *
  *     \code
  *     #version 330
  *
  *     uniform mat4 modelTexture;
  *
  *     in  vec4 modelSpaceCoordinates;
  *     out vec4 gl_FragColor;
  *     \endcode
  *
  * The name of the color output variable `gl_FragColor` is arbitrary.
  *
  * Below is the body of a typical implementation of the fragment shader, that writes
  * the texture coordinates to the color output:
  *
  *     \code
  *     void main()
  *     {
  *         if( abs( modelSpaceCoordinates.x ) > 0.5 || abs( modelSpaceCoordinates.y ) > 0.5 || abs( modelSpaceCoordinates.z ) > 0.5 )
  *         {
  *             discard;
  *         }
  *         vec4 textureCoordinates = modelTexture * modelSpaceCoordinates;
  *         gl_FragColor = vec4( textureCoordinates.rgb, 1 );
  *     }
  *     \endcode
  *
  * For a full example on how to implement the shader, refer to the files
  * \ref src/res/mip.vert and \ref src/res/mip.frag. These should be
  * self-explaining.
  *
  * \author Leonid Kostrykin
  */
class LIBCARNA VolumeRenderingStage : public base::GeometryStage< base::Renderable::BackToFront >
{

    struct Details;
    const std::unique_ptr< Details > pimpl;

    struct VideoResources;
    std::unique_ptr< VideoResources > vr;

public:

    /** \brief
      * Holds the default number of slices rendered per segment.
      */
    const static unsigned int DEFAULT_SAMPLE_RATE = 200;

    /** \brief
      * Instantiates. The created stage will render such \ref base::Geometry scene
      * graph nodes, whose \ref GeometryTypes "geometry types" equal \a geometryType.
      */
    explicit VolumeRenderingStage( unsigned int geometryType );

    /** \brief
      * Deletes.
      */
    virtual ~VolumeRenderingStage();

    /** \brief
      * Sets number of slices to be rendered per segment.
      * \pre `sampleRate >= 2`
      */
    void setSampleRate( unsigned int sampleRate );
    
    /** \brief
      * Tells number of slices to be rendered per segment.
      * \pre `sampleRate() >= 2`
      */
    unsigned int sampleRate() const;

    /** \brief
      * Triggers the \ref VolumeRenderingApproach "volume rendering".
      */
    virtual void renderPass
        ( const base::math::Matrix4f& viewTransform
        , base::RenderTask& rt
        , const base::Viewport& vp ) override;

protected:

    /** \brief
      * Loads video resources when rendering is triggered for the first time.
      * Override this method if you need any additional resources to be loaded, but
      * always call the base implementation.
      *
      * \return the first texture unit to use for volume textures. The dafault
      *     implementation returns \ref base::Texture<0>::SETUP_UNIT `+1`.
      */
    virtual unsigned int loadVideoResources();

    virtual void render( const base::Renderable& ) override;

    /** \brief
      * Creates \ref base::Sampler "texture samplers" for volume textures and uses
      * \a registerSampler to assign them to the roles that they should be used with.
      */
    virtual void createVolumeSamplers( const std::function< void( unsigned int, base::Sampler* ) >& registerSampler ) = 0;

    /** \brief
      * Acquires the shader from the \ref base::ShaderManager, that is to be used for
      * \ref VolumeRenderingAlgorithm "rendering the slices".
      */
    virtual const base::ShaderProgram& acquireShader() = 0;

    /** \brief
      * Tells the name of the uniform variable, that the \a role texture is to be bound to.
      * Use \ref configureShader for custom shader configuration that goes beyond that.
      */
    virtual const std::string& uniformName( unsigned int role ) const = 0;

    /** \brief
      * Performs custom shader configuration on a per-pass level.
      */
    virtual void configureShader() = 0;
    
    /** \brief
      * Performs custom shader configuration on a per-volume level.
      */
    virtual void configureShader( const base::Renderable& ) = 0;

}; // VolumeRenderingStage



}  // namespace Carna :: presets

}  // namespace Carna

#endif // VOLUMERENDERINGSTAGE_H_6014714286
