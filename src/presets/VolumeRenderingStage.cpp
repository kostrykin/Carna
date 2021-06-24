﻿/*
 *  Copyright (C) 2010 - 2015 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#include <Carna/presets/VolumeRenderingStage.h>
#include <Carna/base/Mesh.h>
#include <Carna/base/ManagedTexture3D.h>
#include <Carna/base/ManagedTexture3DInterface.h>
#include <Carna/base/Vertex.h>
#include <Carna/base/IndexBuffer.h>
#include <Carna/base/ShaderManager.h>
#include <Carna/base/RenderState.h>
#include <Carna/base/ShaderUniform.h>
#include <Carna/base/Log.h>
#include <Carna/base/math.h>
#include <Carna/base/text.h>

namespace Carna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// VolumeRenderingStage :: Details
// ----------------------------------------------------------------------------------

struct VolumeRenderingStage::Details
{
    Details();

    base::RenderTask* renderTask;
    const base::Viewport* viewPort;
    unsigned int sampleRate;
    bool stepLengthRequired;
    unsigned int firstVolumeUnit;
};


VolumeRenderingStage::Details::Details()
    : renderTask( nullptr )
    , viewPort( nullptr )
    , sampleRate( DEFAULT_SAMPLE_RATE )
    , stepLengthRequired( true )
{
}



// ----------------------------------------------------------------------------------
// VolumeRenderingStage :: VideoResources
// ----------------------------------------------------------------------------------

struct VolumeRenderingStage::VideoResources
{
    VideoResources( const base::ShaderProgram& shader, unsigned int sampleRate );

    const base::ShaderProgram& shader;
    std::map< unsigned int, base::Sampler* > samplers;
    
    typedef base::Mesh< base::PVertex, uint16_t > SlicesMesh;
    SlicesMesh& slicesMesh( unsigned int sampleRate );
    
private:

    unsigned int mySampleRate;
    std::unique_ptr< SlicesMesh > mySlicesMesh;
    static SlicesMesh* createSlicesMesh( unsigned int sampleRate );
};


VolumeRenderingStage::VideoResources::VideoResources( const base::ShaderProgram& shader, unsigned int sampleRate )
    : shader( shader )
    , mySampleRate( sampleRate + 1 )
{
    /* Create the slices mesh.
     */
    slicesMesh( sampleRate );
}


