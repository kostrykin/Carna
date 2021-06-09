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
#include <Carna/base/Material.h>
#include <Carna/base/Mesh.h>

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

/** \brief
  * Defines \ref MeshRenderingStage class template instance invariants.
  *
  * \author Leonid Kostrykin
  * \date   21.2.15 - 6.3.15
  */
class CARNA_LIB MeshRenderingStageBase
{

public:

    /** \brief
      * Instantiates.
      *
      * \param geometryType
      *     is the \ref QuickStart_FrameRenderer "geometry type" rendered by this
      *     \ref MeshRenderingStage.
      */
    MeshRenderingStageBase( unsigned int geometryType );

    /** \brief
      * Does nothing.
      */   
    virtual ~MeshRenderingStageBase();

    /** \brief
      * Identifies the \ref Mesh object that \ref MeshRenderingStage uses for
      * rendering.
      */
    const static unsigned int ROLE_DEFAULT_MESH = 0;
    
    /** \brief
      * Identifies the \ref Material object that \ref MeshRenderingStage uses for
      * rendering.
      */
    const static unsigned int ROLE_DEFAULT_MATERIAL = 1;

    /** \brief
      * Holds the \ref QuickStart_FrameRenderer "geometry type" rendered by this
      * \ref MeshRenderingStage.
      */
    const unsigned int geometryType;

}; // MeshRenderingStageBase



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
  * - \ref ROLE_DEFAULT_MESH must be a \ref Mesh object.
  * - \ref ROLE_DEFAULT_MATERIAL must be a \ref Material object.
  *
  * \author Leonid Kostrykin
  * \date   21.2.15 - 6.3.15
  */
template< typename RenderableCompare >
class MeshRenderingStage : public GeometryStage< RenderableCompare >, public MeshRenderingStageBase
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
    
    /** \copydoc Carna::base::RenderStage::clone
      */
    MeshRenderingStage* clone() const override;

    virtual void renderPass( const math::Matrix4f& viewTransform, RenderTask& rt, const Viewport& vp ) override;

protected:

    /** \copydoc Carna::base::GeometryStage::render
      */
    virtual void render( const Renderable& renderable ) override;

}; // MeshRenderingStage


template< typename RenderableCompare >
MeshRenderingStage< RenderableCompare >::MeshRenderingStage( unsigned int geometryType )
    : GeometryStage< RenderableCompare >( geometryType )
    , MeshRenderingStageBase( geometryType )
{
}


template< typename RenderableCompare >
MeshRenderingStage< RenderableCompare >* MeshRenderingStage< RenderableCompare >::clone() const
{
    typedef MeshRenderingStage< RenderableCompare > MyType;
    MyType* const result = new MyType( MeshRenderingStageBase::geometryType );
    result->setEnabled( this->isEnabled() );
    return result;
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
    const Material& material = static_cast< Material& >( renderable.geometry().feature( ROLE_DEFAULT_MATERIAL ) );
    this->videoResource( material ).activate( rs );

    ShaderUniform< math::Matrix4f >( "modelView", renderable.modelViewTransform() ).upload();
    ShaderUniform< math::Matrix4f >( "projection", renderTask->projection ).upload();
    ShaderUniform< math::Matrix4f >( "modelViewProjection", renderTask->projection * renderable.modelViewTransform() ).upload();
    ShaderUniform< math::Matrix4f >( "normalsView", renderable.viewModelTransform().transpose() ).upload();

    const ManagedMeshBase& mesh = static_cast< ManagedMeshBase& >( renderable.geometry().feature( ROLE_DEFAULT_MESH ) );
    this->videoResource( mesh ).get().render();
}



}  // namespace Carna :: base

}  // namespace Carna

#endif // MESHRENDERINGSTAGE_H_6014714286
