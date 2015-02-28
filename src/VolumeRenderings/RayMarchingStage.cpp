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

#include <Carna/VolumeRenderings/RayMarchingStage.h>
#include <Carna/base/view/Mesh.h>
#include <Carna/base/view/Vertex.h>
#include <Carna/base/view/IndexBuffer.h>
#include <Carna/base/view/Texture3DManager.h>
#include <Carna/base/view/ShaderManager.h>
#include <Carna/base/view/RenderState.h>
#include <Carna/base/math.h>

namespace Carna
{

namespace VolumeRenderings
{



// ----------------------------------------------------------------------------------
// RayMarchingStage :: Details
// ----------------------------------------------------------------------------------

struct RayMarchingStage::Details
{

    Details();

    base::view::RenderTask* renderTask;
    const base::view::Viewport* viewPort;
    unsigned int mySampleRate;

}; // RayMarchingStage :: Details


RayMarchingStage::Details::Details()
    : renderTask( nullptr )
    , viewPort( nullptr )
    , mySampleRate( DEFAULT_SAMPLE_RATE )
{
}



// ----------------------------------------------------------------------------------
// RayMarchingStage :: VideoResources
// ----------------------------------------------------------------------------------

struct RayMarchingStage::VideoResources
{

    VideoResources( const base::view::ShaderProgram& shader );

    base::view::Mesh< base::view::VertexBase, uint8_t > sliceMesh;
    const base::view::ShaderProgram& shader;
    std::map< unsigned int, base::view::Sampler* > samplers;

