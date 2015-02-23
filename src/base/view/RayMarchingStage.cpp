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

#include <Carna/base/view/RayMarchingStage.h>
#include <Carna/base/Matrix4f.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// RayMarchingStage
// ----------------------------------------------------------------------------------

RayMarchingStage::RayMarchingStage()
    : GeometryStage< Renderable::DepthOrder< Renderable::ORDER_BACK_TO_FRONT > >::GeometryStage( GEOMETRY_TYPE )
    , mySampleRate( DEFAULT_SAMPLE_RATE )
    , renderTask( nullptr )
    , viewPort( nullptr )
{
}


void RayMarchingStage::render( const Renderable& renderable )
{
    // we will need for implementation:
    //
    //   renderable.modelViewTransform();
    //   renderable.geometry().worldTransform();
    //   ...
}


void RayMarchingStage::beginPass()
{
}


void RayMarchingStage::finishPass()
{
}


void RayMarchingStage::renderPass( RenderTask& rt, const Viewport& vp )
{
    this->renderTask = &rt;
    this->viewPort = &vp;

    beginPass();

    GeometryStage< Renderable::DepthOrder< Renderable::ORDER_BACK_TO_FRONT > >::renderPass( rt, vp );
    
    finishPass();
}


void RayMarchingStage::setSampleRate( unsigned int sampleRate )
{
    CARNA_ASSERT( sampleRate >= 2 );
    mySampleRate = sampleRate;
}


unsigned int RayMarchingStage::sampleRate() const
{
    return mySampleRate;
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
