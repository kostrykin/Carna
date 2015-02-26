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

#ifndef RAYMARCHINGSTAGE_H_6014714286
#define RAYMARCHINGSTAGE_H_6014714286

#include <Carna/base/view/GeometryStage.h>
#include <Carna/Carna.h>
#include <map>

/** \file   RayMarchingStage.h
  * \brief  Defines \ref Carna::base::view::RayMarchingStage.
  */

namespace Carna
{

namespace VolumeRenderings
{



// ----------------------------------------------------------------------------------
// RayMarchingStage
// ----------------------------------------------------------------------------------

class CARNA_LIB RayMarchingStage : public base::view::GeometryStage< base::view::Renderable::DepthOrder< base::view::Renderable::ORDER_BACK_TO_FRONT > >
{

    struct Details;
    const std::unique_ptr< Details > pimpl;

    struct VideoResources;
    std::unique_ptr< VideoResources > vr;

public:

    const static int GEOMETRY_TYPE = 2;

    const static unsigned int DEFAULT_SAMPLE_RATE = 100;

    RayMarchingStage();

    virtual ~RayMarchingStage();

    /** \brief
      * Sets number of samples per pixel and per segment.
      */
    void setSampleRate( unsigned int sampleRate );
    
    /** \brief
      * Tells number of samples per pixel and per segment.
      */
    unsigned int sampleRate() const;

    virtual void renderPass
        ( const base::math::Matrix4f& viewTransform
        , base::view::RenderTask& rt
        , const base::view::Viewport& vp ) override;

protected:

    virtual void render( const base::view::Renderable& ) override;

    virtual void createSamplers( const std::function< void( unsigned int, base::view::Sampler* ) >& registerSampler ) = 0;

    virtual const base::view::ShaderProgram& loadShader() = 0;

    virtual const std::string& uniformName( unsigned int role ) const = 0;

    virtual void configureShader( base::view::GLContext& ) = 0;

}; // RayMarchingStage



}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // RAYMARCHINGSTAGE_H_6014714286
