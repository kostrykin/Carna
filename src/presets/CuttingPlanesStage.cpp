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

    ~Details();
    
    typedef base::Mesh< base::VertexBase, uint8_t > PlaneMesh;
    PlaneMesh& planeMesh;

    base::RenderQueue< void > planes;

    base::RenderTask* renderTask;
    const base::Viewport* viewPort;

    base::HUV windowingLevel;
    unsigned int windowingWidth;
    bool renderingIvnerse;

    static PlaneMesh& createPlaneMesh();

    static inline float huvToIntensity( base::HUV huv )
    {
        return ( huv + 1024 ) / 4095.f;
    }

}; // CuttingPlanesStage :: Details


CuttingPlanesStage::Details::PlaneMesh& CuttingPlanesStage::Details::createPlaneMesh()
{
    const float radius = std::sqrt( 3.f ) / 2;
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

    return PlaneMesh::create( base::IndexBufferBase::PRIMITIVE_TYPE_TRIANGLE_FAN, vertices, 4, indices, 4 );
}


CuttingPlanesStage::Details::Details( unsigned int planeGeometryType )
    : planeMesh( createPlaneMesh() )
    , planes( planeGeometryType )
    , renderTask( nullptr )
    , viewPort( nullptr )
    , windowingLevel( DEFAULT_WINDOWING_LEVEL )
    , windowingWidth( DEFAULT_WINDOWING_WIDTH )
    , renderingIvnerse( false )
{
}


CuttingPlanesStage::Details::~Details()
{
    planeMesh.release();
}



// ----------------------------------------------------------------------------------
// CuttingPlanesStage :: VideoResources
// ----------------------------------------------------------------------------------

struct CuttingPlanesStage::VideoResources
{

    VideoResources( const base::ShaderProgram& shader, Details::PlaneMesh& planeMesh );

    Details::PlaneMesh::VideoResourceAcquisition planeMeshVR;
    const base::ShaderProgram& shader;
    base::Sampler volumeSampler;

}; // CuttingPlanesStage :: VideoResources


CuttingPlanesStage::VideoResources::VideoResources( const base::ShaderProgram& shader, Details::PlaneMesh& planeMesh )
    : planeMeshVR( planeMesh )
    , shader( shader )
{
    /* Configure volume volumeSampler.
     */
    volumeSampler.setMinFilter( base::Sampler::FILTER_LINEAR );
    volumeSampler.setMagFilter( base::Sampler::FILTER_LINEAR );
    volumeSampler.setWrapModeR( base::Sampler::WRAP_MODE_CLAMP );
    volumeSampler.setWrapModeS( base::Sampler::WRAP_MODE_CLAMP );
    volumeSampler.setWrapModeT( base::Sampler::WRAP_MODE_CLAMP );
}



// ----------------------------------------------------------------------------------
// CuttingPlanesStage
// ----------------------------------------------------------------------------------

const unsigned int CuttingPlanesStage::DEFAULT_WINDOWING_WIDTH = 4096;
const    base::HUV CuttingPlanesStage::DEFAULT_WINDOWING_LEVEL = static_cast< base::HUV >( CuttingPlanesStage::DEFAULT_WINDOWING_WIDTH / 2 ) - 1024;
const unsigned int CuttingPlanesStage::ROLE_HU_VOLUME = 0;


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


void CuttingPlanesStage::setWindowingLevel( base::HUV windowingLevel )
{
    pimpl->windowingLevel = windowingLevel;
}


void CuttingPlanesStage::setWindowingWidth( unsigned int windowingWidth )
{
    pimpl->windowingWidth = windowingWidth;
}


void CuttingPlanesStage::setRenderingInverse( bool inverse )
{
    pimpl->renderingIvnerse = inverse;
}


base::HUV CuttingPlanesStage::windowingLevel() const
{
    return pimpl->windowingLevel;
}


unsigned int CuttingPlanesStage::windowingWidth() const
{
    return pimpl->windowingWidth;
}


base::HUV CuttingPlanesStage::minimumHUV() const
{
    return static_cast< base::HUV >( pimpl->windowingLevel - static_cast< int >( pimpl->windowingWidth ) );
}


base::HUV CuttingPlanesStage::maximumHUV() const
{
    return static_cast< base::HUV >( pimpl->windowingLevel + static_cast< int >( pimpl->windowingWidth ) );
}


bool CuttingPlanesStage::isRenderingInverse() const
{
    return pimpl->renderingIvnerse;
}


void CuttingPlanesStage::updateRenderQueues( base::Node& root, const base::math::Matrix4f& vt, bool vtTriggered )
{
    /* Since we do not require the renderables to be sorted by any
     * specific order within the render queue, we also do not have to
     * rebuild the render queue when the 'view transformation' changes.
     * It's absolutely sufficient to rewind the render queue then.
     */
    if( vtTriggered )
    {
        rewindRenderQueues();
    }
    else
    {
        GeometryStage< void >::updateRenderQueues( root, vt, vtTriggered );
        pimpl->planes.build( root, vt );
    }
}


void CuttingPlanesStage::render( const base::Renderable& volume )
{
    /* Bind texture and volumeSampler to free texture unit.
     */
    const static unsigned int TEXTURE_UNIT = base::Texture3D::SETUP_UNIT + 1;
    const base::Texture3D& texture = static_cast< const base::Texture3D& >( volume.geometry().feature( ROLE_HU_VOLUME ) );
    vr->volumeSampler.bind( TEXTURE_UNIT );
    this->videoResource( texture ).bind( TEXTURE_UNIT );
    
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

        base::math::Matrix4f planeTangentModel( base::math::zeros4f() );
        planeTangentModel.col( 3 ) = worldToVolumeModel * planeWorldLocation;
        planeTangentModel += scaledWorldToVolumeModel3x3 * planeWorldTransform3x3;
        planeTangentModel( 3, 3 ) = 1;

        /* Upload remaining uniforms to shader.
        */
        base::ShaderUniform< base::math::Matrix4f >( "planeTangentModel", planeTangentModel ).upload();

        /* Draw the plane.
         */
        vr->planeMeshVR.render();
    }
}


void CuttingPlanesStage::renderPass
    ( const base::math::Matrix4f& vt
    , base::RenderTask& rt
    , const base::Viewport& vp )
{
    if( vr.get() == nullptr )
    {
        vr.reset( new VideoResources( base::ShaderManager::instance().acquireShader( "cutting_plane" ), pimpl->planeMesh ) );
    }
    
    rt.renderer.glContext().setShader( vr->shader );

    pimpl->renderTask = &rt;
    pimpl->viewPort = &vp;

    /* Configure proper GL state.
     */
    base::RenderState rs( rt.renderer.glContext() );
    rs.setCullFace( base::RenderState::cullNone );

    /* Set shader and upload all uniforms that are same for all planes and volumes.
     */
    base::ShaderUniform< float >( "minIntensity", Details::huvToIntensity( minimumHUV() ) ).upload();
    base::ShaderUniform< float >( "maxIntensity", Details::huvToIntensity( maximumHUV() ) ).upload();
    base::ShaderUniform<   int >(       "invert", pimpl->renderingIvnerse ? 1 : 0 ).upload();
    
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
