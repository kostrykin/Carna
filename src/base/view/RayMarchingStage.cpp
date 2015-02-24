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
    /* Hereinafter the term 'model' is identified with 'segment'.
     */
    const Matrix4f& modelView = renderable.modelViewTransform();

    /* Compute the directional vector from eye to segment center.
     * This vector needs to be renormalized since 'viewModel' may contain scalings.
     */
    const Matrix4f viewModel = modelView.inverse();
    const Vector4f viewDirectionInModelSpace = normalized( Vector4f( viewModel * Vector4f( 0, 0, -1, 0 ) ) );

    /* Construct billboard at segment center, i.e. plane that always faces the camera.
     */
    const Vector4f modelNormal = viewModel * Vector4f( 0, 0, 1, 0 );
    const Vector4f modelTangent = viewModel * Vector4f( 1, 0, 0, 0 );
    const Vector4f modelBitangent = viewModel * Vector4f( 0, 1, 0, 0 );
    const Matrix4f tangentModel = basis4f( modelTangent, modelBitangent, modelNormal );

    /* NOTE: This can be optimized using geometry shader, by sending only the central
     * slice to the GPU and constructing the others in the shader.
     */
    for( unsigned int sampleIdx = 0; sampleIdx < mySampleRate; ++sampleIdx )
    {
        const Vector4f offset = viewDirectionInModelSpace * sqrt( 3.f ) * ( 0.5f - static_cast< float >( sampleIdx ) / ( mySampleRate - 1 ) );
        if( std::abs( offset.x() ) <= 0.5f && std::abs( offset.y() ) <= 0.5f && std::abs( offset.z() ) <= 0.5f )
        {
            /* Construct transformation from tangent to model space for specific slice.
            */
            const Matrix4f sliceOffset = translation4f( offset );
            const Matrix4f sliceTangentModel = sliceOffset * tangentModel;

            renderSlice( sliceTangentModel, modelView );
        }
    }
}


void RayMarchingStage::renderSlice( const Matrix4f& sliceTangentModel, const Matrix4f& modelView )
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
