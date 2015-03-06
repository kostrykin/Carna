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

#ifndef CUTTINGPLANESSTAGE_H_6014714286
#define CUTTINGPLANESSTAGE_H_6014714286

#include <Carna/base/GeometryStage.h>
#include <Carna/base/Renderable.h>
#include <Carna/Carna.h>

/** \file   CuttingPlanesStage.h
  * \brief  Defines \ref Carna::base::CuttingPlanesStage.
  */

namespace Carna
{

namespace CuttingPlanes
{



// ----------------------------------------------------------------------------------
// CuttingPlanesStage
// ----------------------------------------------------------------------------------

class CARNA_LIB CuttingPlanesStage : public base::GeometryStage< void >
{

    struct Details;
    const std::unique_ptr< Details > pimpl;

    struct VideoResources;
    std::unique_ptr< VideoResources > vr;

public:

    const static unsigned int DEFAULT_WINDOWING_WIDTH = 4096;
    const static unsigned int DEFAULT_WINDOWING_LEVEL = static_cast< int >( DEFAULT_WINDOWING_WIDTH / 2 ) - 1024;
    const static unsigned int ROLE_HU_VOLUME = 0;

    CuttingPlanesStage( int volumeGeometryType, int planeGeometryType );

    virtual ~CuttingPlanesStage();

    virtual void renderPass
        ( const base::math::Matrix4f& viewTransform
        , base::RenderTask& rt
        , const base::Viewport& vp ) override;

    void setWindowingLevel( base::HUV windowingLevel );

    void setWindowingWidth( unsigned int windowingWidth );

    base::HUV windowingLevel() const;

    unsigned int windowingWidth() const;

    base::HUV minimumHUV() const;

    base::HUV maximumHUV() const;

protected:

    virtual void updateRenderQueues
        ( base::Node& root
        , const base::math::Matrix4f& viewTransform
        , bool viewTransformTriggered ) override;

    virtual void render( base::GLContext& glc, const base::Renderable& ) override;

}; // CuttingPlanesStage



}  // namespace Carna :: CuttingPlanes

}  // namespace Carna

#endif // CUTTINGPLANESSTAGE_H_6014714286
