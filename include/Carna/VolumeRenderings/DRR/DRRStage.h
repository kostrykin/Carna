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

#include <Carna/VolumeRenderings/RayMarchingStage.h>
#include <Carna/Carna.h>
#include <memory>

/** \file   DRRStage.h
  * \brief  Defines \ref Carna::base::view::DRRStage.
  */

namespace Carna
{

namespace VolumeRenderings
{

namespace DRR
{



// ----------------------------------------------------------------------------------
// DRRStage
// ----------------------------------------------------------------------------------

class CARNA_LIB DRRStage : public RayMarchingStage
{

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    const static unsigned int ROLE_HU_VOLUME = 0;

    DRRStage( int geometryType );

    virtual ~DRRStage();

    virtual void reshape( const base::view::FrameRenderer& fr, const base::view::Viewport& vp ) override;

    virtual void renderPass
        ( const base::math::Matrix4f& viewTransform
        , base::view::RenderTask& rt
        , const base::view::Viewport& vp ) override;

    float waterAttenuation() const;

    float baseIntensity() const;

    base::HUV lowerThreshold() const;

    base::HUV upperThreshold() const;

    float upperMultiplier() const;

    bool isRenderingInverse() const;

    void setWaterAttenuation( float muWater );

    void setBaseIntensity( float baseIntensity );

    void setLowerThreshold( base::HUV lower );

    void setUpperThreshold( base::HUV upper );

    void setUpperMultiplier( float multiplier );

    void setRenderingInverse( bool inverse );

protected:

    virtual void loadVideoResources() override;

    virtual void createSamplers( const std::function< void( unsigned int, base::view::Sampler* ) >& registerSampler ) override;

    virtual const base::view::ShaderProgram& loadShader() override;

    virtual const std::string& uniformName( unsigned int role ) const override;

    virtual void configureShader( base::view::GLContext& ) override;

}; // DRRStage



}  // namespace Carna :: VolumeRenderings :: DRR

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // DRRStage_H_6014714286
