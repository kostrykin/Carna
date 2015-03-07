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
  * \brief  Defines \ref Carna::presets::MeshColorCodingStage.
  */

namespace Carna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// MeshColorCodingStage
// ----------------------------------------------------------------------------------

class CARNA_LIB MeshColorCodingStage : public base::GeometryStage< void >
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

    virtual void reshape( const base::FrameRenderer& fr, const base::Viewport& vp ) override;

    virtual void renderPass( const base::math::Matrix4f& viewTransform, base::RenderTask& rt, const base::Viewport& vp ) override;

    base::Aggregation< const base::Geometry > pick( unsigned int x, unsigned int y ) const;

    base::Aggregation< const base::Geometry > pick( const base::math::Vector2ui& ) const;

protected:

    virtual void render( base::GLContext& glc, const base::Renderable& renderable ) override;

}; // MeshColorCodingStage



}  // namespace Carna :: presets

}  // namespace Carna

#endif // MESHCOLORCODINGSTAGE_H_6014714286
