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

#include <Carna/presets/MeshColorCodingStage.h>
#include <Carna/base/ShaderManager.h>
#include <Carna/base/ShaderUniform.h>
#include <Carna/base/GLContext.h>
#include <Carna/base/RenderTask.h>
#include <Carna/base/Viewport.h>
#include <Carna/base/Framebuffer.h>
#include <Carna/base/Log.h>
#include <map>
#include <vector>
#include <climits>

namespace Carna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// MeshColorCodingStage :: Details
// ----------------------------------------------------------------------------------

struct MeshColorCodingStage::Details
{

    const static unsigned int FIRST_COLOR_CODING_ID;
    const static unsigned int  LAST_COLOR_CODING_ID;
    const static  base::Color   NULL_GEOMETRY_COLOR;

    Details();

    static unsigned int colorToId( const base::Color& color );
    static base::Color idToColor( unsigned int id );

    unsigned int myActivationPassIndex;

    std::map< unsigned int, unsigned int > meshRolesByGeometryType;
    std::map< unsigned int, unsigned int > materialRolesByGeometryType;
    std::vector< const base::Geometry* > geometryById;
    unsigned int nextColorCodingId;

    base::RenderTask* renderTask;

    unsigned int vpOffsetX;
    unsigned int vpOffsetY;

}; // MeshColorCodingStage :: Details


const unsigned int MeshColorCodingStage::Details::FIRST_COLOR_CODING_ID( 0 );
const unsigned int MeshColorCodingStage::Details:: LAST_COLOR_CODING_ID( std::numeric_limits< unsigned int >::max() - 1 );
const  base::Color MeshColorCodingStage::Details::  NULL_GEOMETRY_COLOR( 0, 0, 0, 0 );


MeshColorCodingStage::Details::Details()
    : myActivationPassIndex( 0u )
    , nextColorCodingId( Details::FIRST_COLOR_CODING_ID )
{
}


unsigned int MeshColorCodingStage::Details::colorToId( const base::Color& color )
{
    unsigned int key = color.a;
    key |= color.b <<  8;
    key |= color.g << 16;
    key |= color.r << 24;
    CARNA_ASSERT( key >= FIRST_COLOR_CODING_ID + 1 );
    return key - 1;
}


base::Color MeshColorCodingStage::Details::idToColor( unsigned int id )
{
    CARNA_ASSERT( id <= LAST_COLOR_CODING_ID );
    const unsigned int key = id + 1;
    unsigned char a = static_cast< unsigned char >( key );
    unsigned char b = static_cast< unsigned char >( key >> 8 );
    unsigned char g = static_cast< unsigned char >( key >> 16 );
    unsigned char r = static_cast< unsigned char >( key >> 24 );
    return base::Color( r, g, b, a );
}



// ----------------------------------------------------------------------------------
// MeshColorCodingStage :: VideoResources
// ----------------------------------------------------------------------------------

struct MeshColorCodingStage::VideoResources
{

    VideoResources( const base::ShaderProgram& shader, unsigned int w, unsigned int h );

    const base::ShaderProgram& shader;
    std::unique_ptr< base::Texture< 2 > > renderTexture;
    base::Framebuffer fbo;

}; // MeshColorCodingStage :: VideoResources


MeshColorCodingStage::VideoResources::VideoResources( const base::ShaderProgram& shader, unsigned int w, unsigned int h )
    : shader( shader )
    , renderTexture( base::Framebuffer::createRenderTexture() )
    , fbo( w, h, *renderTexture )
{
}



// ----------------------------------------------------------------------------------
// MeshColorCodingStage
// ----------------------------------------------------------------------------------

MeshColorCodingStage::MeshColorCodingStage()
    : base::GeometryStage< void >::GeometryStage( 0, 0 )
    , pimpl( new Details() )
{
}


MeshColorCodingStage::~MeshColorCodingStage()
{
    if( vr.get() != nullptr )
    {
        activateGLContext();
        base::ShaderManager::instance().releaseShader( vr->shader );
    }
}


MeshColorCodingStage* MeshColorCodingStage::clone() const
{
    MeshColorCodingStage* const result = new MeshColorCodingStage();
    result->setEnabled( isEnabled() );
    return result;
}


base::Aggregation< const base::Geometry > MeshColorCodingStage::pick( const base::math::Vector2ui& v ) const
{
    return pick( v.x(), v.y() );
}


