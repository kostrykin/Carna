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

#include <Carna/presets/RayMarchingStage.h>
#include <Carna/base/Mesh.h>
#include <Carna/base/Vertex.h>
#include <Carna/base/IndexBuffer.h>
#include <Carna/base/ShaderManager.h>
#include <Carna/base/RenderState.h>
#include <Carna/base/ShaderUniform.h>
#include <Carna/base/math.h>

namespace Carna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// RayMarchingStage :: Details
// ----------------------------------------------------------------------------------

struct RayMarchingStage::Details
{

    typedef base::Mesh< base::VertexBase, uint8_t > SliceMesh;

    Details();
    
    ~Details();

    base::RenderTask* renderTask;
    const base::Viewport* viewPort;
    unsigned int mySampleRate;
    SliceMesh& sliceMesh;

    static SliceMesh& createSliceMesh();

}; // RayMarchingStage :: Details


RayMarchingStage::Details::SliceMesh& RayMarchingStage::Details::createSliceMesh()
{
    /* Actually, one would assume that the radius should be _half_ of the square root
     * of 3. But if specified so, one encounters "holes" in volume renderings. For
     * the moment, just the square root of 3 seems to produce slices that are large
     * enough, although this particular value is somewhat "random".
     */
    const float radius = std::sqrt( 3.f );
    base::VertexBase vertices[ 4 ];
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

    return SliceMesh::create( base::IndexBufferBase::PRIMITIVE_TYPE_TRIANGLE_FAN, vertices, 4, indices, 4 );
}


RayMarchingStage::Details::Details()
    : renderTask( nullptr )
    , viewPort( nullptr )
    , mySampleRate( DEFAULT_SAMPLE_RATE )
    , sliceMesh( createSliceMesh() )
{
}


RayMarchingStage::Details::~Details()
{
    sliceMesh.release();
}



// ----------------------------------------------------------------------------------
// RayMarchingStage :: VideoResources
// ----------------------------------------------------------------------------------

struct RayMarchingStage::VideoResources
{

    VideoResources( base::GLContext& glc, const base::ShaderProgram& shader, Details::SliceMesh& sliceMesh );

    Details::SliceMesh::VideoResourceAcquisition sliceMeshVR;
    const base::ShaderProgram& shader;
    std::map< unsigned int, base::Sampler* > samplers;

    void renderSlice
        ( RayMarchingStage& self
        , const base::Renderable& renderable
        , const base::math::Matrix4f& sliceTangentModel
        , const base::math::Matrix4f& modelView );

}; // RayMarchingStage :: VideoResources


RayMarchingStage::VideoResources::VideoResources
        ( base::GLContext& glc
        , const base::ShaderProgram& shader
        , Details::SliceMesh& sliceMesh )
    : sliceMeshVR( glc, sliceMesh )
    , shader( shader )
{
}


void RayMarchingStage::VideoResources::renderSlice
    ( RayMarchingStage& self
    , const base::Renderable& renderable
    , const base::math::Matrix4f& sliceTangentModel
    , const base::math::Matrix4f& modelView )
{
    unsigned int lastUnit = base::Texture3D::SETUP_UNIT;
    std::vector< unsigned int > roles;
    const base::Texture3D* anyTexture;
    renderable.geometry().visitFeatures( [&]( base::GeometryFeature& gf, unsigned int role )
        {
            if( dynamic_cast< base::Texture3D* >( &gf ) != nullptr )
            {
                const base::Texture3D& texture = static_cast< const base::Texture3D& >( gf );
                anyTexture = &texture;
                self.videoResource( texture ).bind( ++lastUnit );
                samplers[ role ]->bind( lastUnit );
                roles.push_back( role );
            }
        }
    );

    /* We assume here that the texture coordinates correction is same for all
     * textures, i.e. all textures have same resolution.
     */
    const base::math::Matrix4f modelTexture =
        ( anyTexture == nullptr ? base::math::identity4f() : anyTexture->textureCoordinatesCorrection )
        * base::math::translation4f( 0.5f, 0.5f, 0.5f );

    /* Configure shader.
     */
    base::ShaderUniform< base::math::Matrix4f >( "sliceTangentModel", sliceTangentModel ).upload();
    base::ShaderUniform< base::math::Matrix4f >( "modelViewProjection", self.pimpl->renderTask->projection * modelView ).upload();
    base::ShaderUniform< base::math::Matrix4f >( "modelTexture", modelTexture ).upload();
    for( unsigned int samplerOffset = 0; samplerOffset < roles.size(); ++samplerOffset )
    {
        const unsigned int role = roles[ samplerOffset ];
        const unsigned int unit = base::Texture3D::SETUP_UNIT + 1 + samplerOffset;
        const std::string& uniformName = self.uniformName( role );
        base::ShaderUniform< int >( uniformName, unit ).upload();
    }

    /* Invoke shader.
     */
    sliceMeshVR.render();
}



// ----------------------------------------------------------------------------------
// RayMarchingStage
// ----------------------------------------------------------------------------------

RayMarchingStage::RayMarchingStage( unsigned int geometryType )
    : base::GeometryStage< base::Renderable::DepthOrder< base::Renderable::ORDER_BACK_TO_FRONT > >
        ::GeometryStage( geometryType )
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
        base::ShaderManager::instance().releaseShader( vr->shader );

        /* Release texture samplers.
         */
        for( auto samplerItr = vr->samplers.begin(); samplerItr != vr->samplers.end(); ++samplerItr )
        {
            delete samplerItr->second;
        }
    }
}


void RayMarchingStage::render( base::GLContext& glc, const base::Renderable& renderable )
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
    const Vector4f viewDirectionInModelSpace = ( viewModel * Vector4f( 0, 0, -1, 0 ) ).normalized();

    /* Construct billboard at segment center, i.e. plane that always faces the camera.
     */
    const float s = std::sqrt( 2.f ) / 2;
    const Vector4f modelNormal    = s * -viewDirectionInModelSpace;
    const Vector4f modelTangent   = s * ( viewModel * Vector4f( 1, 0, 0, 0 ) ).normalized();
    const Vector4f modelBitangent = s * ( viewModel * Vector4f( 0, 1, 0, 0 ) ).normalized();
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


void RayMarchingStage::loadVideoResources( base::GLContext& glc )
{
    const base::ShaderProgram& shader = loadShader();
    vr.reset( new VideoResources( glc, shader, pimpl->sliceMesh ) );
    createSamplers( [&]( unsigned int role, base::Sampler* sampler )
        {
            CARNA_ASSERT( vr->samplers.find( role ) == vr->samplers.end() );
            vr->samplers[ role ] = sampler;
        }
    );
}


void RayMarchingStage::renderPass
    ( const base::math::Matrix4f& vt
    , base::RenderTask& rt
    , const base::Viewport& vp )
{
    if( vr.get() == nullptr )
    {
        loadVideoResources( rt.renderer.glContext() );
    }

    rt.renderer.glContext().setShader( vr->shader );
    configureShader( rt.renderer.glContext() );

    pimpl->renderTask = &rt;
    pimpl->viewPort = &vp;
    
    /* Configure proper OpenGL state.
     */
    base::RenderState rs( rt.renderer.glContext() );
    rs.setDepthWrite( false );
    
    /* Do the rendering.
     */
    base::GeometryStage< base::Renderable::DepthOrder< base::Renderable::ORDER_BACK_TO_FRONT > >::renderPass( vt, rt, vp );

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



}  // namespace Carna :: presets

}  // namespace Carna
