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

#include <Carna/base/glew.h>
#include <Carna/base/Mesh.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// computeVertexStride
// ----------------------------------------------------------------------------------

static std::size_t computeVertexStride( const VertexAttributes& vertexAttributes )
{
    std::size_t componentsCount = 0;
    for( auto itr = vertexAttributes.begin(); itr != vertexAttributes.end(); ++itr )
    {
        componentsCount += itr->componentsCount;
    }
    return componentsCount * sizeof( float );
}


// ----------------------------------------------------------------------------------
// MeshBase :: VideoResourceAcquisition
// ----------------------------------------------------------------------------------

MeshBase::VideoResourceAcquisition::VideoResourceAcquisition
        ( GLContext& glc
        , MeshBase& mesh )
    : GeometryFeature::VideoResourceAcquisition( glc, mesh )
    , mesh( mesh )
{
    if( mesh.videoResourceAcquisitionsCount() == 1 )
    {
        /* Upload the vertices and indices to video memory.
         */
        mesh.vertexBuffer.reset( mesh.loadVertexBuffer() );
        mesh. indexBuffer.reset( mesh.loadIndexBuffer () );
         
        /* Create vertex array.
         */
        glGenVertexArrays( 1, &mesh.id );
        glBindVertexArray( mesh.id );
        vertexBuffer().bind();
        std::size_t offset = 0;
        const std::size_t stride = computeVertexStride( mesh.vertexAttributes );
        for( auto itr = mesh.vertexAttributes.begin(); itr != mesh.vertexAttributes.end(); ++itr )
        {
            glEnableVertexAttribArray( itr->position );
            glVertexAttribPointer
                ( itr->position
                , itr->componentsCount
                , itr->type
                , false
                , stride
                , static_cast< float* >( nullptr ) + offset );
            offset += itr->componentsCount;
        }
        glBindVertexArray( 0 );
    }
}


MeshBase::VideoResourceAcquisition::~VideoResourceAcquisition()
{
    if( mesh.videoResourceAcquisitionsCount() == 1 )
    {
        /* Delete mesh from video memory.
         */
        glDeleteVertexArrays( 1, &mesh.id );
        mesh.id = 0;
    }
}


void MeshBase::VideoResourceAcquisition::bind() const
{
    glBindVertexArray( mesh.id );
}


void MeshBase::VideoResourceAcquisition::render() const
{
    CARNA_ASSERT_EX( vertexBuffer().isValid(), "Vertex buffer is invalid." );
    CARNA_ASSERT_EX(  indexBuffer().isValid(),  "Index buffer is invalid." );

    glBindVertexArray( mesh.id );
    indexBuffer().bind();
    glDrawElements( indexBuffer().primitiveType, indexBuffer().size(), indexBuffer().type, nullptr );
}


const VertexBufferBase& MeshBase::VideoResourceAcquisition::vertexBuffer() const
{
    return *mesh.vertexBuffer;
}


const IndexBufferBase& MeshBase::VideoResourceAcquisition::indexBuffer() const
{
    return *mesh.indexBuffer;
}


VertexBufferBase& MeshBase::VideoResourceAcquisition::vertexBuffer()
{
    return *mesh.vertexBuffer;
}


IndexBufferBase& MeshBase::VideoResourceAcquisition::indexBuffer()
{
    return *mesh.indexBuffer;
}



// ----------------------------------------------------------------------------------
// MeshBase
// ----------------------------------------------------------------------------------

MeshBase::MeshBase( unsigned int primitiveType, const VertexAttributes& va )
    : primitiveType( primitiveType )
    , vertexAttributes( va )
{
}


MeshBase::~MeshBase()
{
}


bool MeshBase::controlsSameVideoResource( const GeometryFeature& ) const
{
    return false;
}


MeshBase::VideoResourceAcquisition* MeshBase::acquireVideoResource( GLContext& glc )
{
    return new VideoResourceAcquisition( glc, *this );
}



}  // namespace Carna :: base

}  // namespace Carna
