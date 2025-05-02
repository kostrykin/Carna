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

#include <Carna/presets/CuttingPlanesStage.h>
#include <Carna/base/Mesh.h>
#include <Carna/base/Vertex.h>
#include <Carna/base/IndexBuffer.h>
#include <Carna/base/ShaderManager.h>
#include <Carna/base/ShaderUniform.h>
#include <Carna/base/RenderState.h>
#include <Carna/base/math.h>

namespace Carna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// CuttingPlanesStage :: Details
// ----------------------------------------------------------------------------------

struct CuttingPlanesStage::Details
{
    Details( unsigned int planeGeometryType );

    base::RenderQueue< void > planes;

    base::RenderTask* renderTask;
    const base::Viewport* viewPort;

    float windowingLevel;
    float windowingWidth;
    bool renderingInverse;
};


CuttingPlanesStage::Details::Details( unsigned int planeGeometryType )
    : planes( planeGeometryType )
    , renderTask( nullptr )
    , viewPort( nullptr )
    , windowingLevel( DEFAULT_WINDOWING_LEVEL )
    , windowingWidth( DEFAULT_WINDOWING_WIDTH )
    , renderingInverse( false )
{
}



// ----------------------------------------------------------------------------------
// CuttingPlanesStage :: VideoResources
// ----------------------------------------------------------------------------------

struct CuttingPlanesStage::VideoResources
{
    VideoResources( const base::ShaderProgram& shader );

    typedef base::Mesh< base::PVertex, uint8_t > PlaneMesh;
    const std::unique_ptr< PlaneMesh > planeMesh;
    static PlaneMesh* createPlaneMesh();
    
    const base::ShaderProgram& shader;
    base::Sampler volumeSampler;
};


CuttingPlanesStage::VideoResources::VideoResources( const base::ShaderProgram& shader )
    : planeMesh( createPlaneMesh() )
    , shader( shader )
    , volumeSampler
        ( base::Sampler::WRAP_MODE_CLAMP, base::Sampler::WRAP_MODE_CLAMP, base::Sampler::WRAP_MODE_CLAMP
        , base::Sampler::FILTER_LINEAR, base::Sampler::FILTER_LINEAR )
{
}


CuttingPlanesStage::VideoResources::PlaneMesh* CuttingPlanesStage::VideoResources::createPlaneMesh()
{
    const float radius = std::sqrt( 3.f ) / 2;
    base::PVertex vertices[ 4 ];
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
    
    /* Create vertex buffer.
     */
    typedef base::VertexBuffer< PlaneMesh::Vertex > VBuffer;
    VBuffer* const vertexBuffer = new VBuffer();
    vertexBuffer->copy( vertices, 4 );
    
    /* Create index buffer.
     */
    typedef base::IndexBuffer< PlaneMesh::Index > IBuffer;
    IBuffer* const indexBuffer = new IBuffer( base::IndexBufferBase::PRIMITIVE_TYPE_TRIANGLE_FAN );
    indexBuffer->copy( indices, 4 );

    /* Create the mesh.
     */
    return new PlaneMesh
        ( new base::Composition< base::VertexBufferBase >( vertexBuffer )
        , new base::Composition< base:: IndexBufferBase >(  indexBuffer ) );
}



// ----------------------------------------------------------------------------------
// CuttingPlanesStage
// ----------------------------------------------------------------------------------

const float CuttingPlanesStage::DEFAULT_WINDOWING_WIDTH = 1.0f;
const float CuttingPlanesStage::DEFAULT_WINDOWING_LEVEL = 0.5f;
const unsigned int CuttingPlanesStage::ROLE_INTENSITIES = 0;


CuttingPlanesStage::CuttingPlanesStage( unsigned int volumeGeometryType, unsigned int planeGeometryType )
    : base::GeometryStage< void >::GeometryStage( volumeGeometryType )
    , pimpl( new Details( planeGeometryType ) )
{
}


CuttingPlanesStage::~CuttingPlanesStage()
{
    activateGLContext();
    if( vr.get() != nullptr )
    {
        /* Release main shader.
         */
        base::ShaderManager::instance().releaseShader( vr->shader );
    }
}


void CuttingPlanesStage::setWindowingLevel( float windowingLevel )
{
    pimpl->windowingLevel = windowingLevel;
}


void CuttingPlanesStage::setWindowingWidth( float windowingWidth )
{
    pimpl->windowingWidth = windowingWidth;
}


void CuttingPlanesStage::setRenderingInverse( bool inverse )
{
    pimpl->renderingInverse = inverse;
}


float CuttingPlanesStage::windowingLevel() const
{
    return pimpl->windowingLevel;
}


float CuttingPlanesStage::windowingWidth() const
{
    return pimpl->windowingWidth;
}


float CuttingPlanesStage::minimumIntensity() const
{
    return pimpl->windowingLevel - pimpl->windowingWidth / 2;
}


float CuttingPlanesStage::maximumIntensity() const
{
    return pimpl->windowingLevel + pimpl->windowingWidth / 2;
}


bool CuttingPlanesStage::isRenderingInverse() const
{
    return pimpl->renderingInverse;
}

    
void CuttingPlanesStage::buildRenderQueues( base::Node& root, const base::math::Matrix4f& viewTransform )
{
    GeometryStage< void >::buildRenderQueues( root, viewTransform );
    pimpl->planes.build( root, viewTransform );
}


