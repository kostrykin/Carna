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

#ifndef OCCLUDEDRENDERINGSTAGE_H_6014714286
#define OCCLUDEDRENDERINGSTAGE_H_6014714286

#include <Carna/base/RenderStage.h>

/** \file   OccludedRenderingStage.h
  * \brief  Defines \ref Carna::presets::OccludedRenderingStage.
  */

namespace Carna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// OccludedRenderingStage
// ----------------------------------------------------------------------------------

/** \brief
  * Renders geometry that is actually depth-occluded. The occluded geometry is
  * rendered with reduced opacity on-top of the geometry that it is occluded by.
  *
  * \note
  * In the \ref RenderingProcess "rendering process" this stage should be inserted
  * \em before the geometry that is to be rendered with reduced opacity when occluded
  * and \em after the geometry that the occluded geometry should be rendered on-top
  * of.
  *
  * Note that every geometry type that is to be rendered like described above when
  * occluded must be \ref enableStage "enabled" explicitly.
  *
  * The following exemplary setup of the rendering process draws opaque meshes, that
  * are occluded by cutting planes, on-top of them with reduced opacity:
  *
  * -# \ref CuttingPlanesStage
  * -# \ref OccludedRenderingStage with stage 3 enabled
  * -# \ref OpaqueRenderingStage
  *
  * \author Leonid Kostrykin
  * \date   21.2.15 - 6.3.15
  */
class CARNA_LIB OccludedRenderingStage : public base::RenderStage
{

    struct Details;
    const std::unique_ptr< Details > pimpl;

    struct VideoResources;
    std::unique_ptr< VideoResources > vr;

public:

    const static float DEFAULT_OCCLUSION_TRANSLUCENCY;

    OccludedRenderingStage();

    virtual ~OccludedRenderingStage();

    void disableAllStages();

    void enableStage( const base::RenderStage& );

    void disableStage( const base::RenderStage& );

    bool isStageEnabled( const base::RenderStage& ) const;

    void setOcclusionTranslucency( float );

    float occlusionTranslucency() const;

    virtual void reshape( const base::FrameRenderer& fr, unsigned int width, unsigned int height ) override;

    virtual bool isInitialized() const override;
    
    virtual void prepareFrame( base::Node& root ) override;

    virtual void renderPass
        ( const base::math::Matrix4f& viewTransform
        , base::RenderTask& rt
        , const base::Viewport& vp ) override;

}; // OccludedRenderingStage



}  // namespace Carna :: presets

}  // namespace Carna

#endif // OCCLUDEDRENDERINGSTAGE_H_6014714286