VolumeRenderingStage::VideoResources::SlicesMesh* VolumeRenderingStage::VideoResources::createSlicesMesh( unsigned int sampleRate )
{
    /* The mesh is constructed in model space. The box [-0,5; +0.5]^3 defines the
     * space that we need to cover. For a ray that hits the center, the distant-most
     * point of the box is half the square root of 3. However, we use a radius that
     * is *slightly* larger, ensuring that numerical errors do not prevent us from
     * covering the whole area. Leaving out this correction factor causes ugly
     * artifacts under particular view angles.
     */
    const float correctionTerm = 1e-2f;
    const float idealRadius = ( 1 + correctionTerm ) * std::sqrt( 3.f ) / 2;

    /* We use even the double of the radius described above because this suppresses
     * some other artifacts that only appear in grids with many cells like 8x8. The
     * reason for these artifacts have not been found yet.
     *
     * ** NOTE: **  We also use the double sample rate here to achieve rendering
     *              results that look the same at the cost of a lower frame rate.
     *
     * ** Update 24.6.2021: **  This has apparently finally been fixed, so we will
     *                          return to using factor 1 (see Version 3.3.2 notes).
     */
    const float radius = idealRadius * 1;
    sampleRate = 1 * sampleRate;
    
    /* Create slices.
     */
    std::vector< typename SlicesMesh::Vertex > vertices( 4 * sampleRate );
    std::vector< typename SlicesMesh::Index  >  indices( 6 * sampleRate );
    for( unsigned int sliceIdx = 0; sliceIdx < sampleRate; ++sliceIdx )
    {
        const float progress = static_cast< float >( sliceIdx ) / ( sampleRate - 1 );
        const float   offset = -2 * radius * ( 0.5f - progress );

        /* Create slice vertices.
         */
        vertices[ 4 * sliceIdx + 0 ].x = -radius;
        vertices[ 4 * sliceIdx + 0 ].y = -radius;
        vertices[ 4 * sliceIdx + 0 ].z =  offset;

        vertices[ 4 * sliceIdx + 1 ].x = +radius;
        vertices[ 4 * sliceIdx + 1 ].y = -radius;
        vertices[ 4 * sliceIdx + 1 ].z =  offset;

        vertices[ 4 * sliceIdx + 2 ].x = +radius;
        vertices[ 4 * sliceIdx + 2 ].y = +radius;
        vertices[ 4 * sliceIdx + 2 ].z =  offset;

        vertices[ 4 * sliceIdx + 3 ].x = -radius;
        vertices[ 4 * sliceIdx + 3 ].y = +radius;
        vertices[ 4 * sliceIdx + 3 ].z =  offset;
        
        /* Create slice indices.
         */
        indices[ 6 * sliceIdx + 0 ] = 4 * sliceIdx + 0;
        indices[ 6 * sliceIdx + 1 ] = 4 * sliceIdx + 1;
        indices[ 6 * sliceIdx + 2 ] = 4 * sliceIdx + 2;
        indices[ 6 * sliceIdx + 3 ] = 4 * sliceIdx + 2;
        indices[ 6 * sliceIdx + 4 ] = 4 * sliceIdx + 3;
        indices[ 6 * sliceIdx + 5 ] = 4 * sliceIdx + 0;
    }
    
    /* Create vertex buffer.
     */
    typedef base::VertexBuffer< SlicesMesh::Vertex > VBuffer;
    VBuffer* const vertexBuffer = new VBuffer();
    vertexBuffer->copy( &vertices.front(), vertices.size() );
    
    /* Create index buffer.
     */
    typedef base::IndexBuffer< SlicesMesh::Index > IBuffer;
    IBuffer* const indexBuffer = new IBuffer( base::IndexBufferBase::PRIMITIVE_TYPE_TRIANGLES );
    indexBuffer->copy( &indices.front(), indices.size() );

    /* Create the mesh.
     */
    return new SlicesMesh
        ( new base::Composition< base::VertexBufferBase >( vertexBuffer )
        , new base::Composition< base:: IndexBufferBase >(  indexBuffer ) );
}


VolumeRenderingStage::VideoResources::SlicesMesh& VolumeRenderingStage::VideoResources::slicesMesh( unsigned int sampleRate )
{
    if( mySampleRate != sampleRate )
    {
        mySampleRate = sampleRate;
        mySlicesMesh.reset( createSlicesMesh( sampleRate ) );

        std::stringstream msg;
        msg << "VolumeRenderingStage: Created new slices mesh with " << sampleRate << " samples per pixel.";
        base::Log::instance().record( base::Log::debug, msg.str() );
    }
    return *mySlicesMesh;
}



// ----------------------------------------------------------------------------------
// VolumeRenderingStage
// ----------------------------------------------------------------------------------

VolumeRenderingStage::VolumeRenderingStage( unsigned int geometryType )
    : base::GeometryStage< base::Renderable::BackToFront >::GeometryStage( geometryType )
    , pimpl( new Details() )
{
}


VolumeRenderingStage::~VolumeRenderingStage()
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


