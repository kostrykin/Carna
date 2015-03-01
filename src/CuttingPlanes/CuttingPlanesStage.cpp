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

}; // CuttingPlanesStage :: Details


CuttingPlanesStage::Details::Details( int planeGeometryType )
    : planes( planeGeometryType )
    , renderTask( nullptr )
    , viewPort( nullptr )
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

}; // CuttingPlanesStage :: VideoResources


CuttingPlanesStage::VideoResources::VideoResources( const base::view::ShaderProgram& shader )
    : planeMesh( PlaneMesh::create( base::view::IndexBufferBase::PRIMITIVE_TYPE_TRIANGLE_FAN ) )
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

    planeMesh.vertexBuffer().copy( vertices, 4 );
    planeMesh. indexBuffer().copy( indices, 4 );
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


void CuttingPlanesStage::prepareFrame( base::view::Node& root )
{
    /* Find all planes that need to be rendered. We can set the 'viewTransform' matrix
     * to identity cause we don't need the planes to be sorted in any specific order.
     */
    pimpl->planes.build( root, base::math::identity4f() );
}


void CuttingPlanesStage::render( base::view::GLContext& glc, const base::view::Renderable& renderable )
{
    while( !pimpl->planes.isEmpty() )
    {
        const base::view::Geometry& plane = pimpl->planes.poll().geometry();
        // TODO: implement
    }
}


void CuttingPlanesStage::renderPass
    ( const base::math::Matrix4f& vt
    , base::view::RenderTask& rt
    , const base::view::Viewport& vp )
{
    if( vr.get() == nullptr )
    {
        vr.reset( new VideoResources( base::view::ShaderManager::instance().acquireShader( "huv-sampling" ) ) );
    }

    rt.renderer.glContext().setShader( vr->shader );

    pimpl->renderTask = &rt;
    pimpl->viewPort = &vp;
    pimpl->planes.rewind();
    
    /* Do the rendering.
     */
    base::view::GeometryStage< void >::renderPass( vt, rt, vp );

    /* There is no guarantee that 'renderTask' will be valid later.
     */
    pimpl->renderTask = nullptr;
}



}  // namespace Carna :: CuttingPlanes

}  // namespace Carna
