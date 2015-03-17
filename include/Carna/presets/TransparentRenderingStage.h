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

#ifndef TRANSPARENTRENDERINGSTAGE_H_6014714286
#define TRANSPARENTRENDERINGSTAGE_H_6014714286

#include <Carna/base/MeshRenderingStage.h>

/** \file   TransparentRenderingStage.h
  * \brief  Defines \ref Carna::presets::TransparentRenderingStage.
  */

namespace Carna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// TransparentRenderingStage
// ----------------------------------------------------------------------------------

class CARNA_LIB TransparentRenderingStage : public base::MeshRenderingStage< base::Renderable::BackToFront >
{

public:

    TransparentRenderingStage( unsigned int geometryType );

    virtual void renderPass
        ( const base::math::Matrix4f& viewTransform
        , base::RenderTask& rt
        , const base::Viewport& vp ) override;

}; // TransparentRenderingStage



}  // namespace Carna :: presets

}  // namespace Carna

#endif // TRANSPARENTRENDERINGSTAGE_H_6014714286
