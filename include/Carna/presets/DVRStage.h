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

#ifndef DVRSTAGE_H_6014714286
#define DVRSTAGE_H_6014714286

#include <Carna/presets/VolumeRenderingStage.h>
#include <Carna/Carna.h>
#include <memory>

/** \file   DVRStage.h
  * \brief  Defines \ref Carna::presets::DVRStage.
  */

namespace Carna
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
  * the HU volume must be uploaded to the GPU, but also another 3D texture that maps
  * \ref VolumeRenderingModelTexture "texture space coordinates" to the normal
  * vectors of the surface at a particular location. Depending on the resolution of
  * the data, this might cause noteworthy memory consumption.
  *
  * Luckily the \ref helpers::VolumeGridHelper class does everything that is needed
  * in order to compute the normal vectors and upload them to the GPU, if it is
  * instructed to do so: Its second type argument is optional and set to `void` by
  * default. This disables its normal vectors support. Setting it to anything
  * different, like \ref base::NormalMap3DInt8 for example, makes it compute the
  * normal vectors when \ref helpers::VolumeGridHelper::loadData or its
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
  * \date   25.3.15 - 29.3.15
  */
class CARNA_LIB DVRStage : public VolumeRenderingStage
{

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    /** \brief
      * Holds the \ref GeometryFeatures "role" that HU volume data is expected to
      * take when attached to \ref base::Geometry nodes.
      */
    const static unsigned int ROLE_HU_VOLUME = 0;
    
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
      * Instantiates. The created stage will render such \ref base::Geometry scene
      * graph nodes, whose \ref GeometryTypes "geometry types" equal \a geometryType.
      */
    DVRStage( unsigned int geometryType );

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
      * Clears the color map. All HU values are mapped to
      * \ref base::Color::BLACK_NO_ALPHA after calling this method.
      */
    void clearColorMap();
    
    /** \brief
      * Maps all HU values from \a huRange to \a colorRange. The first/last HU value
      * from \a huRange is mapped to the first/last value of \a colorRange,
      * respectively. The values are interpolated linearly in between.
      *
      * Nothing happens if the last HU value of \a huRange is *smaller* than the
      * first. If the first and the last HU values of \a huRange are equal, they are
      * mapped to the *mean* of the first and the last values from \a colorRange.
      */
    void writeColorMap( const base::math::Span< base::HUV >& huRange, const base::math::Span< base::Color > colorRange );
    
    /** \brief
      * Sets the \ref DVRStageTranslucence "translucence" property.
      */
    void setTranslucence( float translucence );
    
    /** \brief
      * Tells current \ref DVRStageTranslucence "translucence".
      */
    float translucence() const;
    
    /** \overload
      */
    void writeColorMap( base::HUV huFirst, base::HUV huLast, const base::Color& colorFirst, const base::Color& colorLast );

protected:

    virtual unsigned int loadVideoResources() override;

    virtual void createVolumeSamplers( const std::function< void( unsigned int, base::Sampler* ) >& registerSampler ) override;

    /** \brief
      * Acquires the `dvr` shader from the \ref base::ShaderManager.
      */
    virtual const base::ShaderProgram& acquireShader() override;

    /** \brief
      * Maps \ref ROLE_HU_VOLUME to `huVolume` and \ref ROLE_NORMALS to `normalMap`.
      */
    virtual const std::string& uniformName( unsigned int role ) const override;

    virtual void configureShader() override;
    
    /** \brief
      * Computes and uploads the normals transformation matrix.
      */
    virtual void configureShader( const base::Renderable& ) override;

}; // DVRStage



}  // namespace Carna :: presets

}  // namespace Carna

#endif // DVRSTAGE_H_6014714286
