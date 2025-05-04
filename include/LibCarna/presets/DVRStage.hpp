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

#ifndef DVRSTAGE_H_6014714286
#define DVRSTAGE_H_6014714286

#include <LibCarna/LibCarna.hpp>
#include <LibCarna/base/ColorMap.hpp>
#include <LibCarna/presets/VolumeRenderingStage.hpp>
#include <memory>

/** \file
  * \brief
  * Defines \ref LibCarna::presets::DVRStage.
  */

namespace LibCarna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// DVRStage
// ----------------------------------------------------------------------------------

/** \brief
  * Performs *direct volume renderings* of the volume geometries in the scene.
  *
  * \section DVRStageUsage Usage
  *
  * The `%DVRStage` constructor takes a geometry type parameter:
  *
  * \snippet ModuleTests/DVRStageTest.cpp dvr_instantiation
  *
  * The concept of geometry types is explained \ref GeometryTypes "here".
  *
  * \note
  * In the \ref RenderingProcess "rendering process" this stage will usually be
  * inserted \em after such stages that render opaque geometry, like
  * \ref CuttingPlanesStage and \ref OpaqueRenderingStage.
  *
  * The `%DVRStage` supports two types of rendering modes: Rendering with lighting
  * enabled and rendering without lighting. In general, lighting produces more
  * sculptural results, but it comes at the cost of additional requirements: Not only
  * the intensity volume must be uploaded to the GPU, but also another 3D texture
  * that maps \ref VolumeRenderingModelTexture "texture space coordinates" to the
  * normal vectors of the surface at a particular location. Depending on the
  * resolution of the data, this might cause noteworthy memory consumption.
  *
  * Luckily the \ref helpers::VolumeGridHelper class does everything that is needed
  * in order to compute the normal vectors and upload them to the GPU, if it is
  * instructed to do so: Its second type argument is optional and set to `void` by
  * default. This disables its normal vectors support. Setting it to anything
  * different, like \ref base::NormalMap3DInt8 for example, makes it compute the
  * normal vectors when \ref helpers::VolumeGridHelper::loadHUData or its
  * `computeNormals` method are called.
  *
  * The `%DVRStage` decides which mode to use depending on whether the normal vectors
  * are provided or not.
  *
  * \subsection DVRStageWithoutLighting Without Lighting
  *
  * The following example code configures the `%DVRStage` s.t. it produces the
  * rendering presented further below:
  *
  * \snippet ModuleTests/DVRStageTest.cpp dvr_setup_without_lighting
  *
  * This gives the following rendering:
  * \image html DVRStageTest/withoutLighting.png "exemplary rendering without lighting from code above"
  *
  * \subsection DVRStageTranslucence Translucence
  *
  * The \ref setTranslucence "translucence" scales the opacity values queried from
  * the color map. Assuming that \f$\alpha\f$ was the value from the color map, the
  * translucence \f$t \geq 0\f$ scales the actual opacity to
  * \f$\frac{\alpha}{1 + t}\f$.
  *
  * \subsection DVRStageWithLighting With Lighting Enabled
  *
  * The \ref DEFAULT_TRANSLUCENCE "default translucence" is quite high, which is fine
  * for rendering without lighting. You might consider setting it down when
  * rendering with lighting enabled:
  *
  * \snippet ModuleTests/DVRStageTest.cpp dvr_setup_with_lighting
  *
  * This gives the following rendering:
  * \image html DVRStageTest/withLighting.png "exemplary rendering with lighting from code above"
  *
  * \author Leonid Kostrykin
  */
class LIBCARNA DVRStage : public VolumeRenderingStage
{

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    /** \brief
      * Holds the \ref GeometryFeatures "role" that intensity volume data is expected
      * to take when attached to \ref base::Geometry nodes.
      */
    const static unsigned int ROLE_INTENSITIES = 0;
    
    /** \brief
      * Holds the \ref GeometryFeatures "role" that the normal map is expected to
      * take when attached to \ref base::Geometry nodes.
      */
    const static unsigned int ROLE_NORMALS = 1;
    
    /** \brief
      * Holds the default \ref DVRStageTranslucence "translucence".
      */
    const static float DEFAULT_TRANSLUCENCE;
    
    /** \brief
      * Holds the default diffuse light amount. The ambient light amount is always
      * one minus the diffuse light amount.
      */
    const static float DEFAULT_DIFFUSE_LIGHT;
    
    /** \brief
      * Instantiates. The created stage will render such \ref base::Geometry scene
      * graph nodes, whose \ref GeometryTypes "geometry types" equal \a geometryType.
      *
      * The parameter \a colorMapResolution determines the resolution of the color
      * map. If your data is 8bit, using 8bit color map is sufficient. If your data
      * is 32bit, you probably also want to use a 32bit color map.
      */
    explicit DVRStage
        ( unsigned int geometryType
        , unsigned int colorMapResolution = base::ColorMap::DEFAULT_RESOLUTION );

    /** \brief
      * Deletes.
      */
    virtual ~DVRStage();

    virtual void reshape( base::FrameRenderer& fr, unsigned int width, unsigned int height ) override;

    virtual void renderPass
        ( const base::math::Matrix4f& viewTransform
        , base::RenderTask& rt
        , const base::Viewport& vp ) override;

    /** \brief
      * The color map used for the rendering.
      */
    base::ColorMap colorMap;
    
    /** \brief
      * Sets the \ref DVRStageTranslucence "translucence" property.
      */
    void setTranslucence( float translucence );
    
    /** \brief
      * Tells current \ref DVRStageTranslucence "translucence".
      */
    float translucence() const;
    
    /** \brief
      * Sets the diffuse light amount to \a diffuseLight and the ambient light amount
      * to one minus \a diffuseLight.
      *
      * \pre `diffuseLight >= 0 && diffuseLight <= 1`
      *
      * This only has an effect if lighting is enabled. If lighting is enabled,
      * setting this to \f$0\f$ virtually disables the lighting.
      */
    void setDiffuseLight( float diffuseLight );
    
    /** \brief
      * Tells the diffuse light amount. The ambient light amount is one minus the
      * diffuse light amount.
      */
    float diffuseLight() const;
    
    /** \brief
      * Tells whether lighting was used during the last rendering. The return value
      * is undefined if nothing was rendered yet.
      */
    bool isLightingUsed() const;

protected:

    virtual unsigned int loadVideoResources() override;

    virtual void createVolumeSamplers( const std::function< void( unsigned int, base::Sampler* ) >& registerSampler ) override;

    /** \brief
      * Acquires the `dvr` shader from the \ref base::ShaderManager.
      */
    virtual const base::ShaderProgram& acquireShader() override;

    /** \brief
      * Maps \ref ROLE_INTENSITIES to `intensities` and \ref ROLE_NORMALS to `normalMap`.
      */
    virtual const std::string& uniformName( unsigned int role ) const override;

    virtual void configureShader() override;
    
    /** \brief
      * Computes and uploads the normals transformation matrix.
      */
    virtual void configureShader( const base::Renderable& ) override;

}; // presets :: DVRStage



}  // namespace LibCarna :: presets

}  // namespace LibCarna

#endif // DVRSTAGE_H_6014714286