void CuttingPlanesStage::rewindRenderQueues()
{
    GeometryStage< void >::rewindRenderQueues();
    pimpl->planes.rewind();
}


void CuttingPlanesStage::updateRenderQueues( const base::math::Matrix4f& viewTransform )
{
    GeometryStage< void >::updateRenderQueues( viewTransform );
    pimpl->planes.updateModelViewTransforms( viewTransform );
}


void CuttingPlanesStage::render( const base::Renderable& volume )
{
    /* Bind texture and volumeSampler to free texture unit.
     */
    const static unsigned int TEXTURE_UNIT = base::Texture< 0 >::SETUP_UNIT + 1;
    const base::ManagedTexture3D& texture = static_cast< const base::ManagedTexture3D& >( volume.geometry().feature( ROLE_INTENSITIES ) );
    this->videoResource( texture ).get().bind( TEXTURE_UNIT );
    vr->volumeSampler.bind( TEXTURE_UNIT );
    
    /* Upload volume-specific uniforms that are equal for all planes.
     */
    const base::math::Matrix4f modelViewProjection = pimpl->renderTask->projection * volume.modelViewTransform();
    const base::math::Matrix4f modelTexture = texture.textureCoordinatesCorrection * base::math::translation4f( 0.5f, 0.5f, 0.5f );
    base::ShaderUniform< base::math::Matrix4f >( "modelViewProjection", modelViewProjection ).upload();
    base::ShaderUniform< base::math::Matrix4f >( "modelTexture", modelTexture ).upload();
    base::ShaderUniform< int >( "huVolume", TEXTURE_UNIT ).upload();

    /* Compute 'volume' scale in world space.
     */
    const base::math::Vector3f volumeScaleInWorld
        ( volume.geometry().worldTransform().col( 0 ).norm()
        , volume.geometry().worldTransform().col( 1 ).norm()
        , volume.geometry().worldTransform().col( 2 ).norm() );

    /* For each plane, process it's intersection with 'volume'.
     */
    const base::math::Matrix4f worldToVolumeModel = volume.geometry().worldTransform().inverse();
    base::math::Matrix4f scaledWorldToVolumeModel3x3
        = worldToVolumeModel * base::math::scaling4f( base::math::maxAbsElement( volumeScaleInWorld ) );
    scaledWorldToVolumeModel3x3.col( 3 ) = base::math::Vector4f( 0, 0, 0, 1 );
    pimpl->planes.rewind();
    while( !pimpl->planes.isEmpty() )
    {
        const base::Renderable& plane = pimpl->planes.poll();
        base::math::Vector4f planeNormal = plane.geometry().worldTransform().col( 2 );
        planeNormal.w() = 0;
        planeNormal.normalize();

        /* Shift plane origin along the plane s.t. it has minimum distance to the center of the volume.
         */
        const base::math::Vector4f volumeCenter = volume.geometry().worldTransform().col( 3 );
        const float planeVolumeCenterDistance =
               plane.geometry().worldTransform().col( 3 ).dot( planeNormal )
            - volume.geometry().worldTransform().col( 3 ).dot( planeNormal );
        const base::math::Vector4f planeWorldLocation = volumeCenter + planeNormal * planeVolumeCenterDistance;

        /* Compute transformation from plane tangent to the volume's model space.
         */
        base::math::Matrix4f planeWorldTransform3x3( plane.geometry().worldTransform() );
        planeWorldTransform3x3.col( 3 ) = base::math::Vector4f( 0, 0, 0, 1 );

        base::math::Matrix4f planeTangentModel( base::math::zeros< base::math::Matrix4f >() );
        planeTangentModel.col( 3 ) = worldToVolumeModel * planeWorldLocation;
        planeTangentModel += scaledWorldToVolumeModel3x3 * planeWorldTransform3x3;
        planeTangentModel( 3, 3 ) = 1;

        /* Upload remaining uniforms to shader.
        */
        base::ShaderUniform< base::math::Matrix4f >( "planeTangentModel", planeTangentModel ).upload();

        /* Draw the plane.
         */
        vr->planeMesh->render();
    }
}


void CuttingPlanesStage::renderPass
    ( const base::math::Matrix4f& vt
    , base::RenderTask& rt
    , const base::Viewport& vp )
{
    if( vr.get() == nullptr )
    {
        vr.reset( new VideoResources( base::ShaderManager::instance().acquireShader( "cutting_plane" ) ) );
    }
    
    rt.renderer.glContext().setShader( vr->shader );

    pimpl->renderTask = &rt;
    pimpl->viewPort = &vp;

    /* Configure proper GL state.
     */
    base::RenderState rs;
    rs.setCullFace( base::RenderState::cullNone );

    /* Set shader and upload all uniforms that are same for all planes and volumes.
     */
    base::ShaderUniform< float >( "minIntensity", minimumIntensity() ).upload();
    base::ShaderUniform< float >( "maxIntensity", maximumIntensity() ).upload();
    base::ShaderUniform<   int >(       "invert", pimpl->renderingInverse ? 1 : 0 ).upload();
    
    /* Set shader and do the rendering.
     */
    rt.renderer.glContext().setShader( vr->shader );
    base::GeometryStage< void >::renderPass( vt, rt, vp );

    /* There is no guarantee that 'renderTask' will be valid later.
     */
    pimpl->renderTask = nullptr;
}



}  // namespace Carna :: presets

}  // namespace Carna
