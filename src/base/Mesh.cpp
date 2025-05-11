/*
 *  Copyright (C) 2010 - 2016 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 * 
 * 
 *  Copyright (C) 2021 - 2025 Leonid Kostrykin
 * 
 */

#include <LibCarna/base/glew.hpp>
#include <LibCarna/base/Mesh.hpp>

namespace LibCarna
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
// createGLVertexArray
// ----------------------------------------------------------------------------------

static unsigned int createGLVertexArray( const VertexAttributes& va, const VertexBufferBase& vertexBuffer )
{
    /* Create new vertex array.
     */
    unsigned int id;
    glGenVertexArrays( 1, &id );
    
    /* Configure vertex array.
     */
    glBindVertexArray( id );
    vertexBuffer.bind();
    std::size_t offset = 0;
    const std::size_t stride = computeVertexStride( va );
    for( auto itr = va.begin(); itr != va.end(); ++itr )
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
    REPORT_GL_ERROR;
    glBindVertexArray( 0 );
    return id;
}



// ----------------------------------------------------------------------------------
// MeshBase
// ----------------------------------------------------------------------------------

MeshBase::MeshBase
        ( const VertexAttributes& va
        , Association< VertexBufferBase >* vertexBuffer
        , Association<  IndexBufferBase >*  indexBuffer )
    : myVertexBuffer( vertexBuffer )
    , myIndexBuffer( indexBuffer )
    , id( createGLVertexArray( va, **vertexBuffer ) )
    , glContext( GLContext::current() )
{
}


MeshBase::~MeshBase() noexcept( false )
{
    LIBCARNA_ASSERT( &GLContext::current() == &glContext );
    glDeleteVertexArrays( 1, &id );
}


void MeshBase::bind() const
{
    LIBCARNA_ASSERT( &GLContext::current() == &glContext );
    glBindVertexArray( id );
}


void MeshBase::render() const
{
    LIBCARNA_ASSERT( &GLContext::current() == &glContext );
    
    LIBCARNA_ASSERT_EX( vertexBuffer().isValid(), "Vertex buffer is invalid." );
    LIBCARNA_ASSERT_EX(  indexBuffer().isValid(),  "Index buffer is invalid." );

    this->bind();
    indexBuffer().bind();
    glDrawElements( indexBuffer().primitiveType, indexBuffer().size(), indexBuffer().type, nullptr );
}


const VertexBufferBase& MeshBase::vertexBuffer() const
{
    return **myVertexBuffer;
}


const IndexBufferBase& MeshBase::indexBuffer() const
{
    return **myIndexBuffer;
}


VertexBufferBase& MeshBase::vertexBuffer()
{
    return **myVertexBuffer;
}


IndexBufferBase& MeshBase::indexBuffer()
{
    return **myIndexBuffer;
}



}  // namespace LibCarna :: base

}  // namespace LibCarna
