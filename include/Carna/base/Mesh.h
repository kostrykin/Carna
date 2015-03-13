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
  * Format-independent abstract \ref Mesh base class.
  *
  * \author Leonid Kostrykin
  * \date   22.2.15 - 13.3.15
  */
class CARNA_LIB MeshBase : public GeometryFeature
{

    NON_COPYABLE

    std::unique_ptr< VertexBufferBase > vertexBuffer;
    std::unique_ptr< IndexBufferBase > indexBuffer;
    
    unsigned int id;

protected:

    friend class GeometryFeature;

    MeshBase( unsigned int primitiveType, const VertexAttributes& va );

    virtual ~MeshBase();
    
    virtual VertexBufferBase* loadVertexBuffer() = 0;
    
    virtual IndexBufferBase* loadIndexBuffer() = 0;

public:

    virtual bool controlsSameVideoResource( const GeometryFeature& ) const override;
    
    const unsigned int primitiveType;
    
    const VertexAttributes vertexAttributes;

    // ------------------------------------------------------------------------------
    // MeshBase :: VideoResourceAcquisition
    // ------------------------------------------------------------------------------

    class VideoResourceAcquisition : public GeometryFeature::VideoResourceAcquisition
    {
    
    public:
    
        VideoResourceAcquisition( GLContext& glc, MeshBase& mesh );
    
        virtual ~VideoResourceAcquisition();
    
        unsigned int id() const;

        void bind() const;

        void render() const;

        const VertexBufferBase& vertexBuffer() const;

        const IndexBufferBase& indexBuffer() const;

        VertexBufferBase& vertexBuffer();

        IndexBufferBase& indexBuffer();
    
        MeshBase& mesh;
    
    }; // MeshBase :: VideoResourceAcquisition
    
    virtual VideoResourceAcquisition* acquireVideoResource( GLContext& glc ) override;

}; // MeshBase



// ----------------------------------------------------------------------------------
// Mesh
// ----------------------------------------------------------------------------------

template< typename VertexType, typename IndexType >
class Mesh : public MeshBase
{

    const std::vector< VertexType > vertices;
    const std::vector<  IndexType > indices;

    Mesh( unsigned int primitiveType
        , const VertexType* vertices
        , const std::size_t vertexCount
        , const IndexType* indices
        , const std::size_t indexCount );
    
protected:
    
    virtual VertexBufferBase* loadVertexBuffer() override;
    
    virtual IndexBufferBase* loadIndexBuffer() override;

public:

    typedef VertexType Vertex;

    typedef IndexType Index;

    /** \brief
      * Instantiates. Call \ref release when you do not need the object any longer.
      */
    static Mesh< VertexType, IndexType >& create
        ( unsigned int primitiveType
        , const VertexType* vertices
        , const std::size_t vertexCount
        , const IndexType* indices
        , const std::size_t indexCount );

}; // Mesh


template< typename VertexType, typename IndexType >
Mesh< VertexType, IndexType >::Mesh
        ( unsigned int primitiveType
        , const VertexType* vertices
        , const std::size_t vertexCount
        , const IndexType* indices
        , const std::size_t indexCount )
    : MeshBase( primitiveType, Vertex::attributes )
    , vertices( vertices, vertices + vertexCount )
    , indices (  indices,  indices +  indexCount )
{
}


template< typename VertexType, typename IndexType >
VertexBufferBase* Mesh< VertexType, IndexType >::loadVertexBuffer()
{
    VertexBuffer< VertexType >* const vb = new VertexBuffer< VertexType >();
    vb->copy( &vertices.front(), vertices.size() );
    return vb;
}


template< typename VertexType, typename IndexType >
IndexBufferBase* Mesh< VertexType, IndexType >::loadIndexBuffer()
{
    IndexBuffer< IndexType >* const ib = new IndexBuffer< IndexType >( primitiveType );
    ib->copy( &indices.front(), indices.size() );
    return ib;
}


template< typename VertexType, typename IndexType >
Mesh< VertexType, IndexType >& Mesh< VertexType, IndexType >::create
    ( unsigned int primitiveType
    , const VertexType* vertices
    , const std::size_t vertexCount
    , const IndexType* indices
    , const std::size_t indexCount )
{
    return *new Mesh< Vertex, Index >( primitiveType, vertices, vertexCount, indices, indexCount );
}



}  // namespace Carna :: base

}  // namespace Carna

#endif // MESH_H_6014714286
