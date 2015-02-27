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

    const static unsigned int ROLE_DEFAULT_SHADER = 1;

}; // MeshRenderingStageBase



// ----------------------------------------------------------------------------------
// MeshRenderingStage
// ----------------------------------------------------------------------------------

template< int geometryType, typename RenderableCompare >
class MeshRenderingStage : public GeometryStage< RenderableCompare >, public MeshRenderingStageBase
{

public:

    const static int GEOMETRY_TYPE = geometryType;

    MeshRenderingStage();

protected:

    virtual void render( GLContext& glc, const Renderable& renderable ) override;

}; // MeshRenderingStage


template< int geometryType, typename RenderableCompare >
MeshRenderingStage< geometryType, RenderableCompare >::MeshRenderingStage()
    : GeometryStage< RenderableCompare >( GEOMETRY_TYPE )
{
}


template< int geometryType, typename RenderableCompare >
void MeshRenderingStage< geometryType, RenderableCompare >::render( GLContext& glc, const Renderable& renderable )
{
    const ShaderControl& shaderControl = renderable.geometry().controlByRole< ShaderControl >( renderable );
    glc.setShader( shaderControl.shader() );

    const MeshControl& meshControl = renderable.geometry().controlByRole< MeshControl >( renderable );   
    meshControl.mesh().render();
}



// ----------------------------------------------------------------------------------
// OpaqueRenderingStage
// ----------------------------------------------------------------------------------

typedef MeshRenderingStage< 0, Renderable::VideoResourcesOrder< MeshRenderingStageBase::ROLE_DEFAULT_SHADER > > OpaqueRenderingStage;



// ----------------------------------------------------------------------------------
// TransparentRenderingStage
// ----------------------------------------------------------------------------------

typedef MeshRenderingStage< 1, Renderable::DepthOrder< Renderable::ORDER_BACK_TO_FRONT > > TransparentRenderingStage;



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // MESHRENDERINGSTAGE_H_6014714286
