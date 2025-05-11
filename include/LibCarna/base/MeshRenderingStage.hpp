/*
 *  Copyright (C) 2010 - 2016 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 * 
 * 
 *  Copyright (C) 2021 - 2025 Leonid Kostrykin
 * 
 */

#ifndef MESHRENDERINGSTAGE_H_6014714286
#define MESHRENDERINGSTAGE_H_6014714286

#include <LibCarna/base/GeometryStage.hpp>
#include <LibCarna/base/Renderable.hpp>
#include <LibCarna/base/RenderState.hpp>
#include <LibCarna/base/RenderTask.hpp>
#include <LibCarna/base/ShaderUniform.hpp>
#include <LibCarna/base/Material.hpp>
#include <LibCarna/base/ManagedMesh.hpp>

/** \file
  * \brief
  * Defines \ref LibCarna::base::MeshRenderingStage.
  */

namespace LibCarna
{

namespace base
{



// ----------------------------------------------------------------------------------
// MeshRenderingMixin
// ----------------------------------------------------------------------------------

/** \brief
  * Defines \ref MeshRenderingStage class template instance invariants.
  *
  * \author Leonid Kostrykin
  */
class LIBCARNA MeshRenderingMixin
{

public:

    /** \brief
      * Instantiates.
      *
      * \param geometryType
      *     is the \ref QuickStart_FrameRenderer "geometry type" rendered by this
      *     \ref MeshRenderingStage.
      */
     MeshRenderingMixin( unsigned int geometryType );

    /** \brief
      * Does nothing.
      */   
    virtual ~MeshRenderingMixin();

    /** \brief
      * Identifies the \ref Mesh object that \ref MeshRenderingStage uses for
      * rendering.
      */
    const static unsigned int DEFAULT_ROLE_MESH = 0;
    
    /** \brief
      * Identifies the \ref Material object that \ref MeshRenderingStage uses for
      * rendering.
      */
    const static unsigned int DEFAULT_ROLE_MATERIAL = 1;

    /** \brief
      * Holds the \ref QuickStart_FrameRenderer "geometry type" rendered by this
      * \ref MeshRenderingStage.
      */
    const unsigned int geometryType;

}; // MeshRenderingMixin



// ----------------------------------------------------------------------------------
// MeshRenderingStage
// ----------------------------------------------------------------------------------

/** \brief
  * Implements a \ref GeometryStage "geometry rendering stage" that renders
  * \ref Mesh "meshes".
  *
  * \see
  * Refer to the documentation of the \ref RenderingProcess "rendering process" for
  * further notes on how rendering stages operate.
  *
  * An instance of this rendering stage expects two features from the \ref Geometry
  * objects it processes:
  *
  * - \ref DEFAULT_ROLE_MESH must be a \ref Mesh object.
  * - \ref DEFAULT_ROLE_MATERIAL must be a \ref Material object.
  *
  * \author Leonid Kostrykin
  */
template< typename RenderableCompare >
class MeshRenderingStage : public GeometryStage< RenderableCompare >, public MeshRenderingMixin
{

    RenderTask* renderTask;

public:

    /** \brief
      * Instantiates.
      *
      * \param geometryType
      *     is the \ref GeometryTypes "geometry type" rendered by this stage.
      */
    MeshRenderingStage( unsigned int geometryType );

    virtual void renderPass( const math::Matrix4f& viewTransform, RenderTask& rt, const Viewport& vp ) override;

protected:

    /** \copydoc LibCarna::base::GeometryStage::render
      */
    virtual void render( const Renderable& renderable ) override;

}; // MeshRenderingStage


template< typename RenderableCompare >
MeshRenderingStage< RenderableCompare >::MeshRenderingStage( unsigned int geometryType )
    : GeometryStage< RenderableCompare >( geometryType )
    , MeshRenderingMixin( geometryType )
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
void MeshRenderingStage< RenderableCompare >::render( const Renderable& renderable )
{
    RenderState rs;
    const Material& material = static_cast< Material& >( renderable.geometry().feature( DEFAULT_ROLE_MATERIAL ) );
    this->videoResource( material ).activate( rs );

    ShaderUniform< math::Matrix4f >( "modelView", renderable.modelViewTransform() ).upload();
    ShaderUniform< math::Matrix4f >( "projection", renderTask->projection ).upload();
    ShaderUniform< math::Matrix4f >( "modelViewProjection", renderTask->projection * renderable.modelViewTransform() ).upload();
    ShaderUniform< math::Matrix4f >( "normalsView", renderable.viewModelTransform().transpose() ).upload();

    const ManagedMeshBase& mesh = static_cast< ManagedMeshBase& >( renderable.geometry().feature( DEFAULT_ROLE_MESH ) );
    this->videoResource( mesh ).get().render();
}



}  // namespace LibCarna :: base

}  // namespace LibCarna

#endif // MESHRENDERINGSTAGE_H_6014714286
