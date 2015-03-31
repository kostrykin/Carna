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

#ifndef MESHFACTORY_H_6014714286
#define MESHFACTORY_H_6014714286

#include <Carna/base/VertexAttributes.h>
#include <Carna/base/VertexBuffer.h>
#include <Carna/base/IndexBuffer.h>
#include <Carna/base/ManagedMesh.h>
#include <Carna/base/math.h>

/** \file   MeshFactory.h
  * \brief  Defines \ref Carna::base::MeshFactory.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// MeshFactory
// ----------------------------------------------------------------------------------

/** \brief
  * Creates simple predefined \ref ManagedMesh instances.
  *
  * \param VertexType
  *     specifies the vertex type that is to be used to build the
  *     \ref VertexBuffer "vertex buffers".
  *
  * \author Leonid Kostrykin
  * \date   22.2.15 - 14.3.15
  */
template< typename VertexType >
class MeshFactory
{

    template< typename VectorType >
    static VertexType vertex( const VectorType& );

public:

    /** \brief
      * Creates box with \a width, \a height and \a depth. The box is centered in
      * \f$\left(0, 0, 0\right)^\mathrm T\f$.
      */
    static ManagedMesh< VertexType, uint8_t >& createBox( float width, float height, float depth );

    /** \overload
      */
    static ManagedMesh< VertexType, uint8_t >& createBox( const math::Vector3f& size );

    /** \brief
      * Creates mesh that consists of a single point.
      */
    static ManagedMesh< VertexType, uint8_t >& createPoint();

}; // MeshFactory


template< typename VertexType >
template< typename VectorType >
VertexType MeshFactory< VertexType >::vertex( const VectorType& v )
{
    VertexType vertex;
    vertex.x = v.x();
    vertex.y = v.y();
    vertex.z = v.z();
    return vertex;
}


template< typename VertexType >
ManagedMesh< VertexType, uint8_t >& MeshFactory< VertexType >::createBox( const math::Vector3f& size )
{
    return createBox( size.x(), size.y(), size.z() );
}


template< typename VertexType >
ManagedMesh< VertexType, uint8_t >& MeshFactory< VertexType >::createBox( float sizeX, float sizeY, float sizeZ )
{
    const math::Matrix4f baseTransform = math::scaling4f( sizeX / 2, sizeY / 2, sizeZ / 2 );

    /* Define faces.
     */
    math::Matrix4f transforms[ 6 ];
    transforms[ 0 ] = math::basis4f( math::Vector3f(  0,  0, +1 ), math::Vector3f(  0, +1,  0 ), math::Vector3f( -1,  0,  0 ) );  // left
    transforms[ 1 ] = math::basis4f( math::Vector3f(  0,  0, -1 ), math::Vector3f(  0, +1,  0 ), math::Vector3f( +1,  0,  0 ) );  // right
    transforms[ 2 ] = math::basis4f( math::Vector3f( +1,  0,  0 ), math::Vector3f(  0, +1,  0 ), math::Vector3f(  0,  0, +1 ) );  // front
    transforms[ 3 ] = math::basis4f( math::Vector3f( -1,  0,  0 ), math::Vector3f(  0, +1,  0 ), math::Vector3f(  0,  0, -1 ) );  // back
    transforms[ 4 ] = math::basis4f( math::Vector3f( +1,  0,  0 ), math::Vector3f(  0,  0, -1 ), math::Vector3f(  0, +1,  0 ) );  // top
    transforms[ 5 ] = math::basis4f( math::Vector3f( +1,  0,  0 ), math::Vector3f(  0,  0, +1 ), math::Vector3f(  0, -1,  0 ) );  // bottom

    const std::size_t verticesCount = 6 * 4;
    const std::size_t indicesCount  = 6 * 2 * 3;

    typedef ManagedMesh< VertexType, uint8_t > MeshInstance;
    typedef typename MeshInstance::Vertex Vertex;
    typedef typename MeshInstance:: Index  Index;
    Vertex vertices[ verticesCount ];
    Index   indices[  indicesCount ];

    int lastVertex = -1;
    int lastIndex  = -1;

    /* Create vertices and indices.
     */
    for( unsigned int faceIndex = 0; faceIndex < 6; ++faceIndex )
    {
        const math::Matrix4f transform = baseTransform * transforms[ faceIndex ];
        vertices[ ++lastVertex ] = vertex( transform * math::Vector4f( -1, -1, 1, 1 ) );
        vertices[ ++lastVertex ] = vertex( transform * math::Vector4f( +1, -1, 1, 1 ) );
        vertices[ ++lastVertex ] = vertex( transform * math::Vector4f( +1, +1, 1, 1 ) );
        vertices[ ++lastVertex ] = vertex( transform * math::Vector4f( -1, +1, 1, 1 ) );

        indices[ ++lastIndex ] = lastVertex - 3;
        indices[ ++lastIndex ] = lastVertex - 2;
        indices[ ++lastIndex ] = lastVertex;

        indices[ ++lastIndex ] = lastVertex;
        indices[ ++lastIndex ] = lastVertex - 2;
        indices[ ++lastIndex ] = lastVertex - 1;
    }

    return MeshInstance::create
        ( IndexBufferBase::PRIMITIVE_TYPE_TRIANGLES
        , vertices, verticesCount
        ,  indices,  indicesCount );
}


template< typename VertexType >
ManagedMesh< VertexType, uint8_t >& MeshFactory< VertexType >::createPoint()
{
    typedef ManagedMesh< VertexType, uint8_t > MeshInstance;
    typedef typename MeshInstance::Vertex Vertex;
    typedef typename MeshInstance:: Index  Index;

    Vertex vertex;
    Index index = 0;

    return MeshInstance::create( IndexBufferBase::PRIMITIVE_TYPE_POINTS, &vertex, 1, &index, 1 );
}



}  // namespace Carna :: base

}  // namespace Carna

#endif // MESHFACTORY_H_6014714286