base::Aggregation< const base::Geometry > MeshColorCodingStage::pick( unsigned int x, unsigned int y ) const
{
    using namespace base;
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

            if( x >= vr->fbo.width() || y >= vr->fbo.height() )
            {
                Log::instance().record( Log::debug, "MeshColorCodingStage::pick queried outside viewport." );
                return Aggregation< const Geometry >::NULL_PTR;
            }
            else
            {
                y = vr->fbo.height() - 1 - y;
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


void MeshColorCodingStage::renderPass( const base::math::Matrix4f& viewTransform, base::RenderTask& rt, const base::Viewport& vp )
{
    using namespace base;
    if( renderedPassesCount() == activationPassIndex() )
    {
        if( vr.get() == nullptr )
        {
            const ShaderProgram& shader = ShaderManager::instance().acquireShader( "unshaded" );
            vr.reset( new VideoResources( shader, vp.width(), vp.height() ) );
        }

        pimpl->vpOffsetX = vp.marginLeft();
        pimpl->vpOffsetY = vp.marginTop();
        pimpl->nextColorCodingId = Details::FIRST_COLOR_CODING_ID;
        pimpl->geometryById.clear();
        pimpl->renderTask = &rt;

        /* Setup the shader.
         */
        rt.renderer.glContext().setShader( vr->shader );

        /* Do the rendering.
         */
        Viewport fboViewport( vp, 0, 0, vr->fbo.width(), vr->fbo.height() );
        fboViewport.makeActive();
        CARNA_RENDER_TO_FRAMEBUFFER( vr->fbo,
            rt.renderer.glContext().clearBuffers( GLContext::COLOR_BUFFER_BIT | GLContext::DEPTH_BUFFER_BIT );
            GeometryStage< void >::renderPass( viewTransform, rt, vp );
        );
        fboViewport.done();

        /* Denote that rendering is finished.
         */
        pimpl->renderTask = nullptr;
    }
}


void MeshColorCodingStage::render( const base::Renderable& renderable )
{
    using namespace base;
    const unsigned int geometryType = renderable.geometry().geometryType;
    const auto meshRoleItr = pimpl->meshRolesByGeometryType.find( geometryType );
    if( meshRoleItr != pimpl->meshRolesByGeometryType.end() )
    {
        /* Lookup whether materials are enabled for this geometry type.
         */
        const auto materialRoleItr = pimpl->materialRolesByGeometryType.find( geometryType );
        const bool materialEnabled = materialRoleItr != pimpl->materialRolesByGeometryType.end();
        
        /* Check whether the mesh is 'GL_POINTS' typed and setup 'glPointSize' than.
         */
        RenderState rs;
        const ManagedMeshBase& mesh = static_cast< const ManagedMeshBase& >( renderable.geometry().feature( meshRoleItr->second ) );
        if( materialEnabled && mesh.primitiveType == IndexBufferBase::PRIMITIVE_TYPE_POINTS
            && renderable.geometry().hasFeature( materialRoleItr->second ) )
        {
            const static std::string PARAMETER_POINT_SIZE = "pointSize";
            const Material& material = static_cast< const Material& >( renderable.geometry().feature( materialRoleItr->second ) );
            if( material.hasParameter( PARAMETER_POINT_SIZE ) )
            {
                const ShaderUniformBase& uniformBase = material.parameter( PARAMETER_POINT_SIZE );
                const ShaderUniform< float > uniform = static_cast< const ShaderUniform< float >& >( uniformBase );
                const float pointSize = uniform.value;
                rs.setPointSize( pointSize );
            }
        }
    
        /* Setup the shader.
         */
        ShaderUniform< math::Vector4f >( "color", Details::idToColor( pimpl->nextColorCodingId ) ).upload();
        ShaderUniform< math::Matrix4f >
            ( "modelViewProjection"
            , pimpl->renderTask->projection * renderable.modelViewTransform() ).upload();

        /* Do the rendering.
         */
        this->videoResource( mesh ).get().render();

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


void MeshColorCodingStage::reshape( base::FrameRenderer& fr, unsigned int width, unsigned int height )
{
    base::RenderStage::reshape( fr, width, height );
    if( vr.get() != nullptr )
    {
        vr.reset( new VideoResources( vr->shader, width, height ) );
    }
}


void MeshColorCodingStage::putGeometryType( unsigned int geometryType, unsigned int meshRole )
{
    removeGeometryType( geometryType );
    pimpl->meshRolesByGeometryType[ geometryType ] = meshRole;
}


void MeshColorCodingStage::enableMaterials( unsigned int geometryType, unsigned int materialRole )
{
    pimpl->materialRolesByGeometryType[ geometryType ] = materialRole;
}


void MeshColorCodingStage::removeGeometryType( unsigned int geometryType )
{
    pimpl->meshRolesByGeometryType.erase( geometryType );
    pimpl->materialRolesByGeometryType.erase( geometryType );
}


void MeshColorCodingStage::clearGeometryTypes()
{
    pimpl->meshRolesByGeometryType.clear();
    pimpl->materialRolesByGeometryType.clear();
}



}  // namespace Carna :: presets

}  // namespace Carna