void VolumeRenderingStage::render( const base::Renderable& renderable )
{
    using base::math::Matrix4f;
    using base::math::Vector4f;

    /* Hereinafter the term 'model' is identified with 'segment'.
     */
    const Matrix4f& modelView = renderable.modelViewTransform();

    /* Compute the directional vector from eye to segment center.
     * This vector needs to be renormalized since 'viewModel' may contain scalings.
     */
    const Matrix4f& viewModel = renderable.viewModelTransform();
    const Vector4f  viewDirectionInModelSpace = ( viewModel * Vector4f( 0, 0, -1, 0 ) ).normalized();

    /* Construct billboard at segment center, i.e. a plane that always faces the
     * camera. We choose the planes normal vector inverse to the view direction to
     * ensure that the slices are rendered from back to front.
     */
    const Vector4f modelNormal    = -viewDirectionInModelSpace;
    const Vector4f modelTangent   = ( viewModel * Vector4f( 1, 0, 0, 0 ) ).normalized();
    const Vector4f modelBitangent = ( viewModel * Vector4f( 0, 1, 0, 0 ) ).normalized();
    const Matrix4f tangentModel   = base::math::basis4f( modelTangent, modelBitangent, modelNormal );

    /* Compute the scale to fit the minimum bounding ellipsoid of the volume.
     */
    const Matrix4f tangentModelInverse = tangentModel.inverse();
    base::math::Vector4f corners[] =
    {
        tangentModelInverse * base::math::Vector4f( -0.5f, -0.5f, -0.5f, 1.0f ),
        tangentModelInverse * base::math::Vector4f( -0.5f, -0.5f,  0.5f, 1.0f ),
        tangentModelInverse * base::math::Vector4f( -0.5f,  0.5f, -0.5f, 1.0f ),
        tangentModelInverse * base::math::Vector4f( -0.5f,  0.5f,  0.5f, 1.0f ),
        tangentModelInverse * base::math::Vector4f(  0.5f, -0.5f, -0.5f, 1.0f ),
        tangentModelInverse * base::math::Vector4f(  0.5f, -0.5f,  0.5f, 1.0f ),
        tangentModelInverse * base::math::Vector4f(  0.5f,  0.5f, -0.5f, 1.0f ),
        tangentModelInverse * base::math::Vector4f(  0.5f,  0.5f,  0.5f, 1.0f )
    };
    const float inf = std::numeric_limits< float >::infinity();
    base::math::Vector3f cornersMinima( inf, inf, inf ), cornersMaxima( -inf, -inf, -inf );
    for( std::size_t cornerIdx = 0; cornerIdx < 8; ++cornerIdx )
    for( unsigned char componentIdx = 0; componentIdx < 3; ++componentIdx )
    {
        cornersMinima[ componentIdx ] = std::min( cornersMinima[ componentIdx ], corners[ cornerIdx ][ componentIdx ] );
        cornersMaxima[ componentIdx ] = std::max( cornersMaxima[ componentIdx ], corners[ cornerIdx ][ componentIdx ] );
    }
    const base::math::Vector3f scale = ( cornersMinima.cwiseAbs().cwiseMax( cornersMaxima.cwiseAbs() ) / 0.5f )
        .cwiseMax( base::math::Vector3f( 1, 1, 1 ) );

    /* Lets compute the distance between the slices only if the shader requires this,
     * i.e. it has an uniform named 'stepLength' defined. Initially however we assume
     * that the shader does require this value.
     */
    if( pimpl->stepLengthRequired )
    {
        /* The slices are equally distributed along a line, that's length is the
         * square root of 3. With this we can compute the location of the *last*
         * slice. The *first* slice is located exactly on the opposite side of the
         * model space origin. Transforming both locations to world space and taking
         * their distance, we can compute the step length between two successive
         * slices.
         */
        const Vector4f modelLastSlice  = base::math::vector4< float, 4 >( viewDirectionInModelSpace * std::sqrt( 3.f ) / 2, 1 );
        const Vector4f modelFirstSlice = base::math::vector4< float, 4 >( -modelLastSlice, 1 );
        const Vector4f worldLastSlice  = renderable.geometry().worldTransform() * modelLastSlice;
        const Vector4f worldFirstSlice = renderable.geometry().worldTransform() * modelFirstSlice;
        const float totalLength = base::math::vector3< float, 4 >( worldFirstSlice - worldLastSlice ).norm() * scale.z();
        const float stepLength = totalLength / ( pimpl->sampleRate + 1 );
        pimpl->stepLengthRequired = base::ShaderUniform< float >( "stepLength", stepLength ).upload();
        
        /* Write to log that the step length will not be computed any more.
         */
        if( !pimpl->stepLengthRequired )
        {
            base::Log::instance().record( base::Log::debug, "VolumeRenderingStage: Step length ignored." );
        }
    }
    
    /* Bind all 'ManagedTexture3D' geometry features that have a sampler.
     */
    unsigned int lastUnit = pimpl->firstVolumeUnit - 1;
    std::vector< unsigned int > roles;
    const base::ManagedTexture3D* anyTexture = nullptr;
    renderable.geometry().visitFeatures( [&]( base::GeometryFeature& gf, unsigned int role )
        {
            if( dynamic_cast< base::ManagedTexture3D* >( &gf ) != nullptr )
            {
                const auto samplerItr = vr->samplers.find( role );
                if( samplerItr != vr->samplers.end() )
                {
                    const base::ManagedTexture3D& texture = static_cast< const base::ManagedTexture3D& >( gf );
                    if( anyTexture != nullptr && anyTexture->size != texture.size )
                    {
                        base::Log::instance().record( base::Log::error, "Renderable has ManagedTexture3D objects of conflicting resolutions." );
                    }
                    anyTexture = &texture;
                    videoResource( texture ).get().bind( ++lastUnit );
                    samplerItr->second->bind( lastUnit );
                    roles.push_back( role );
                }
            }
        }
    );

    /* We assume here that the texture coordinates correction is same for all
     * textures, i.e. all textures *of one geometry node* have same resolution.
     */
    const Matrix4f modelTexture =
        ( anyTexture == nullptr ? base::math::identity4f() : anyTexture->textureCoordinatesCorrection )
        * base::math::translation4f( 0.5f, 0.5f, 0.5f );

    /* Upload matrices to the shader and set the texture samplers properly.
     */
    base::ShaderUniform< Matrix4f >( "modelViewProjection", pimpl->renderTask->projection * modelView ).upload();
    base::ShaderUniform< Matrix4f >( "modelTexture", modelTexture ).upload();
    base::ShaderUniform< Matrix4f >( "tangentModel", tangentModel * base::math::scaling4f( scale ) ).upload();
    for( unsigned int samplerOffset = 0; samplerOffset < roles.size(); ++samplerOffset )
    {
        const unsigned int role = roles[ samplerOffset ];
        const unsigned int unit = pimpl->firstVolumeUnit + samplerOffset;
        const std::string& uniformName = this->uniformName( role );
        base::ShaderUniform< int >( uniformName, unit ).upload();
    }

    /* Apply custom shader setup.
     */
    configureShader( renderable );

    /* Invoke shader.
     */
    VideoResources::SlicesMesh& slicesMesh = vr->slicesMesh( pimpl->sampleRate );
    slicesMesh.render();
}


