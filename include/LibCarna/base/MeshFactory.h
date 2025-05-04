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

#ifndef MESHFACTORY_H_6014714286
#define MESHFACTORY_H_6014714286

#include <LibCarna/base/VertexAttributes.h>
#include <LibCarna/base/VertexBuffer.h>
#include <LibCarna/base/IndexBuffer.h>
#include <LibCarna/base/ManagedMesh.h>
#include <LibCarna/base/Vertex.h>
#include <LibCarna/base/math.h>
#include <cstdint>
#include <unordered_set>
#include <functional>
#include <memory>
#include <istream>
#include <ios>
#include <string>

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
    static VertexType vertex( const VectorType& position, const VectorType& normal = VectorType(), const VectorType& color = VectorType() );

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
      * Creates sphere with \a radius and a vertices number determined by \a degree.
      * The ball is centered in \f$\left(0, 0, 0\right)^\mathrm T\f$.
      *
      * \date   June 2021
      * \since  \ref v_3_1_0
      */
    static ManagedMesh< VertexType, uint16_t >& createBall( float radius, unsigned int degree = 3 );

    /** \brief
      * Creates mesh that consists of a single point.
      */
    static ManagedMesh< VertexType, uint8_t >& createPoint();

    /** \brief
      * Creates mesh from an STL file.
      *
      * \author Tim Schroeder
      * \date   July 2015
      * \since  \ref v_3_0_2
      */
    inline static ManagedMesh< VertexType, uint32_t >& createFromSTL( const std::string& path );

    /** \overload
     */
    static ManagedMesh< VertexType, uint32_t >& createFromSTL( std::istream& stlStream );

}; // MeshFactory


