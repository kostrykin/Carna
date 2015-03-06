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

#ifndef MESHRENDERINGSTAGE_H_6014714286
#define MESHRENDERINGSTAGE_H_6014714286

#include <Carna/base/GeometryStage.h>
#include <Carna/base/Renderable.h>
#include <Carna/base/RenderState.h>
#include <Carna/base/RenderTask.h>
#include <Carna/base/ShaderUniform.h>

/** \file   MeshRenderingStage.h
  * \brief  Defines \ref Carna::base::MeshRenderingStage.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// MeshRenderingStageBase
// ----------------------------------------------------------------------------------

class MeshRenderingStageBase
{

public:

    const static unsigned int ROLE_DEFAULT_MESH = 0;

    const static unsigned int ROLE_DEFAULT_MATERIAL = 1;

}; // MeshRenderingStageBase



// ----------------------------------------------------------------------------------
// MeshRenderingStage
// ----------------------------------------------------------------------------------

template< typename RenderableCompare >
class MeshRenderingStage : public GeometryStage< RenderableCompare >, public MeshRenderingStageBase
{

    RenderTask* renderTask;

public:

    MeshRenderingStage( unsigned int geometryType );

    virtual void renderPass( const math::Matrix4f& viewTransform, RenderTask& rt, const Viewport& vp ) override;

protected:

    virtual void render( GLContext& glc, const Renderable& renderable ) override;

}; // MeshRenderingStage


template< typename RenderableCompare >
MeshRenderingStage< RenderableCompare >::MeshRenderingStage( unsigned int geometryType )
    : GeometryStage< RenderableCompare >( geometryType )
{
}


template< typename RenderableCompare >
void MeshRenderingStage< RenderableCompare >::renderPass( const math::Matrix4f& viewTransform, RenderTask& rt, const Viewport& vp )
{
    renderTask = &rt;
    GeometryStage< RenderableCompare >::renderPass( viewTransform, rt, vp );
    renderTask = nullptr;
}


template< typename RenderableCompare >
void MeshRenderingStage< RenderableCompare >::render( GLContext& glc, const Renderable& renderable )
{
    RenderState rs( glc );
    const Material& material = static_cast< Material& >( renderable.geometry().feature( ROLE_DEFAULT_MATERIAL ) );
    material.activate( rs, glc );

    ShaderUniform< math::Matrix4f >( "modelView", renderable.modelViewTransform() ).upload();
    ShaderUniform< math::Matrix4f >( "projection", renderTask->projection ).upload();
    ShaderUniform< math::Matrix4f >( "modelViewProjection", renderTask->projection * renderable.modelViewTransform() ).upload();

    const MeshBase& mesh = static_cast< MeshBase& >( renderable.geometry().feature( ROLE_DEFAULT_MESH ) );
    mesh.render();
}



}  // namespace Carna :: base

}  // namespace Carna

#endif // MESHRENDERINGSTAGE_H_6014714286
