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

/** \brief
  * Implements a simple interface for mapping
  * \ref FrameCoordinates "frame coordinates" to \ref Mesh objects.
  *
  * Lets assume we have an \ref OpaqueRenderingStage defined, e.g.:
  *
  * \snippet ModuleTests/MeshColorCodingStageTest.cpp MeshColorCodingStage_Opaque_Setup
  *
  * The concept of geometry types is explained \ref QuickStart_FrameRenderer "here".
  *
  * Than we can instantiate the `%MeshColorCodingStage` like this:
  *
  * \snippet ModuleTests/MeshColorCodingStageTest.cpp MeshColorCodingStage_instantiation
  *
  * After rendering the frame, we can query the `%MeshColorCodingStage` object for
  * meshes at particular frame coordinates using its \ref pick method.
  *
  * \note
  * In the \ref RenderingProcess "rendering process" this stage can be inserted
  * *anywhere*. However, it is a good idea to insert it as close to the beginning of
  * the rendering stages sequence as possible, because this will prevent it from
  * being processed multiple times occasionally.
  *
  * \author Leonid Kostrykin
  * \date   22.2.15 - 20.3.15
  */
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

    virtual void reshape( const base::FrameRenderer& fr, unsigned int width, unsigned int height ) override;

    virtual void renderPass( const base::math::Matrix4f& viewTransform, base::RenderTask& rt, const base::Viewport& vp ) override;

    base::Aggregation< const base::Geometry > pick( unsigned int x, unsigned int y ) const;

    base::Aggregation< const base::Geometry > pick( const base::math::Vector2ui& ) const;

protected:

    virtual void render( const base::Renderable& renderable ) override;

}; // MeshColorCodingStage



}  // namespace Carna :: presets

}  // namespace Carna

#endif // MESHCOLORCODINGSTAGE_H_6014714286
