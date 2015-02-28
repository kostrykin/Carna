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

    MeshRenderingStage( int geometryType );

    virtual void renderPass( const math::Matrix4f& viewTransform, RenderTask& rt, const Viewport& vp ) override;

protected:

    virtual void render( GLContext& glc, const Renderable& renderable ) override;

}; // MeshRenderingStage


template< typename RenderableCompare >
MeshRenderingStage< RenderableCompare >::MeshRenderingStage( int geometryType )
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
    const Material& material = renderable.geometry().aggregateByRole< Material >( renderable );
    material.activate( rs, glc );

    ShaderProgram::putUniform4x4f( "modelView", renderable.modelViewTransform() );
    ShaderProgram::putUniform4x4f( "projection", renderTask->projection );
    ShaderProgram::putUniform4x4f( "modelViewProjection", renderTask->projection * renderable.modelViewTransform() );

    const MeshManager& meshControl = renderable.geometry().aggregateByRole< MeshManager >( renderable );   
    meshControl.mesh().render();
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
