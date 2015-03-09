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

#include <Carna/presets/TransparentRenderingStage.h>
#include <Carna/base/RenderState.h>

namespace Carna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// TransparentRenderingStage
// ----------------------------------------------------------------------------------

TransparentRenderingStage::TransparentRenderingStage( unsigned int geometryType )
    : MeshRenderingStage< base::Renderable::DepthOrder< base::Renderable::ORDER_BACK_TO_FRONT > >::MeshRenderingStage( geometryType )
{
}


void TransparentRenderingStage::renderPass
    ( const base::math::Matrix4f& viewTransform
    , base::RenderTask& rt
    , const base::Viewport& vp )
{
    /* Configure render state.
     */
    base::RenderState rs( rt.renderer.glContext() );
    rs.setBlend( true );
    rs.setDepthWrite( false );

    /* Do the rendering.
     */
    base::MeshRenderingStage< base::Renderable::DepthOrder< base::Renderable::ORDER_BACK_TO_FRONT > >
        ::renderPass( viewTransform, rt, vp );
}



}  // namespace Carna :: presets

}  // namespace Carna