template< typename VertexType >
template< typename VectorType >
VertexType MeshFactory< VertexType >::vertex( const VectorType& position, const VectorType& normal, const VectorType& color )
{
    VertexType vertex;
    vertex.x = position.x();
    vertex.y = position.y();
    vertex.z = position.z();
    vertex.setNormal( normal );
    vertex.setColor ( color  );
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
        const auto positionTransform = baseTransform * transforms[ faceIndex ];
        const auto   normalTransform = positionTransform.inverse().transpose();
        const auto normalVector = ( normalTransform * math::Vector4f( 0, 0, 1, 0 ) ).normalized();

        vertices[ ++lastVertex ] = vertex< math::Vector4f >( positionTransform * math::Vector4f( -1, -1, 1, 1 ), normalVector );
        vertices[ ++lastVertex ] = vertex< math::Vector4f >( positionTransform * math::Vector4f( +1, -1, 1, 1 ), normalVector );
        vertices[ ++lastVertex ] = vertex< math::Vector4f >( positionTransform * math::Vector4f( +1, +1, 1, 1 ), normalVector );
        vertices[ ++lastVertex ] = vertex< math::Vector4f >( positionTransform * math::Vector4f( -1, +1, 1, 1 ), normalVector );

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
ManagedMesh< VertexType, uint16_t >& MeshFactory< VertexType >::createBall( float radius, unsigned int degree )
{
    const math::Matrix4f baseTransform = math::scaling4f( radius, radius, radius );
    const unsigned int verticesPerEdge = 2 + degree;
    const unsigned int verticesPerSide = verticesPerEdge * verticesPerEdge;
    const unsigned int    facesPerSide = ( verticesPerEdge - 1 ) * ( verticesPerEdge - 1 );
    const unsigned int  indicesPerSide = 6 * facesPerSide;

    /* Define sides.
     */
    math::Matrix4f transforms[ 6 ];
    transforms[ 0 ] = math::basis4f( math::Vector3f(  0,  0, +1 ), math::Vector3f(  0, +1,  0 ), math::Vector3f( -1,  0,  0 ) );  // left
    transforms[ 1 ] = math::basis4f( math::Vector3f(  0,  0, -1 ), math::Vector3f(  0, +1,  0 ), math::Vector3f( +1,  0,  0 ) );  // right
    transforms[ 2 ] = math::basis4f( math::Vector3f( +1,  0,  0 ), math::Vector3f(  0, +1,  0 ), math::Vector3f(  0,  0, +1 ) );  // front
    transforms[ 3 ] = math::basis4f( math::Vector3f( -1,  0,  0 ), math::Vector3f(  0, +1,  0 ), math::Vector3f(  0,  0, -1 ) );  // back
    transforms[ 4 ] = math::basis4f( math::Vector3f( +1,  0,  0 ), math::Vector3f(  0,  0, -1 ), math::Vector3f(  0, +1,  0 ) );  // top
    transforms[ 5 ] = math::basis4f( math::Vector3f( +1,  0,  0 ), math::Vector3f(  0,  0, +1 ), math::Vector3f(  0, -1,  0 ) );  // bottom

    const std::size_t verticesCount = 6 * verticesPerSide;
    const std::size_t indicesCount  = 6 * indicesPerSide;
    LIBCARNA_ASSERT(indicesCount < ( 1 << 16 ));

    typedef ManagedMesh< VertexType, uint16_t > MeshInstance;
    typedef typename MeshInstance::Vertex Vertex;
    typedef typename MeshInstance:: Index  Index;
    Vertex vertices[ verticesCount ];
    Index   indices[  indicesCount ];

    int lastVertex = -1;
    int lastIndex  = -1;

    /* Create vertices and indices.
     */
    for( unsigned int sideIndex = 0; sideIndex < 6; ++sideIndex )
    {
        const auto positionTransform = baseTransform * transforms[ sideIndex ];
        const auto   normalTransform = positionTransform.inverse().transpose();

        for( unsigned int y = 0; y < verticesPerEdge; ++y )
        for( unsigned int x = 0; x < verticesPerEdge; ++x )
        {
            const float fx = 2 * x / float( verticesPerEdge - 1 );
            const float fy = 2 * y / float( verticesPerEdge - 1 );
            const auto position = math::Vector3f( -1 + fx, -1 + fy, 1 ).normalized();
            vertices[ ++lastVertex ] = vertex< math::Vector4f >
                ( positionTransform * math::Vector4f( position.x(), position.y(), position.z(), 1 )
                ,   normalTransform * math::Vector4f( position.x(), position.y(), position.z(), 0 )
            );
        }

        for( unsigned int y = 0; y < verticesPerEdge - 1; ++y )
        for( unsigned int x = 0; x < verticesPerEdge - 1; ++x )
        {
            const uint16_t ul = x     + y * verticesPerEdge;         // upper left
            const uint16_t ur = x + 1 + y * verticesPerEdge;         // upper right
            const uint16_t ll = x     + ( y + 1 ) * verticesPerEdge; // lower left
            const uint16_t lr = x + 1 + ( y + 1 ) * verticesPerEdge; // lower right
            
            indices[ ++lastIndex ] = verticesPerSide * sideIndex + ul;
            indices[ ++lastIndex ] = verticesPerSide * sideIndex + lr;
            indices[ ++lastIndex ] = verticesPerSide * sideIndex + ll;

            indices[ ++lastIndex ] = verticesPerSide * sideIndex + ul;
            indices[ ++lastIndex ] = verticesPerSide * sideIndex + ur;
            indices[ ++lastIndex ] = verticesPerSide * sideIndex + lr;
        }
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

template< typename VertexType >
inline ManagedMesh< VertexType, uint32_t >& MeshFactory< VertexType >::createFromSTL(const std::string& path)
{
    return createFromSTL( std::fstream(path, std::ios::in | std::ios::binary) );
}

template< typename VertexType >
ManagedMesh< VertexType, uint32_t >& MeshFactory< VertexType >::createFromSTL( std::istream& stlStream )
{
    auto origExceptMask = stlStream.exceptions();
    stlStream.exceptions(std::istream::failbit | std::istream::badbit | std::istream::eofbit);

    uint32_t amountTriangles;

    //discard first 80 bytes
    stlStream.seekg(stlStream.beg + 80);

    //read amount of triangles (32bit-uint)
    stlStream.read(reinterpret_cast<char*>(&amountTriangles), 4);
    
    typedef ManagedMesh< VertexType, uint32_t > MeshInstance;
    typedef typename MeshInstance::Vertex Vertex;
    typedef typename MeshInstance::Index  Index;
    typedef typename std::pair< Vertex, std::size_t > VertexIndexPair;

    std::size_t indicesCount = amountTriangles * 3;
    std::unique_ptr<Index[]>  indices (new Index  [indicesCount]);
    std::unique_ptr<Vertex[]> vertices(new Vertex [indicesCount]);
    
    std::function<std::size_t(const VertexIndexPair&)> vertexHash =
    [](const VertexIndexPair& vertAndIndex) -> std::size_t
    {
        //creates combined hash value of the 3 public members "x","y","z" of a Vertex object; ignores the second member of pair
        //This hash-combine-technique is taken from : http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3876.pdf (Page 2)
        std::size_t hashVal = std::hash<decltype(vertAndIndex.first.x)>()(vertAndIndex.first.x);
        hashVal ^= std::hash<decltype(vertAndIndex.first.y)>()(vertAndIndex.first.y) + 0x9e3779b9 + (hashVal << 6) + (hashVal >> 2);
        hashVal ^= std::hash<decltype(vertAndIndex.first.z)> ()(vertAndIndex.first.z) + 0x9e3779b9 + (hashVal << 6) + (hashVal >> 2);
        return hashVal;
    };

    std::function<std::size_t(const VertexIndexPair&, const VertexIndexPair&)> vertexComp =
    [](const VertexIndexPair& vertAndIndex1, const VertexIndexPair& vertAndIndex2) -> bool
    {
        //checks whether the members "x", "y", "z" of two Vertex objects are equal
        return ((vertAndIndex1.first.x) == (vertAndIndex2.first.x)) && ((vertAndIndex1.first.y) == (vertAndIndex2.first.y)) && ((vertAndIndex1.first.z) == (vertAndIndex2.first.z));
    };

    std::unordered_set< VertexIndexPair, decltype(vertexHash), decltype(vertexComp) > vertsWithArrIndices(amountTriangles * 3, vertexHash, vertexComp);

    std::size_t indicesItCount = 0;
    std::size_t verticesItCount = 0;

    //read triangles
    for (unsigned int i = 0; i < amountTriangles; i++)
    {
        //discard normal
        stlStream.seekg(12, std::ios_base::cur);
        
        //read vertices
        for (unsigned int j = 0; j < 3; j++)    //read 3 vertices
        {
            float x, y, z;
            
            //read 1 vertex
            stlStream.read(reinterpret_cast<char*>(&x), 4);
            stlStream.read(reinterpret_cast<char*>(&y), 4);
            stlStream.read(reinterpret_cast<char*>(&z), 4);

            Vertex vert;
            vert.x = x;
            vert.y = y;
            vert.z = z;
            //vert gets invalidated here
            auto emplRes = vertsWithArrIndices.emplace(std::piecewise_construct, std::forward_as_tuple(std::move(vert)), std::forward_as_tuple(verticesItCount));

            if (emplRes.second)
            {
                vertices[verticesItCount++] = (emplRes.first)->first;
            }

            indices[indicesItCount++] = (emplRes.first)->second;
        }

        //discard 2 bytes
        stlStream.seekg(2, std::ios_base::cur);
    }

    stlStream.exceptions(origExceptMask);

    return MeshInstance::create
        (IndexBufferBase::PRIMITIVE_TYPE_TRIANGLES
        , vertices.get(), verticesItCount
        , indices.get(), indicesCount);
}



}  // namespace Carna :: base

}  // namespace Carna

#endif // MESHFACTORY_H_6014714286
