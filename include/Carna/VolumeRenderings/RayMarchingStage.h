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

#include <Carna/base/GeometryStage.h>
#include <Carna/Carna.h>
#include <map>

/** \file   RayMarchingStage.h
  * \brief  Defines \ref Carna::base::RayMarchingStage.
  */

namespace Carna
{

namespace VolumeRenderings
{



// ----------------------------------------------------------------------------------
// RayMarchingStage
// ----------------------------------------------------------------------------------

class CARNA_LIB RayMarchingStage
    : public base::GeometryStage< base::Renderable::DepthOrder< base::Renderable::ORDER_BACK_TO_FRONT > >
{

    struct Details;
    const std::unique_ptr< Details > pimpl;

    struct VideoResources;
    std::unique_ptr< VideoResources > vr;

public:

    const static unsigned int DEFAULT_SAMPLE_RATE = 100;

    RayMarchingStage( unsigned int geometryType );

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
        , base::RenderTask& rt
        , const base::Viewport& vp ) override;

protected:

    virtual void loadVideoResources();

    virtual void render( base::GLContext& glc, const base::Renderable& ) override;

    virtual void createSamplers( const std::function< void( unsigned int, base::Sampler* ) >& registerSampler ) = 0;

    /** \brief
      * Loads the ray marching shader.
      * This shader is applied during rendering of the volume slices.
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
    virtual void configureShader( base::GLContext& ) = 0;

}; // RayMarchingStage



}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // RAYMARCHINGSTAGE_H_6014714286
