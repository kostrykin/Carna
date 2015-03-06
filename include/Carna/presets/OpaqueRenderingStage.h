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

typedef base::MeshRenderingStage< base::Renderable::VideoResourcesOrder< base::MeshRenderingStageBase::ROLE_DEFAULT_MATERIAL > >
    OpaqueRenderingStage;



}  // namespace Carna :: presets

}  // namespace Carna

#endif // OPAQUERENDERINGSTAGE_H_6014714286