    void renderSlice
        ( RayMarchingStage& self
        , const base::view::Renderable& renderable
        , const base::math::Matrix4f& sliceTangentModel
        , const base::math::Matrix4f& modelView );

}; // RayMarchingStage :: VideoResources


RayMarchingStage::VideoResources::VideoResources( const base::view::ShaderProgram& shader )
    : sliceMesh( base::view::IndexBufferBase::PRIMITIVE_TYPE_TRIANGLE_FAN )
    , shader( shader )
{
    const float radius = std::sqrt( 3.f ) / 2;
    base::view::VertexBase vertices[ 4 ];
    uint8_t indices[ 4 ];

    vertices[ 0 ].x = -radius;
    vertices[ 0 ].y = -radius;
    indices [ 0 ] = 0;

    vertices[ 1 ].x = +radius;
    vertices[ 1 ].y = -radius;
    indices [ 1 ] = 1;

    vertices[ 2 ].x = +radius;
    vertices[ 2 ].y = +radius;
    indices [ 2 ] = 2;

    vertices[ 3 ].x = -radius;
    vertices[ 3 ].y = +radius;
    indices [ 3 ] = 3;

    sliceMesh.vertexBuffer().copy( vertices, 4 );
    sliceMesh.indexBuffer().copy( indices, 4 );
}


void RayMarchingStage::VideoResources::renderSlice
    ( RayMarchingStage& self
    , const base::view::Renderable& renderable
    , const base::math::Matrix4f& sliceTangentModel
    , const base::math::Matrix4f& modelView )
{
    unsigned int lastUnit = base::view::Texture3D::SETUP_UNIT;
    std::vector< unsigned int > roles;
    renderable.geometry().visitAggregates( [&]( base::view::GeometryAggregate& ga, unsigned int role )
        {
            if( dynamic_cast< base::view::Texture3DManager* >( &ga ) != nullptr )
            {
                const base::view::Texture3DManager& textureManager = static_cast< const base::view::Texture3DManager& >( ga );
                textureManager.resource().bind( ++lastUnit );
                samplers[ role ]->bind( lastUnit );
                roles.push_back( role );
            }
        }
    );

    /* Configure shader.
     */
    base::view::ShaderProgram::putUniform4x4f( "sliceTangentModel", sliceTangentModel );
    base::view::ShaderProgram::putUniform4x4f( "modelViewProjection", self.pimpl->renderTask->projection * modelView );
    for( unsigned int samplerOffset = 0; samplerOffset < roles.size(); ++samplerOffset )
    {
        const unsigned int role = roles[ samplerOffset ];
        const unsigned int sampler = base::view::Texture3D::SETUP_UNIT + 1 + samplerOffset;
        const std::string& uniformName = self.uniformName( role );
        base::view::ShaderProgram::putUniform1i( uniformName, sampler );
    }

    /* Invoke shader.
     */
    sliceMesh.render();
}



// ----------------------------------------------------------------------------------
// RayMarchingStage
// ----------------------------------------------------------------------------------

RayMarchingStage::RayMarchingStage()
    : base::view::GeometryStage< base::view::Renderable::DepthOrder< base::view::Renderable::ORDER_BACK_TO_FRONT > >
        ::GeometryStage( GEOMETRY_TYPE )
    , pimpl( new Details() )
{
}


RayMarchingStage::~RayMarchingStage()
{
    activateGLContext();
    if( vr.get() != nullptr )
    {
        /* Release main shader.
         */
        base::view::ShaderManager::instance().releaseShader( vr->shader );

        /* Release texture samplers.
         */
        for( auto samplerItr = vr->samplers.begin(); samplerItr != vr->samplers.end(); ++samplerItr )
        {
            delete samplerItr->second;
        }
    }
}


void RayMarchingStage::render( base::view::GLContext& glc, const base::view::Renderable& renderable )
{
    using base::math::Matrix4f;
    using base::math::Vector4f;

    /* Hereinafter the term 'model' is identified with 'segment'.
     */
    const Matrix4f& modelView = renderable.modelViewTransform();

    /* Compute the directional vector from eye to segment center.
     * This vector needs to be renormalized since 'viewModel' may contain scalings.
     */
    const Matrix4f viewModel = modelView.inverse();
    const Vector4f viewDirectionInModelSpace = base::math::normalized( Vector4f( viewModel * Vector4f( 0, 0, -1, 0 ) ) );

    /* Construct billboard at segment center, i.e. plane that always faces the camera.
     */
    const float s = std::sqrt( 2.f ) / 2;
    const Vector4f modelNormal    = s * -viewDirectionInModelSpace;
    const Vector4f modelTangent   = s * base::math::normalized( Vector4f( viewModel * Vector4f( 1, 0, 0, 0 ) ) );
    const Vector4f modelBitangent = s * base::math::normalized( Vector4f( viewModel * Vector4f( 0, 1, 0, 0 ) ) );
    const Matrix4f tangentModel   = base::math::basis4f( modelTangent, modelBitangent, modelNormal );

    /* NOTE: This can be optimized using geometry shader, by sending only the central
     * slice to the GPU and constructing the others in the shader.
     */
    for( unsigned int sampleIdx = 0; sampleIdx < pimpl->mySampleRate; ++sampleIdx )
    {
        const float progress = static_cast< float >( sampleIdx ) / ( pimpl->mySampleRate - 1 );
        const float offset = std::sqrt( 3.f ) * ( 0.5f - progress );

        /* Construct transformation from tangent to model space for specific slice.
         */
        const Matrix4f sliceOffset = base::math::translation4f( viewDirectionInModelSpace * offset );
        const Matrix4f sliceTangentModel = sliceOffset * tangentModel;

        vr->renderSlice( *this, renderable, sliceTangentModel, modelView );
    } 
}


void RayMarchingStage::loadVideoResources()
{
    const base::view::ShaderProgram& shader = loadShader();
    vr.reset( new VideoResources( shader ) );
    createSamplers( [&]( unsigned int role, base::view::Sampler* sampler )
        {
            CARNA_ASSERT( vr->samplers.find( role ) == vr->samplers.end() );
            vr->samplers[ role ] = sampler;
        }
    );
}


void RayMarchingStage::renderPass
    ( const base::math::Matrix4f& vt
    , base::view::RenderTask& rt
    , const base::view::Viewport& vp )
{
    if( vr.get() == nullptr )
    {
        loadVideoResources();
    }

    rt.renderer.glContext().setShader( vr->shader );
    configureShader( rt.renderer.glContext() );

    pimpl->renderTask = &rt;
    pimpl->viewPort = &vp;
    
    /* Configure proper OpenGL state.
     */
    base::view::RenderState rs( rt.renderer.glContext() );
    rs.setDepthWrite( false );
    
    /* Do the rendering.
     */
    base::view::GeometryStage< base::view::Renderable::DepthOrder< base::view::Renderable::ORDER_BACK_TO_FRONT > >::renderPass( vt, rt, vp );

    /* There is no guarantee that 'renderTask' will be valid later.
     */
    pimpl->renderTask = nullptr;
}


void RayMarchingStage::setSampleRate( unsigned int sampleRate )
{
    CARNA_ASSERT( sampleRate >= 2 );
    pimpl->mySampleRate = sampleRate;
}


unsigned int RayMarchingStage::sampleRate() const
{
    return pimpl->mySampleRate;
}



}  // namespace Carna :: VolumeRenderings

}  // namespace Carna
