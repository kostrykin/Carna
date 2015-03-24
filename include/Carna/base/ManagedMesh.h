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

#ifndef MANAGEDMESH_H_6014714286
#define MANAGEDMESH_H_6014714286

#include <Carna/base/Mesh.h>
#include <Carna/base/GeometryFeature.h>
#include <Carna/base/ManagedMeshInterface.h>
#include <memory>
#include <vector>

/** \file   ManagedMesh.h
  * \brief  Defines \ref Carna::base::ManagedMesh.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// ManagedMeshBase
// ----------------------------------------------------------------------------------

/** \brief
  * Represents \ref MeshBase object whose lifetime is managed by instances of this
  * class. This is a format-independent abstract \ref ManagedMesh base class.
  *
  * \see
  * The \ref MeshFactory class contains a few examples.
  *
  * The class maintains one \ref VertexBuffer and one \ref IndexBuffer on an
  * application level. The buffers are created the first time the
  * \ref ManagedMeshInterface "mesh's video resources" are acquired.
  * The buffers are deleted when the last acquisition is released. The buffer
  * instances are available across all \ref GLContext "OpenGL contexts".
  *
  * The \ref MeshBase class is only valid within the OpenGL context it was created
  * within. Refer to the class documentation for an explanation.
  *
  * \author Leonid Kostrykin
  * \date   1.9.14 - 24.3.15
  */
class CARNA_LIB ManagedMeshBase : public GeometryFeature
{

    NON_COPYABLE
    
    struct Details;
    const std::unique_ptr< Details > pimpl;

protected:

    friend class GeometryFeature;
    friend class ManagedMeshInterface;

    /** \brief
      * Instantiates.
      */
    ManagedMeshBase( unsigned int primitiveType, const VertexAttributes& va );

    /** \brief
      * Deletes.
      */
    virtual ~ManagedMeshBase();
    
    /** \brief
      * Creates OpenGL vertex buffer object and fills it with data.
      */
    virtual VertexBufferBase* loadVertexBuffer() = 0;
    
    /** \brief
      * Creates OpenGL index buffer object and fills it with data.
      */
    virtual IndexBufferBase* loadIndexBuffer() = 0;
    
    /** \brief
      * Acquires mesh within current OpenGL context.
      */
    void acquireMesh();
    
    /** \brief
      * Releases mesh within current OpenGL context.
      */
    void releaseMesh();
    
    /** \brief
      * References mesh within current OpenGL context.
      */
    const MeshBase& mesh() const;

public:

    /** \brief
      * Defines the type to be used for interfacing the video resource.
      */
    typedef ManagedMeshInterface ManagedInterface;

    virtual bool controlsSameVideoResource( const GeometryFeature& ) const override;
    
    /** \brief
      * Holds the primitive type, like `GL_TRIANGLES`, that should be used when
      * rendering this mesh.
      */
    const unsigned int primitiveType;
    
    /** \brief
      * Holds the vertex format of the vertices contained by the vertex buffer.
      */
    const VertexAttributes vertexAttributes;

    virtual ManagedMeshInterface* acquireVideoResource() override;

}; // ManagedMeshBase



// ----------------------------------------------------------------------------------
// Mesh
// ----------------------------------------------------------------------------------

/** \brief
  * Implements \ref MeshBase class for particular \a VertexType and \a IndexType.
  *
  * \param VertexType specifies the \ref VertexBuffer "vertex buffer" format.
  * \param VertexType specifies the \ref  IndexBuffer  "index buffer" format.
  *
  * \see
  * The \ref MeshFactory class contains a few examples.
  *
  * \author Leonid Kostrykin
  * \date   22.2.15 - 24.3.15
  */
template< typename VertexType, typename IndexType >
class ManagedMesh : public ManagedMeshBase
{

    const std::vector< VertexType > vertices;
    const std::vector<  IndexType > indices;

    ManagedMesh( unsigned int primitiveType
        , const VertexType* vertices
        , const std::size_t vertexCount
        , const IndexType* indices
        , const std::size_t indexCount );
    
protected:
    
    virtual VertexBufferBase* loadVertexBuffer() override;
    
    virtual IndexBufferBase* loadIndexBuffer() override;

public:

    typedef VertexType Vertex;  ///< Holds the element type of the vertex buffer.
    typedef  IndexType  Index;  ///< Holds the element type of the  index buffer.

    /** \brief
      * Instantiates. Call \ref release when you do not need the object any longer.
      */
    static ManagedMesh< VertexType, IndexType >& create
        ( unsigned int primitiveType
        , const VertexType* vertices
        , const std::size_t vertexCount
        , const IndexType* indices
        , const std::size_t indexCount );

}; // ManagedMesh


template< typename VertexType, typename IndexType >
ManagedMesh< VertexType, IndexType >::ManagedMesh
        ( unsigned int primitiveType
        , const VertexType* vertices
        , const std::size_t vertexCount
        , const IndexType* indices
        , const std::size_t indexCount )
    : ManagedMeshBase( primitiveType, Vertex::attributes )
    , vertices( vertices, vertices + vertexCount )
    , indices (  indices,  indices +  indexCount )
{
}


template< typename VertexType, typename IndexType >
VertexBufferBase* ManagedMesh< VertexType, IndexType >::loadVertexBuffer()
{
    VertexBuffer< VertexType >* const vb = new VertexBuffer< VertexType >();
    vb->copy( &vertices.front(), vertices.size() );
    return vb;
}


template< typename VertexType, typename IndexType >
IndexBufferBase* ManagedMesh< VertexType, IndexType >::loadIndexBuffer()
{
    IndexBuffer< IndexType >* const ib = new IndexBuffer< IndexType >( primitiveType );
    ib->copy( &indices.front(), indices.size() );
    return ib;
}


template< typename VertexType, typename IndexType >
ManagedMesh< VertexType, IndexType >& ManagedMesh< VertexType, IndexType >::create
    ( unsigned int primitiveType
    , const VertexType* vertices
    , const std::size_t vertexCount
    , const IndexType* indices
    , const std::size_t indexCount )
{
    return *new ManagedMesh< Vertex, Index >( primitiveType, vertices, vertexCount, indices, indexCount );
}



}  // namespace Carna :: base

}  // namespace Carna

#endif // MANAGEDMESH_H_6014714286
