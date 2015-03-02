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

#include <Carna/CuttingPlanes/CuttingPlanesStage.h>
#include <Carna/base/view/Mesh.h>
#include <Carna/base/view/Vertex.h>
#include <Carna/base/view/IndexBuffer.h>
#include <Carna/base/view/ShaderManager.h>
#include <Carna/base/view/ShaderUniform.h>
#include <Carna/base/view/RenderState.h>
#include <Carna/base/math.h>

namespace Carna
{

namespace CuttingPlanes
{



// ----------------------------------------------------------------------------------
// CuttingPlanesStage :: Details
// ----------------------------------------------------------------------------------

struct CuttingPlanesStage::Details
{

    Details( int planeGeometryType );

    base::view::RenderQueue< void > planes;

    base::view::RenderTask* renderTask;
    const base::view::Viewport* viewPort;

    base::HUV windowingLevel;
    unsigned int windowingWidth;

    static inline float huvToIntensity( base::HUV huv )
    {
        return ( huv + 1024 ) / 4095.f;
    }

}; // CuttingPlanesStage :: Details


CuttingPlanesStage::Details::Details( int planeGeometryType )
    : planes( planeGeometryType )
    , renderTask( nullptr )
    , viewPort( nullptr )
    , windowingLevel( DEFAULT_WINDOWING_LEVEL )
    , windowingWidth( DEFAULT_WINDOWING_WIDTH )
{
}



// ----------------------------------------------------------------------------------
// CuttingPlanesStage :: VideoResources
// ----------------------------------------------------------------------------------

struct CuttingPlanesStage::VideoResources
{

    VideoResources( const base::view::ShaderProgram& shader );

    ~VideoResources();

    typedef base::view::Mesh< base::view::VertexBase, uint8_t > PlaneMesh;

    PlaneMesh& planeMesh;
    const base::view::ShaderProgram& shader;
    base::view::Sampler volumeSampler;

}; // CuttingPlanesStage :: VideoResources


CuttingPlanesStage::VideoResources::VideoResources( const base::view::ShaderProgram& shader )
    : planeMesh( PlaneMesh::create( base::view::IndexBufferBase::PRIMITIVE_TYPE_TRIANGLE_FAN ) )
    , shader( shader )
{
    /* Create plane mesh.
     */
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

    planeMesh.vertexBuffer().copy( vertices, 4 );
    planeMesh. indexBuffer().copy( indices, 4 );

    /* Configure volume volumeSampler.
     */
    volumeSampler.setMinFilter( base::view::Sampler::FILTER_LINEAR );
    volumeSampler.setMagFilter( base::view::Sampler::FILTER_LINEAR );
    volumeSampler.setWrapModeR( base::view::Sampler::WRAP_MODE_CLAMP );
    volumeSampler.setWrapModeS( base::view::Sampler::WRAP_MODE_CLAMP );
    volumeSampler.setWrapModeT( base::view::Sampler::WRAP_MODE_CLAMP );
}


CuttingPlanesStage::VideoResources::~VideoResources()
{
    planeMesh.release();
}



// ----------------------------------------------------------------------------------
// CuttingPlanesStage
// ----------------------------------------------------------------------------------

CuttingPlanesStage::CuttingPlanesStage( int volumeGeometryType, int planeGeometryType )
    : base::view::GeometryStage< void >::GeometryStage( volumeGeometryType )
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
        base::view::ShaderManager::instance().releaseShader( vr->shader );
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


void CuttingPlanesStage::updateRenderQueues( base::view::Node& root, const base::math::Matrix4f& vt, bool vtTriggered )
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


void CuttingPlanesStage::render( base::view::GLContext& glc, const base::view::Renderable& volume )
{
    /* Bind texture and volumeSampler to free texture unit.
     */
    const static unsigned int TEXTURE_UNIT = base::view::Texture3D::SETUP_UNIT + 1;
    const base::view::Texture3D& texture = static_cast< const base::view::Texture3D& >( volume.geometry().feature( ROLE_HU_VOLUME ) );
    vr->volumeSampler.bind( TEXTURE_UNIT );
    texture.bind( TEXTURE_UNIT );
    
    /* Upload volume-specific uniforms that are equal for all planes.
     */
    const base::math::Matrix4f modelViewProjection = pimpl->renderTask->projection * volume.modelViewTransform();
    base::view::ShaderUniform< base::math::Matrix4f >( "modelViewProjection", modelViewProjection ).upload();
    base::view::ShaderUniform< int >( "huVolume", TEXTURE_UNIT ).upload();

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
        const base::view::Renderable& plane = pimpl->planes.poll();
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
        base::view::ShaderUniform< base::math::Matrix4f >( "planeTangentModel", planeTangentModel ).upload();

        /* Draw the plane.
         */
        vr->planeMesh.render();
    }
}


void CuttingPlanesStage::renderPass
    ( const base::math::Matrix4f& vt
    , base::view::RenderTask& rt
    , const base::view::Viewport& vp )
{
    if( vr.get() == nullptr )
    {
        vr.reset( new VideoResources( base::view::ShaderManager::instance().acquireShader( "cutting_plane" ) ) );
    }
    
    rt.renderer.glContext().setShader( vr->shader );

    pimpl->renderTask = &rt;
    pimpl->viewPort = &vp;

    /* Configure proper GL state.
     */
    base::view::RenderState rs( rt.renderer.glContext() );
    rs.setCullFace( base::view::RenderState::cullNone );

    /* Set shader and upload all uniforms that are same for all planes and volumes.
     */
    base::view::ShaderUniform< float >( "minIntensity", Details::huvToIntensity( minimumHUV() ) ).upload();
    base::view::ShaderUniform< float >( "maxIntensity", Details::huvToIntensity( maximumHUV() ) ).upload();
    
    /* Set shader and do the rendering.
     */
    rt.renderer.glContext().setShader( vr->shader );
    base::view::GeometryStage< void >::renderPass( vt, rt, vp );

    /* There is no guarantee that 'renderTask' will be valid later.
     */
    pimpl->renderTask = nullptr;
}



}  // namespace Carna :: CuttingPlanes

}  // namespace Carna
