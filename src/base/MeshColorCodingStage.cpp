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

#include <Carna/base/MeshColorCodingStage.h>
#include <Carna/base/ShaderManager.h>
#include <Carna/base/ShaderUniform.h>
#include <Carna/base/GLContext.h>
#include <Carna/base/RenderTask.h>
#include <Carna/base/RenderTexture.h>
#include <Carna/base/Viewport.h>
#include <Carna/base/Framebuffer.h>
#include <Carna/base/Log.h>
#include <map>
#include <vector>
#include <climits>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// MeshColorCodingStage :: Details
// ----------------------------------------------------------------------------------

struct MeshColorCodingStage::Details
{

    const static unsigned int FIRST_COLOR_CODING_ID;
    const static unsigned int  LAST_COLOR_CODING_ID;
    const static        Color   NULL_GEOMETRY_COLOR;

    Details();

    static unsigned int colorToId( const Color& color );
    static Color idToColor( unsigned int id );

    unsigned int myActivationPassIndex;

    std::map< unsigned int, unsigned int > rolesByGeometryType;
    std::vector< const Geometry* > geometryById;
    unsigned int nextColorCodingId;

    RenderTask* renderTask;

    unsigned int vpOffsetX;
    unsigned int vpOffsetY;

}; // MeshColorCodingStage :: Details


const unsigned int MeshColorCodingStage::Details::FIRST_COLOR_CODING_ID( 0 );
const unsigned int MeshColorCodingStage::Details:: LAST_COLOR_CODING_ID( std::numeric_limits< unsigned int >::max() - 1 );
const        Color MeshColorCodingStage::Details::  NULL_GEOMETRY_COLOR( 0, 0, 0, 0 );


MeshColorCodingStage::Details::Details()
    : myActivationPassIndex( 0u )
    , nextColorCodingId( Details::FIRST_COLOR_CODING_ID )
{
}


unsigned int MeshColorCodingStage::Details::colorToId( const Color& color )
{
    unsigned int key = color.a;
    key |= color.b <<  8;
    key |= color.g << 16;
    key |= color.r << 24;
    CARNA_ASSERT( key >= FIRST_COLOR_CODING_ID + 1 );
    return key - 1;
}


Color MeshColorCodingStage::Details::idToColor( unsigned int id )
{
    CARNA_ASSERT( id <= LAST_COLOR_CODING_ID );
    const unsigned int key = id + 1;
    unsigned char a = static_cast< unsigned char >( key );
    unsigned char b = static_cast< unsigned char >( key >> 8 );
    unsigned char g = static_cast< unsigned char >( key >> 16 );
    unsigned char r = static_cast< unsigned char >( key >> 24 );
    return Color( r, g, b, a );
}



// ----------------------------------------------------------------------------------
// MeshColorCodingStage :: VideoResources
// ----------------------------------------------------------------------------------

struct MeshColorCodingStage::VideoResources
{

    VideoResources( const ShaderProgram& shader, unsigned int w, unsigned int h );

    const ShaderProgram& shader;
    RenderTexture rt;
    Framebuffer fbo;

}; // MeshColorCodingStage :: VideoResources


MeshColorCodingStage::VideoResources::VideoResources( const ShaderProgram& shader, unsigned int w, unsigned int h )
    : shader( shader )
    , rt( w, h )
    , fbo( rt )
{
}



// ----------------------------------------------------------------------------------
// MeshColorCodingStage
// ----------------------------------------------------------------------------------

MeshColorCodingStage::MeshColorCodingStage()
    : GeometryStage< void >::GeometryStage( 0, 0 )
    , pimpl( new Details() )
{
}


MeshColorCodingStage::~MeshColorCodingStage()
{
    clearGeometryTypes();
    if( vr.get() != nullptr )
    {
        activateGLContext();
        ShaderManager::instance().releaseShader( vr->shader );
    }
}


