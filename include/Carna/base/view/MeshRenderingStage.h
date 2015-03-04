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

#include <Carna/base/view/GeometryStage.h>
#include <Carna/base/view/Renderable.h>
#include <Carna/base/view/RenderState.h>
#include <Carna/base/view/RenderTask.h>
#include <Carna/base/view/ShaderUniform.h>

/** \file   MeshRenderingStage.h
  * \brief  Defines \ref Carna::base::view::MeshRenderingStage.
  */

namespace Carna
{

namespace base
{

namespace view
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



// ----------------------------------------------------------------------------------
// OpaqueRenderingStage
// ----------------------------------------------------------------------------------

typedef MeshRenderingStage< Renderable::VideoResourcesOrder< MeshRenderingStageBase::ROLE_DEFAULT_MATERIAL > > OpaqueRenderingStage;



// ----------------------------------------------------------------------------------
// TransparentRenderingStage
// ----------------------------------------------------------------------------------

typedef MeshRenderingStage< Renderable::DepthOrder< Renderable::ORDER_BACK_TO_FRONT > > TransparentRenderingStage;



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // MESHRENDERINGSTAGE_H_6014714286
