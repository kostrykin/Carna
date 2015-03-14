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
  * The class maintains one \ref VertexBuffer and one \ref IndexBuffer on an
  * application level. The buffers are created the first time the
  * \ref MeshBase::VideoResourceAcquisition "mesh's video resources" are acquired.
  * The buffers are deleted when the last acqusition is released. The buffer
  * instances are available across all \ref GLContext "OpenGL contexts".
  *
  * The class also maintains one so-called OpenGL vertex array, which, contrary to
  * its name, basically is a conjunction of vertex and index buffer. This is
  * maintained on a per-context level. This is explained further below.
  *
  * \section VertexArrays OpenGL Background on Vertex Arrays
  *
  * OpenGL vertex arrays cannot be shared across OpenGL contexts. Hence we need to
  * create one vertex array per context that it is acquired within.
  *
  * > Any OpenGL object types which are not containers are sharable, as well as Sync
  * > Objects and GLSL Objects (excluding program pipeline objects). All container
  * > objects are not shared between contexts. [1]
  * >
  * > Container objects:
  * > - %Framebuffer Objects
  * > - Program Pipeline
  * > - Transform Feedback Objects
  * > - Vertex Array Objects [2]
  *
  * References:
  *  -# https://www.opengl.org/wiki/OpenGL_Context
  *  -# https://www.opengl.org/wiki/OpenGL_Object#Container_objects
  *
  * \author Leonid Kostrykin
  * \date   22.2.15 - 14.3.15
  */
class CARNA_LIB MeshBase : public GeometryFeature
{

    NON_COPYABLE
    
    struct Details;
    const std::unique_ptr< Details > pimpl;

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
    
        unsigned int myId;
    
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
        
        GLContext& glContext;
    
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
