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

#include <Carna/base/view/glew.h>
#include <Carna/base/view/Mesh.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// MeshBase
// ----------------------------------------------------------------------------------

static unsigned int createGLVertexArray()
{
    unsigned int id;
    glGenVertexArrays( 1, &id );
    return id;
}


static std::size_t computeVertexStride( const VertexAttributes& vertexAttributes )
{
    std::size_t componentsCount = 0;
    for( auto itr = vertexAttributes.begin(); itr != vertexAttributes.end(); ++itr )
    {
        componentsCount += itr->componentsCount;
    }
    return componentsCount * sizeof( float );
}


MeshBase::MeshBase( VertexBufferBase* vertex_buffer, IndexBufferBase* index_buffer, const VertexAttributes& vertexAttributes )
    : id( createGLVertexArray() )
    , myVertexBuffer( vertex_buffer )
    , myIndexBuffer( index_buffer )
{
    glBindVertexArray( id );
    vertexBuffer().bind();

    std::size_t offset = 0;
    const std::size_t stride = computeVertexStride( vertexAttributes );
    for( auto itr = vertexAttributes.begin(); itr != vertexAttributes.end(); ++itr )
    {
        glEnableVertexAttribArray( itr->position );
        glVertexAttribPointer( itr->position, itr->componentsCount, GL_FLOAT, true, stride, static_cast< float* >( nullptr ) + offset );
        offset += itr->componentsCount;
    }

    glBindVertexArray( 0 );
}


MeshBase::~MeshBase()
{
    glDeleteVertexArrays( 1, &id );
}


void MeshBase::bind() const
{
    glBindVertexArray( id );
}


void MeshBase::render() const
{
    CARNA_ASSERT_EX( vertexBuffer().isValid(), "Vertex buffer is invalid." );
    CARNA_ASSERT_EX(  indexBuffer().isValid(),  "Index buffer is invalid." );

    glBindVertexArray( id );
    indexBuffer().bind();
    glDrawElements( indexBuffer().primitiveType, indexBuffer().size(), indexBuffer().type, nullptr );
}


const VertexBufferBase& MeshBase::vertexBuffer() const
{
    return *myVertexBuffer;
}


const IndexBufferBase& MeshBase::indexBuffer() const
{
    return *myIndexBuffer;
}


VertexBufferBase& MeshBase::vertexBuffer()
{
    return *myVertexBuffer;
}


IndexBufferBase& MeshBase::indexBuffer()
{
    return *myIndexBuffer;
}


bool MeshBase::controlsSameVideoResource( const GeometryFeature& ) const
{
    return false;
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
