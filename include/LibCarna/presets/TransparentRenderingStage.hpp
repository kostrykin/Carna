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

#ifndef TRANSPARENTRENDERINGSTAGE_H_6014714286
#define TRANSPARENTRENDERINGSTAGE_H_6014714286

#include <LibCarna/base/MeshRenderingStage.hpp>

/** \file   TransparentRenderingStage.h
  * \brief  Defines \ref LibCarna::presets::TransparentRenderingStage.
  */

namespace LibCarna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// TransparentRenderingStage
// ----------------------------------------------------------------------------------
    
/** \brief
  * Implements rendering stage that renders transparent meshes.
  *
  * The `%TransparentRenderingStage` constructor takes a geometry type parameter:
  *
  * \snippet ModuleTests/TransparentRenderingStageTest.cpp instantiation
  *
  * The concept of geometry types is explained \ref GeometryTypes "here".
  *
  * \note
  * In the \ref RenderingProcess "rendering process" this stage will usually be
  * inserted \em after such stages that render opaque geometry, like
  * \ref CuttingPlanesStage and \ref OpaqueRenderingStage.
  *
  * The following example code configures the \ref SceneGraph "scene" s.t. it
  * produces the rendering presented further below:
  *
  * \snippet ModuleTests/TransparentRenderingStageTest.cpp scene_setup
  *
  * The concept of materials, meshes and other geometry feature is explained
  * \ref GeometryFeatures "here".
  *
  * \image html TransparentRenderingStageTest/transparentFromFront.png "exemplary rendering of two box meshes from code above"
  *
  * \author Leonid Kostrykin
  */
class LIBCARNA TransparentRenderingStage : public base::MeshRenderingStage< base::Renderable::BackToFront >
{

public:
    
    /** \brief
      * Instantiates.
      *
      * \param geometryType
      *     is the \ref GeometryTypes "geometry type" rendered by this stage.
      */
    explicit TransparentRenderingStage( unsigned int geometryType );

    /** \brief
      * Renders transparent geometries with enabled
      * \ref base::RenderState::setBlend "blending" and disabled
      * \ref base::RenderState::setDepthWrite "depth writing".
      */
    virtual void renderPass
        ( const base::math::Matrix4f& viewTransform
        , base::RenderTask& rt
        , const base::Viewport& vp ) override;

}; // TransparentRenderingStage



}  // namespace LibCarna :: presets

}  // namespace LibCarna

#endif // TRANSPARENTRENDERINGSTAGE_H_6014714286
