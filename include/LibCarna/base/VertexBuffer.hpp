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

#ifndef VERTEXBUFFER_H_6014714286
#define VERTEXBUFFER_H_6014714286

#include <LibCarna/LibCarna.hpp>
#include <LibCarna/base/BaseBuffer.hpp>
#include <iterator>

/** \file
  * \brief
  * Defines \ref LibCarna::base::VertexBuffer.
  */

namespace LibCarna
{

namespace base
{



// ----------------------------------------------------------------------------------
// VertexBufferBase
// ----------------------------------------------------------------------------------

/** \brief
  * Maintains `GL_ARRAY_BUFFER` object that holds vertices. This class realizes the
  * RAII-idiom.
  *
  * \author Leonid Kostrykin
  */
class LIBCARNA VertexBufferBase : public BaseBuffer
{

protected:

    /** \brief
      * Copies \a verticesCount vertices referenced by \a bufferPtr to the maintained
      * vertex buffer object.
      */
    void copy( const void* bufferPtr, std::size_t bufferSize, std::size_t verticesCount );

public:

    /** \brief
      * Creates `GL_ARRAY_BUFFER` object.
      */
    VertexBufferBase();

}; // VertexBufferBase



// ----------------------------------------------------------------------------------
// VertexBuffer
// ----------------------------------------------------------------------------------

/** \brief
  * Specializes \ref VertexBufferBase for particular \a VertexType.
  *
  * \author Leonid Kostrykin
  */
template< typename VertexType >
class VertexBuffer : public VertexBufferBase
{

public:

    /** \brief
      * Holds the vertex type maintained by this vertex buffer object.
      */
    typedef VertexType Vertex;
    
    /** \brief
      * Copies \a verticesCount vertices referenced by \a verticesPtr to the
      * maintained vertex buffer object.
      */
    void copy( const Vertex* verticesPtr, const std::size_t verticesCount );

}; // VertexBuffer


template< typename VertexType >
void VertexBuffer< VertexType >::copy( const Vertex* verticesPtr, const std::size_t verticesCount )
{
    VertexBufferBase::copy( verticesPtr, verticesCount * sizeof( Vertex ), verticesCount );
}



}  // namespace LibCarna :: base

}  // namespace LibCarna

#endif // VERTEXBUFFER_H_6014714286
