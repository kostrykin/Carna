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
#include <Carna/base/view/Mesh.h>
#include <Carna/base/view/Vertex.h>
#include <Carna/base/view/IndexBuffer.h>
#include <Carna/base/Matrix4f.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// RayMarchingStage :: Details
// ----------------------------------------------------------------------------------

struct RayMarchingStage::Details
{

    Details( const ShaderProgram& shader );

    ~Details();

    RenderTask* renderTask;
    const Viewport* viewPort;
    Mesh< VertexBase, uint8_t > sliceMesh;
    const ShaderProgram& shader;

    void renderSlice( const Matrix4f& sliceTangentModel, const Matrix4f& modelView );

}; // RayMarchingStage :: Details


RayMarchingStage::Details::Details( const ShaderProgram& shader )
    : renderTask( nullptr )
    , viewPort( nullptr )
    , sliceMesh( IndexBufferBase::PRIMITIVE_TYPE_TRIANGLE_FAN )
    , shader( shader )
{
    const float radius = std::sqrt( 3.f ) / 2;
    VertexBase vertices[ 4 ];

    vertices[ 0 ].x = -radius;
    vertices[ 0 ].y = -radius;

    vertices[ 1 ].x = +radius;
    vertices[ 1 ].y = -radius;

    vertices[ 2 ].x = +radius;
    vertices[ 2 ].y = +radius;

    vertices[ 3 ].x = -radius;
    vertices[ 3 ].y = +radius;

    sliceMesh.vertexBuffer().copy( vertices, 4 );
}


RayMarchingStage::Details::~Details()
{
    if( renderTask != nullptr )
    {
        renderTask->renderer.glContext().makeActive();
    }
}


void RayMarchingStage::Details::renderSlice( const Matrix4f& sliceTangentModel, const Matrix4f& modelView )
{
    /* Configure shader.
     */
    ShaderProgram::putUniform4x4f( "sliceTangentModel", sliceTangentModel );
    ShaderProgram::putUniform4x4f( "modelViewProjection", renderTask->projection * modelView );

    /* Invoke shader.
     */
    sliceMesh.render();
}



// ----------------------------------------------------------------------------------
// RayMarchingStage
// ----------------------------------------------------------------------------------

RayMarchingStage::RayMarchingStage()
    : GeometryStage< Renderable::DepthOrder< Renderable::ORDER_BACK_TO_FRONT > >::GeometryStage( GEOMETRY_TYPE )
    , mySampleRate( DEFAULT_SAMPLE_RATE )
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
        const Vector4f offset = viewDirectionInModelSpace * std::sqrt( 3.f ) * ( 0.5f - static_cast< float >( sampleIdx ) / ( mySampleRate - 1 ) );
        if( std::abs( offset.x() ) <= 0.5f && std::abs( offset.y() ) <= 0.5f && std::abs( offset.z() ) <= 0.5f )
        {
            /* Construct transformation from tangent to model space for specific slice.
            */
            const Matrix4f sliceOffset = translation4f( offset );
            const Matrix4f sliceTangentModel = sliceOffset * tangentModel;

            pimpl->renderSlice( sliceTangentModel, modelView );
        }
    }
}


void RayMarchingStage::renderPass( RenderTask& rt, const Viewport& vp )
{
    if( pimpl.get() == nullptr )
    {
        const ShaderProgram& shader = loadShader();
        pimpl.reset( new Details( shader ) );
    }

    rt.renderer.glContext().setShader( pimpl->shader );
    pimpl->renderTask = &rt;
    pimpl->viewPort = &vp;

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
