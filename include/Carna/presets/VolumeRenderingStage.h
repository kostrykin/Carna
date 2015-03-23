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

#ifndef VOLUMERENDERINGSTAGE_H_6014714286
#define VOLUMERENDERINGSTAGE_H_6014714286

#include <Carna/base/GeometryStage.h>
#include <Carna/base/Renderable.h>
#include <Carna/Carna.h>
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
  * A short introduction to shaders exists in the \ref CustomMaterials section.
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
  * \ref base::Texture3D::textureCoordinatesCorrection "obtains a matrix" that shifts
  * and scales the texture coordinates s.t. these edges are avoided. It creates the
  * illusion as if the texture values were actually mapped to the grid intersections.
  *
  * The matrix, that results from the concatenation of the translation and the
  * edges-correction matrix, is uploaded to the shader as an uniform named
  * `modelTexture` from type `mat4`.
  *
  * \subsection VolumeRenderingOrder Rendering
  *
  * \todo
  * Write here, why depth test/writing is used and why bounding volumes are important.
  *
  * \section VolumeRenderingImplementation Implementation
  *
  * \todo
  * Write here some notes on how to implement classes derived from this.
  *
  * \author Leonid Kostrykin
  * \date   22.2.15 - 23.3.15
  */
class CARNA_LIB VolumeRenderingStage : public base::GeometryStage< base::Renderable::BackToFront >
{

    struct Details;
    const std::unique_ptr< Details > pimpl;

    struct VideoResources;
    std::unique_ptr< VideoResources > vr;

public:

    /** \brief
      * Holds the default number of slices rendered per segment.
      */
    const static unsigned int DEFAULT_SAMPLE_RATE = 100;

    VolumeRenderingStage( unsigned int geometryType );

    virtual ~VolumeRenderingStage();

    /** \brief
      * Sets number of slices to be rendered per segment.
      */
    void setSampleRate( unsigned int sampleRate );
    
    /** \brief
      * Tells number of slices to be rendered per segment.
      */
    unsigned int sampleRate() const;

    virtual void renderPass
        ( const base::math::Matrix4f& viewTransform
        , base::RenderTask& rt
        , const base::Viewport& vp ) override;

protected:

    virtual void loadVideoResources();

    virtual void render( const base::Renderable& ) override;

    virtual void createSamplers( const std::function< void( unsigned int, base::Sampler* ) >& registerSampler ) = 0;

    /** \brief
      * Loads the shader used for
      * \ref VolumeRenderingAlgorithm "rendering the slices".
      */
    virtual const base::ShaderProgram& loadShader() = 0;

    /** \brief
      * Tells the name of the uniform variable, that the \a role texture is to be bound to.
      * Use \ref configureShader for custom shader configuration that goes beyond that.
      */
    virtual const std::string& uniformName( unsigned int role ) const = 0;

    /** \brief
      * Performs custom shader configuration that goes beyond the default setup.
      */
    virtual void configureShader() = 0;

}; // VolumeRenderingStage



}  // namespace Carna :: presets

}  // namespace Carna

#endif // VOLUMERENDERINGSTAGE_H_6014714286
