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
#include <memory>

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
  * Format-independent abstract \ref Mesh base class. Each mesh consists of a
  * \ref VertexBuffer, an \ref IndexBuffer and a *vertex array*. This class realizes
  * the RAII-idiom w.r.t. the vertex array.
  *
  * The class maintains a so-called OpenGL vertex array in RAII-manner, which,
  * contrary to its name, basically is a conjunction of vertex and index buffer.
  * Because a vertex array is not \ref GLContext "sharable across OpenGL contexts",
  * objects from this class are always coupled to a particular context.
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
  * \date   1.9.14 - 24.3.15
  */
class CARNA_LIB MeshBase
{

    const std::unique_ptr< Association< VertexBufferBase > > myVertexBuffer;
    const std::unique_ptr< Association<  IndexBufferBase > >  myIndexBuffer;

public:

    /** Creates new mesh. It is only valid within the
      * \ref GLContext "current OpenGL context".
      * \post `&GLContext::current() == &glContext`
      */
    MeshBase
        ( const VertexAttributes& va
        , Association< VertexBufferBase >* vertexBuffer
        , Association<  IndexBufferBase >*  indexBuffer );

    /** \brief
      * Deletes.
      * \pre `&GLContext::current() == &glContext`
      */
    virtual ~MeshBase() noexcept( false );

    /** \brief
      * Holds the ID of the OpenGL vertex array object.
      */
    const unsigned int id;
    
    /** \brief
      * References the OpenGL context this mesh is valid within.
      */
    const GLContext& glContext;

    /** \brief
      * Binds the vertex array object.
      * \pre `&GLContext::current() == &glContext`
      */
    void bind() const;

    /** \brief
      * Renders the mesh.
      * \pre `&GLContext::current() == &glContext`
      */
    void render() const;

    const VertexBufferBase& vertexBuffer() const;   ///< References the mesh's vertex buffer.
    const  IndexBufferBase&  indexBuffer() const;   ///< References the mesh's  index buffer.
    
    VertexBufferBase& vertexBuffer();   ///< References the mesh's vertex buffer.
     IndexBufferBase&  indexBuffer();   ///< References the mesh's  index buffer.

}; // MeshBase



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
class Mesh : public MeshBase
{

public:

    /** Creates new mesh. It is only valid within the
      * \ref GLContext "current OpenGL context".
      */
    Mesh( Association< VertexBufferBase >* vertexBuffer
        , Association<  IndexBufferBase >* indexBuffer );

    typedef VertexType Vertex;  ///< Holds the element type of the vertex buffer.
    typedef  IndexType  Index;  ///< Holds the element type of the  index buffer.

}; // Mesh


template< typename VertexType, typename IndexType >
Mesh< VertexType, IndexType >::Mesh
        ( Association< VertexBufferBase >* vertexBuffer
        , Association<  IndexBufferBase >* indexBuffer )
    : MeshBase( VertexType::attributes, vertexBuffer, indexBuffer )
{
}



}  // namespace Carna :: base

}  // namespace Carna

#endif // MESH_H_6014714286
