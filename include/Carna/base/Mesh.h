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

#include <Carna/base/VertexAttributes.h>
#include <Carna/base/VertexBuffer.h>
#include <Carna/base/IndexBuffer.h>
#include <Carna/base/GeometryFeature.h>
#include <memory>
#include <vector>

/** \file   Mesh.h
  * \brief  Defines \ref Carna::base::Mesh.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// MeshBase
// ----------------------------------------------------------------------------------

/** \brief
  * Format-independent \ref Mesh base class.
  *
  * \author Leonid Kostrykin
  * \date   22.2.15 - 10.3.15
  */
class CARNA_LIB MeshBase : public GeometryFeature
{

    NON_COPYABLE

    const std::unique_ptr< VertexBufferBase > myVertexBuffer;
    const std::unique_ptr< IndexBufferBase > myIndexBuffer;

protected:

    friend class GeometryFeature;

    MeshBase( VertexBufferBase* vb, IndexBufferBase* ib, const VertexAttributes& va );

    virtual ~MeshBase();

public:

    const unsigned int id;

    void bind() const;

    void render() const;

    const VertexBufferBase& vertexBuffer() const;

    const IndexBufferBase& indexBuffer() const;

    VertexBufferBase& vertexBuffer();

    IndexBufferBase& indexBuffer();

    virtual bool controlsSameVideoResource( const GeometryFeature& ) const override;

}; // MeshBase



// ----------------------------------------------------------------------------------
// Mesh
// ----------------------------------------------------------------------------------

template< typename VertexType, typename IndexType >
class Mesh : public MeshBase
{

    Mesh( unsigned int primitiveType );

public:

    typedef VertexType Vertex;

    typedef IndexType Index;

    const VertexBuffer< VertexType >& vertexBuffer() const;

    const IndexBuffer< IndexType >& indexBuffer() const;

    VertexBuffer< VertexType >& vertexBuffer();

    IndexBuffer< IndexType >& indexBuffer();

    /** \brief
      * Instantiates. Call \ref release when you do not need the object any longer.
      */
    static Mesh< VertexType, IndexType >& create( unsigned int primitiveType );

}; // Mesh


template< typename VertexType, typename IndexType >
Mesh< VertexType, IndexType >::Mesh( unsigned int primitiveType )
    : MeshBase( new VertexBuffer< Vertex >(), new IndexBuffer< Index >( primitiveType ), Vertex::attributes )
{
}


template< typename VertexType, typename IndexType >
const VertexBuffer< VertexType >& Mesh< VertexType, IndexType >::vertexBuffer() const
{
    return static_cast< const VertexBuffer< Vertex >& >( MeshBase::vertexBuffer() );
}


template< typename VertexType, typename IndexType >
const IndexBuffer< IndexType >& Mesh< VertexType, IndexType >::indexBuffer() const
{
    return static_cast< const IndexBuffer< Index >& >( MeshBase::indexBuffer() );
}


template< typename VertexType, typename IndexType >
VertexBuffer< VertexType >& Mesh< VertexType, IndexType >::vertexBuffer()
{
    return static_cast< VertexBuffer< Vertex >& >( MeshBase::vertexBuffer() );
}


template< typename VertexType, typename IndexType >
IndexBuffer< IndexType >& Mesh< VertexType, IndexType >::indexBuffer()
{
    return static_cast< IndexBuffer< Index >& >( MeshBase::indexBuffer() );
}


template< typename VertexType, typename IndexType >
Mesh< VertexType, IndexType >& Mesh< VertexType, IndexType >::create( unsigned int primitiveType )
{
    return *new Mesh< Vertex, Index >( primitiveType );
}



}  // namespace Carna :: base

}  // namespace Carna

#endif // MESH_H_6014714286
