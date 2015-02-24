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

#ifndef MESH_H_6014714286
#define MESH_H_6014714286

#include <Carna/base/view/VertexAttributes.h>
#include <Carna/base/view/VertexBuffer.h>
#include <Carna/base/view/IndexBuffer.h>
#include <memory>
#include <vector>

/** \file   Mesh.h
  * \brief  Defines \ref Carna::base::view::Mesh.
  */

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// MeshBase
// ----------------------------------------------------------------------------------

class MeshBase
{

    const std::unique_ptr< VertexBufferBase > myVertexBuffer;
    const std::unique_ptr< IndexBufferBase > myIndexBuffer;

protected:

    MeshBase( VertexBufferBase* vb, IndexBufferBase* ib, const VertexAttributes& va );

public:

    ~MeshBase();

    const unsigned int id;

    void bind() const;

    void render() const;

    const VertexBufferBase& vertexBuffer() const;

    const IndexBufferBase& indexBuffer() const;

    VertexBufferBase& vertexBuffer();

    IndexBufferBase& indexBuffer();

}; // MeshBase



// ----------------------------------------------------------------------------------
// Mesh
// ----------------------------------------------------------------------------------

template< typename Vertex, typename Index >
class Mesh : public MeshBase
{

public:

    typedef typename Vertex Vertex;

    typedef typename Index Index;

    Mesh( unsigned int primitiveType );

    const VertexBuffer< Vertex >& vertexBuffer() const;

    const IndexBuffer< Index >& indexBuffer() const;

    VertexBuffer< Vertex >& vertexBuffer();

    IndexBuffer< Index >& indexBuffer();

}; // Mesh


template< typename Vertex, typename Index >
Mesh< Vertex, Index >::Mesh( unsigned int primitiveType )
    : MeshBase( new VertexBuffer< Vertex >(), new IndexBuffer< Index >( primitiveType ), Vertex::attributes )
{
}


template< typename Vertex, typename Index >
const VertexBuffer< Vertex >& Mesh< Vertex, Index >::vertexBuffer() const
{
    return static_cast< const VertexBuffer< Vertex >& >( MeshBase::vertexBuffer() );
}


template< typename Vertex, typename Index >
const IndexBuffer< Index >& Mesh< Vertex, Index >::indexBuffer() const
{
    return static_cast< const IndexBuffer< Index >& >( MeshBase::indexBuffer() );
}


template< typename Vertex, typename Index >
VertexBuffer< Vertex >& Mesh< Vertex, Index >::vertexBuffer()
{
    return static_cast< VertexBuffer< Vertex >& >( MeshBase::vertexBuffer() );
}


template< typename Vertex, typename Index >
IndexBuffer< Index >& Mesh< Vertex, Index >::indexBuffer()
{
    return static_cast< IndexBuffer< Index >& >( MeshBase::indexBuffer() );
}



}  // namespace Carna :: base view

}  // namespace Carna :: base

}  // namespace Carna

#endif // MESH_H_6014714286