Aggregation< const Geometry > MeshColorCodingStage::pick( unsigned int x, unsigned int y ) const
{
    if( vr.get() == nullptr )
    {
        Log::instance().record( Log::warning, "MeshColorCodingStage::pick queried before frame was rendered." );
        return Aggregation< const Geometry >::NULL_PTR;
    }
    else
    {
        /* Apply viewport offset to picking coordinates.
         */
        if( x < pimpl->vpOffsetX || y < pimpl->vpOffsetY )
        {
            Log::instance().record( Log::debug, "MeshColorCodingStage::pick queried outside viewport." );
            return Aggregation< const Geometry >::NULL_PTR;
        }
        else
        {
            x = x - pimpl->vpOffsetX;
            y = y - pimpl->vpOffsetY;

            if( x >= vr->rt.width() || y >= vr->rt.height() )
            {
                Log::instance().record( Log::debug, "MeshColorCodingStage::pick queried outside viewport." );
                return Aggregation< const Geometry >::NULL_PTR;
            }
            else
            {
                y = vr->rt.height() - 1 - y;
                Framebuffer::MinimalBinding binding( vr->fbo );
                const Color color = binding.readPixel( x, y );
                if( color == Details::NULL_GEOMETRY_COLOR )
                {
                    return Aggregation< const Geometry >::NULL_PTR;
                }
                else
                {
                    const unsigned int id = Details::colorToId( color );
                    CARNA_ASSERT( id < pimpl->geometryById.size() );
                    return Aggregation< const Geometry >( *pimpl->geometryById[ id ] );
                }
            }
        }
    }
}


void MeshColorCodingStage::setActivationPassIndex( unsigned int activationPassIndex )
{
    pimpl->myActivationPassIndex = activationPassIndex;
}


unsigned int MeshColorCodingStage::activationPassIndex() const
{
    return pimpl->myActivationPassIndex;
}


void MeshColorCodingStage::renderPass( const math::Matrix4f& viewTransform, RenderTask& rt, const Viewport& vp )
{
    if( renderedPassesCount() == activationPassIndex() )
    {
        if( vr.get() == nullptr )
        {
            const ShaderProgram& shader = ShaderManager::instance().acquireShader( "unshaded" );
            vr.reset( new VideoResources( shader, vp.width, vp.height ) );
        }

        pimpl->vpOffsetX = vp.left;
        pimpl->vpOffsetY = vp.top;
        pimpl->nextColorCodingId = Details::FIRST_COLOR_CODING_ID;
        pimpl->geometryById.clear();
        pimpl->renderTask = &rt;

        /* Setup the shader.
         */
        rt.renderer.glContext().setShader( vr->shader );

        /* Do the rendering.
         */
        CARNA_RENDER_TO_FRAMEBUFFER( vr->fbo,
            rt.renderer.glContext().clearBuffers( GLContext::COLOR_BUFFER_BIT | GLContext::DEPTH_BUFFER_BIT );
            GeometryStage< void >::renderPass( viewTransform, rt, vp )
        );

        /* Denote that rendering is finished.
         */
        pimpl->renderTask = nullptr;
    }
}


void MeshColorCodingStage::render( GLContext& glc, const Renderable& renderable )
{
    const unsigned int geometryType = renderable.geometry().geometryType;
    const auto roleItr = pimpl->rolesByGeometryType.find( geometryType );
    if( roleItr != pimpl->rolesByGeometryType.end() )
    {
        /* Setup the shader.
         */
        ShaderUniform< math::Vector4f >( "color", Details::idToColor( pimpl->nextColorCodingId ) ).upload();
        ShaderUniform< math::Matrix4f >
            ( "modelViewProjection"
            , pimpl->renderTask->projection * renderable.modelViewTransform() ).upload();

        /* Do the rendering.
         */
        const MeshBase& mesh = static_cast< const MeshBase& >( renderable.geometry().feature( roleItr->second ) );
        mesh.render();

        /* Update rendering state.
         */
        CARNA_ASSERT( pimpl->nextColorCodingId == pimpl->geometryById.size() );
        pimpl->geometryById.push_back( &renderable.geometry() );
        if( pimpl->nextColorCodingId == Details::LAST_COLOR_CODING_ID )
        {
            Log::instance().record( Log::error, "Too many objects, color encoding failed." );
        }
        else
        {
            ++pimpl->nextColorCodingId;
        }
    }
}


void MeshColorCodingStage::reshape( const FrameRenderer& fr, const Viewport& vp )
{
    GeometryStage< void >::reshape( fr, vp );
    if( vr.get() != nullptr )
    {
        vr.reset( new VideoResources( vr->shader, vp.width, vp.height ) );
    }
}


void MeshColorCodingStage::putGeometryType( unsigned int geometryType, unsigned int role )
{
    removeGeometryType( geometryType );
    pimpl->rolesByGeometryType[ geometryType ] = role;
}


void MeshColorCodingStage::removeGeometryType( unsigned int geometryType )
{
    pimpl->rolesByGeometryType.erase( geometryType );
}


void MeshColorCodingStage::clearGeometryTypes()
{
    pimpl->rolesByGeometryType.clear();
}



}  // namespace Carna :: base

}  // namespace Carna
