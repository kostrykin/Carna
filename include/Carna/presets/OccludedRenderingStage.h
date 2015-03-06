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

class CARNA_LIB OccludedRenderingStage : public base::RenderStage
{

    struct Details;
    const std::unique_ptr< Details > pimpl;

    struct VideoResources;
    std::unique_ptr< VideoResources > vr;

public:

    OccludedRenderingStage();

    virtual ~OccludedRenderingStage();

    virtual void reshape( const base::FrameRenderer& fr, const base::Viewport& vp ) override;

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
