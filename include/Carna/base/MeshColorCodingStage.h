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

#ifndef MESHCOLORCODINGSTAGE_H_6014714286
#define MESHCOLORCODINGSTAGE_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/GeometryStage.h>
#include <Carna/base/Aggregation.h>

/** \file   MeshColorCodingStage.h
  * \brief  Defines \ref Carna::ColorCoding::MeshColorCodingStage.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// MeshColorCodingStage
// ----------------------------------------------------------------------------------

class CARNA_LIB MeshColorCodingStage : public GeometryStage< void >
{

    struct Details;
    const std::unique_ptr< Details > pimpl;

    struct VideoResources;
    std::unique_ptr< VideoResources > vr;

public:

    MeshColorCodingStage();

    virtual ~MeshColorCodingStage();

    void setActivationPassIndex( unsigned int activationPassIndex );

    unsigned int activationPassIndex() const;

    void putGeometryType( unsigned int geometryType, unsigned int meshRole );

    void removeGeometryType( unsigned int geometryType );

    void clearGeometryTypes();

    virtual void reshape( const FrameRenderer& fr, const Viewport& vp ) override;

    virtual void renderPass( const math::Matrix4f& viewTransform, RenderTask& rt, const Viewport& vp ) override;

    Aggregation< const Geometry > pick( unsigned int x, unsigned int y ) const;

protected:

    virtual void render( GLContext& glc, const Renderable& renderable ) override;

}; // MeshColorCodingStage



}  // namespace Carna :: base

}  // namespace Carna

#endif // MESHCOLORCODINGSTAGE_H_6014714286
