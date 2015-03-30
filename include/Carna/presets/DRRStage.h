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

#ifndef DRRSTAGE_H_6014714286
#define DRRSTAGE_H_6014714286

#include <Carna/presets/VolumeRenderingStage.h>
#include <Carna/Carna.h>
#include <memory>

/** \file   DRRStage.h
  * \brief  Defines \ref Carna::presets::DRRStage.
  */

namespace Carna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// DRRStage
// ----------------------------------------------------------------------------------

/** \brief
  * Renders digital radiograph reconstructs of volume geometries in the scene.
  *
  * \section DRRStageBackground Background
  *
  * For each pixel of the output framebuffer, a ray is casted into the scene. This
  * ray occasionally intersects volumetric data, that is assigned to geometry nodes.
  * This ray defines a one-dimensional space. We denote its coordinates with \f$x\f$.
  * Further, we write \f$\mathrm{hu}\left(x\right)\f$ to denote the HU value at
  * location \f$x\f$.
  *
  * This rendering stage computes
  * \f[
  * I = I_o \exp\left( -\int_{-\infty}^{\infty} g\left(x\right) \mu\left(x\right) \,\mathrm dx \right)
  * \f]
  * with
  * \f[
  * \mu\left(x\right) = \mu_\text{water} \cdot \left(1+\frac{\mathrm{hu}\left(x\right)}{1000}\right)\text.
  * \f]
  *
  * The function \f$g\left(x\right)\f$ yields some non-linearity, where \f$\lambda\f$
  * is an arbitrary positive factor:
  * \f[
  * g\left(x\right) =
  * \begin{cases}
  *     0       & \text{if} & \mathrm{hu}_\text{lower} > \mathrm{hu}\left(x\right) \\
  *     1       & \text{if} & \mathrm{hu}_\text{lower} \geq \mathrm{hu}\left(x\right) < \mathrm{hu}_\text{upper} \\
  *     \lambda & \text{if} & \mathrm{hu}_\text{upper} \leq \mathrm{hu}\left(x\right)
  * \end{cases}
  * \f]
  *
  * The integral
  * \f$y = \int_{-\infty}^{\infty} g\left(x\right) \mu\left(x\right) \,\mathrm dx\f$
  * is computed through \ref \VolumeRenderingStage::renderPass. For each pixel, the
  * result is written to a texture. Afterwards this texture is drawn back to the
  * output buffer, using a shader the computes \f$I = I_0 \exp\left(-y\right)\f$.
  * The resulting colors are defined as
  * \f$I \mapsto \left(1, 1, 1, 1 - I\right)^\mathrm T\f$ if
  * \ref setRenderingInverse "inverse rendering" is disabled, or
  * \f$I \mapsto \left(0, 0, 0, 1 - I\right)^\mathrm T\f$ otherwise. The fourth
  * component of the color vector is the opacity.
  *
  * \section DRRStageUsage Usage
  *
  * The `%DRRStage` constructor takes a geometry type parameter:
  *
  * \snippet ModuleTests/DRRStageTest.cpp drr_instantiation
  *
  * The concept of geometry types is explained \ref GeometryTypes "here".
  *
  * \note
  * In the \ref RenderingProcess "rendering process" this stage will usually be
  * inserted \em after such stages that render opaque geometry, like
  * \ref CuttingPlanesStage and \ref OpaqueRenderingStage.
  *
  * The following example code configures the `%DRRStage` s.t. it produces the
  * rendering presented further below:
  *
  * \snippet ModuleTests/DRRStageTest.cpp drr_setup_base
  *
  * The last step is to state that the brightness of any particular pixel shall be
  * *inverse* proportional to the ray attenuation at that pixel:
  *
  * \snippet ModuleTests/DRRStageTest.cpp drr_setup_inverse
  *
  * \image html DRRStageTest/inverted.png "exemplary rendering in inverse-mode from code above"
  *
  * \author Leonid Kostrykin
  * \date   22.2.15 - 11.3.15
  */
class CARNA_LIB DRRStage : public VolumeRenderingStage
{

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    const static float      DEFAULT_WATER_ATTENUATION;  ///< Holds default value for \f$\mu_\text{water}\f$.
    const static float      DEFAULT_BASE_INTENSITY;     ///< Holds default value for \f$I_0\f$.
    const static base::HUV  DEFAULT_LOWER_THRESHOLD;    ///< Holds default value for \f$\mathrm{hu}_\text{lower}\f$.
    const static base::HUV  DEFAULT_UPPER_THRESHOLD;    ///< Holds default value for \f$\mathrm{hu}_\text{upper}\f$.
    const static float      DEFAULT_UPPER_MULTIPLIER;   ///< Holds default value for \f$\lambda\f$.
    const static bool       DEFAULT_RENDER_INVERSE;     ///< Holds default value for \ref isRenderingInverse "inverse rendering".