unsigned int VolumeRenderingStage::loadVideoResources()
{
    const base::ShaderProgram& shader = acquireShader();
    vr.reset( new VideoResources( shader, pimpl->sampleRate ) );
    createVolumeSamplers( [&]( unsigned int role, base::Sampler* sampler )
        {
            CARNA_ASSERT( vr->samplers.find( role ) == vr->samplers.end() );
            vr->samplers[ role ] = sampler;
        }
    );
    return base::Texture< 0 >::SETUP_UNIT + 1;
}


void VolumeRenderingStage::renderPass
    ( const base::math::Matrix4f& vt
    , base::RenderTask& rt
    , const base::Viewport& vp )
{
    if( vr.get() == nullptr )
    {
        pimpl->firstVolumeUnit = loadVideoResources();
    }

    rt.renderer.glContext().setShader( vr->shader );
    configureShader();

    pimpl->renderTask = &rt;
    pimpl->viewPort = &vp;
    
    /* Do the rendering.
     */
    base::GeometryStage< base::Renderable::BackToFront >::renderPass( vt, rt, vp );

    /* There is no guarantee that 'renderTask' will be valid later.
     */
    pimpl->renderTask = nullptr;
}


void VolumeRenderingStage::setSampleRate( unsigned int sampleRate )
{
    CARNA_ASSERT( sampleRate >= 2 );
    pimpl->sampleRate = sampleRate;
}


unsigned int VolumeRenderingStage::sampleRate() const
{
    return pimpl->sampleRate;
}



}  // namespace Carna :: presets

}  // namespace Carna
