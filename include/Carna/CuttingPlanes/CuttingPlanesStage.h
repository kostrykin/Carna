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

#include <Carna/base/view/GeometryStage.h>
#include <Carna/base/view/Renderable.h>
#include <Carna/Carna.h>

/** \file   CuttingPlanesStage.h
  * \brief  Defines \ref Carna::base::view::CuttingPlanesStage.
  */

namespace Carna
{

namespace CuttingPlanes
{



// ----------------------------------------------------------------------------------
// CuttingPlanesStage
// ----------------------------------------------------------------------------------

class CARNA_LIB CuttingPlanesStage : public base::view::GeometryStage< void >
{

    struct Details;
    const std::unique_ptr< Details > pimpl;

    struct VideoResources;
    std::unique_ptr< VideoResources > vr;

public:

    CuttingPlanesStage( int volumeGeometryType, int planeGeometryType );

    virtual ~CuttingPlanesStage();

    virtual void prepareFrame( base::view::Node& root ) override;

    virtual void renderPass
        ( const base::math::Matrix4f& viewTransform
        , base::view::RenderTask& rt
        , const base::view::Viewport& vp ) override;

    void setWindowingLevel( base::HUV windowingLevel );

    void setWindowingWidth( unsigned int windowingWidth );

    base::HUV windowingLevel() const;

    unsigned int windowingWidth() const;

protected:

    virtual void render( base::view::GLContext& glc, const base::view::Renderable& ) override;

}; // CuttingPlanesStage



}  // namespace Carna :: CuttingPlanes

}  // namespace Carna

#endif // CUTTINGPLANESSTAGE_H_6014714286