    /** \brief
      * Holds the \ref GeometryFeatures "role" that HU volume data is expected to
      * take when attached to \ref base::Geometry nodes.
      */
    const static unsigned int ROLE_HU_VOLUME = 0;
    
    /** \brief
      * Instantiates. The created stage will render such \ref base::Geometry scene
      * graph nodes, whose \ref GeometryTypes "geometry types" equal \a geometryType.
      */
    DRRStage( unsigned int geometryType );

    /** \brief
      * Deletes.
      */
    virtual ~DRRStage();

    virtual void reshape( const base::FrameRenderer& fr, unsigned int width, unsigned int height ) override;

    /** \brief
      * Computes the digital radiograph reconstruct like described
      * \ref DRRStageBackground "here".
      */
    virtual void renderPass
        ( const base::math::Matrix4f& viewTransform
        , base::RenderTask& rt
        , const base::Viewport& vp ) override;

    /** \brief
      * Tells \f$\mu_\text{water}\f$. The parameters are described
      * \ref DRRStageBackground "here".
      */
    float waterAttenuation() const;

    /** \brief
      * Tells \f$I_0\f$. The parameters are described \ref DRRStageBackground "here".
      */
    float baseIntensity() const;
    
    /** \brief
      * Tells \f$\mathrm{hu}_\text{lower}\f$. The parameters are described
      * \ref DRRStageBackground "here".
      */
    base::HUV lowerThreshold() const;
    
    /** \brief
      * Tells \f$\mathrm{hu}_\text{upper}\f$. The parameters are described
      * \ref DRRStageBackground "here".
      */
    base::HUV upperThreshold() const;
    
    /** \brief
      * Tells \f$\lambda\f$. The parameters are described
      * \ref DRRStageBackground "here".
      */
    float upperMultiplier() const;

    /** \brief
      * Tells whether ray attenuation at any particular pixel is *inverse*
      * proportional to the brightness of that pixel or just proportional.
      */
    bool isRenderingInverse() const;
    
    /** \brief
      * Sets \f$\mu_\text{water}\f$ to \a muWater. The parameters are described
      * \ref DRRStageBackground "here".
      */
    void setWaterAttenuation( float muWater );
    
    /** \brief
      * Sets \f$I_0\f$ to \a baseIntensity. The parameters are described
      * \ref DRRStageBackground "here".
      */
    void setBaseIntensity( float baseIntensity );
    
    /** \brief
      * Sets \f$\mathrm{hu}_\text{lower}\f$ to \a lower. The parameters are described
      * \ref DRRStageBackground "here".
      */
    void setLowerThreshold( base::HUV lower );
    
    /** \brief
      * Sets \f$\mathrm{hu}_\text{upper}\f$ to \a upper. The parameters are described
      * \ref DRRStageBackground "here".
      */
    void setUpperThreshold( base::HUV upper );
    
    /** \brief
      * Sets \f$\lambda\f$ to \a multiplier. The parameters are described
      * \ref DRRStageBackground "here".
      */
    void setUpperMultiplier( float multiplier );

    /** \brief
      * Sets whether the colors of the DRR should be \a inverse or not. The ray
      * attenuation at a particular pixel is proportional to the brightness of that
      * pixel if \a inverse is `false`.
      *
      * If you set \a inverse to `true`, you will also have to change the
      * \ref base::FrameRenderer::setBackgroundColor "rendering's background color"
      * from default \ref base::Color::BLACK_NO_ALPHA "black" to something brighter,
      * like \ref base::Color::WHITE_NO_ALPHA "white". Otherwise you might not see
      * any rendering results from this stage. Default value is `false`.
      */
    void setRenderingInverse( bool inverse );

protected:

    virtual unsigned int loadVideoResources() override;

    virtual void createVolumeSamplers( const std::function< void( unsigned int, base::Sampler* ) >& registerSampler ) override;

    /** \brief
      * Acquires the `drr_accumulation` shader from the \ref base::ShaderManager.
      */
    virtual const base::ShaderProgram& acquireShader() override;

    /** \brief
      * Maps \ref ROLE_HU_VOLUME to `huVolume`.
      */
    virtual const std::string& uniformName( unsigned int role ) const override;

    /** \brief
      * Uploads the \ref DRRStageBackground "parameters" to the shader.
      */
    virtual void configureShader() override;

    /** \brief
      * Does nothing.
      */
    virtual void configureShader( const base::Renderable& ) override;

}; // DRRStage



}  // namespace Carna :: presets

}  // namespace Carna

#endif // DRRSTAGE_H_6014714286
