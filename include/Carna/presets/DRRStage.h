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

#ifndef DRRStage_H_6014714286
#define DRRStage_H_6014714286

#include <Carna/presets/RayMarchingStage.h>
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
  * \author Leonid Kostrykin
  * \date   22.2.15 - 11.3.15
  */
class CARNA_LIB DRRStage : public RayMarchingStage
{

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    const static unsigned int ROLE_HU_VOLUME = 0;

    DRRStage( unsigned int geometryType );

    virtual ~DRRStage();

    virtual void reshape( const base::FrameRenderer& fr, const base::Viewport& vp ) override;

    virtual void renderPass
        ( const base::math::Matrix4f& viewTransform
        , base::RenderTask& rt
        , const base::Viewport& vp ) override;

    float waterAttenuation() const;

    float baseIntensity() const;

    base::HUV lowerThreshold() const;

    base::HUV upperThreshold() const;

    float upperMultiplier() const;

    /** \brief
      * Tells whether ray attenuation at a particular pixel is \em inverse
      * proportional to the brightness of that pixel or just proportional.
      */
    bool isRenderingInverse() const;

    void setWaterAttenuation( float muWater );

    void setBaseIntensity( float baseIntensity );

    void setLowerThreshold( base::HUV lower );

    void setUpperThreshold( base::HUV upper );

    void setUpperMultiplier( float multiplier );

    /** \brief
      * Sets whether the colors of the DRR should be \a inverse or not. The ray
      * attenuation at a particular pixel is proportional to the brightness of that
      * pixel if \a inverse is \c false.
      *
      * \note
      *     If you set \a inverse to \c true, you will also have to change the
      *     \ref base::FrameRenderer::setBackgroundColor "rendering's background
      *     color" from default \ref base::Color::BLACK_NO_ALPHA "black" to something
      *     brighter, like \ref base::Color::WHITE_NO_ALPHA "white". Otherwise you
      *     might not see any rendering results from this stage.
      *
      * Default value is \c false.
      */
    void setRenderingInverse( bool inverse );

protected:

    virtual void loadVideoResources() override;

    virtual void createSamplers( const std::function< void( unsigned int, base::Sampler* ) >& registerSampler ) override;

    virtual const base::ShaderProgram& loadShader() override;

    virtual const std::string& uniformName( unsigned int role ) const override;

    virtual void configureShader( base::GLContext& ) override;

}; // DRRStage



}  // namespace Carna :: presets

}  // namespace Carna

#endif // DRRStage_H_6014714286
