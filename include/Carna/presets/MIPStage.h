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

#ifndef MIPSTAGE_H_6014714286
#define MIPSTAGE_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/ColorMap.h>
#include <Carna/presets/VolumeRenderingStage.h>
#include <memory>

/** \file   MIPStage.h
  * \brief  Defines \ref Carna::presets::MIPStage.
  */

namespace Carna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// MIPStage
// ----------------------------------------------------------------------------------

/** \brief
  * Renders maximum intensity projections of volume geometries in the scene.
  *
  * \section MIPStageBasics Basics
  *
  * The `%MIPStage` constructor takes a geometry type parameter:
  *
  * \snippet ModuleTests/MIPStageTest.cpp mip_instantiation
  *
  * \image html MIPStageTest/writeLinearSegment.png "exemplary rendering with a linearly transitioning blue-to-red color map"
  *
  * The concept of geometry types is explained \ref GeometryTypes "here".
  *
  * \note
  * In the \ref RenderingProcess "rendering process" this stage will usually be
  * inserted \em after such stages that render opaque geometry, like
  * \ref CuttingPlanesStage and \ref OpaqueRenderingStage.
  *
  * More complex color maps can easily be generated using the \ref base::ColorMap::writeLinearSpline function:
  *
  * \snippet ModuleTests/MIPStageTest.cpp mip_jet
  *
  * \image html MIPStageTest/writeLinearSpline.png "exemplary rendering with a \"jet\"-like color map"
  *
  * \author Leonid Kostrykin
  * \date   22.2.15 - 30.4.25
  */
class CARNA_LIB MIPStage : public VolumeRenderingStage
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
      * Instantiates.
      */
    explicit MIPStage( unsigned int geometryType, unsigned int colorMapResolution = base::ColorMap::DEFAULT_RESOLUTION );

    /** \brief
      * Deletes.
      */
    virtual ~MIPStage();

    /** \brief
      * The color map used for the layer.
      */
    base::ColorMap colorMap;

    virtual void reshape( base::FrameRenderer& fr, unsigned int width, unsigned int height ) override;

    virtual void renderPass
        ( const base::math::Matrix4f& viewTransform
        , base::RenderTask& rt
        , const base::Viewport& vp ) override;

protected:

    virtual unsigned int loadVideoResources() override;

    virtual void createVolumeSamplers( const std::function< void( unsigned int, base::Sampler* ) >& registerSampler ) override;

    virtual const base::ShaderProgram& acquireShader() override;

    virtual const std::string& uniformName( unsigned int role ) const override;

    /** \brief
      * Does nothing.
      */
    virtual void configureShader() override;

    /** \brief
      * Does nothing.
      */
    virtual void configureShader( const base::Renderable& ) override;

}; // MIPStage



}  // namespace Carna :: presets

}  // namespace Carna

#endif // MIPSTAGE_H_6014714286
