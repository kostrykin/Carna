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

#ifndef OPAQUERENDERINGSTAGE_H_6014714286
#define OPAQUERENDERINGSTAGE_H_6014714286

#include <Carna/base/MeshRenderingStage.h>

/** \file   OpaqueRenderingStage.h
  * \brief  Defines \ref Carna::presets::OpaqueRenderingStage.
  */

namespace Carna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// OpaqueRenderingStage
// ----------------------------------------------------------------------------------

/** \brief
  * Defines rendering stage that renders opaque meshes.
  *
  * \note
  * In the \ref RenderingProcess "rendering process" this stage will usually be
  * inserted \em before such stages that render transparent geometry, like
  * \ref TransparentRenderingStage and \ref RayMarchingStage.
  *
  * \author Leonid Kostrykin
  * \date   21.2.15 - 6.3.15
  */
typedef base::MeshRenderingStage< base::Renderable::VideoResourcesOrder< base::MeshRenderingStageBase::ROLE_DEFAULT_MATERIAL > >
    OpaqueRenderingStage;



}  // namespace Carna :: presets

}  // namespace Carna

#endif // OPAQUERENDERINGSTAGE_H_6014714286
