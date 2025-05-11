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

#include <LibCarna/presets/TransparentRenderingStage.hpp>
#include <LibCarna/base/RenderState.hpp>

namespace LibCarna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// TransparentRenderingStage
// ----------------------------------------------------------------------------------

TransparentRenderingStage::TransparentRenderingStage( unsigned int geometryType )
    : MeshRenderingStage< base::Renderable::BackToFront >::MeshRenderingStage( geometryType )
{
}


void TransparentRenderingStage::renderPass
    ( const base::math::Matrix4f& viewTransform
    , base::RenderTask& rt
    , const base::Viewport& vp )
{
    /* Configure render state.
     */
    base::RenderState rs;
    rs.setBlend( true );
    rs.setDepthWrite( false );

    /* Do the rendering.
     */
    base::MeshRenderingStage< base::Renderable::BackToFront >::renderPass( viewTransform, rt, vp );
}



}  // namespace LibCarna :: presets

}  // namespace LibCarna
