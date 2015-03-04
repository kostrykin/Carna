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

#ifndef VERTEXBUFFER_H_6014714286
#define VERTEXBUFFER_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/BaseBuffer.h>
#include <iterator>

/** \file   VertexBuffer.h
  * \brief  Defines \ref Carna::base::VertexBuffer.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// VertexBufferBase
// ----------------------------------------------------------------------------------

class CARNA_LIB VertexBufferBase : public BaseBuffer
{

protected:

    void copy( const void* buffer_ptr, std::size_t buffer_size, std::size_t vertices_count );

public:

    VertexBufferBase();

}; // VertexBufferBase



// ----------------------------------------------------------------------------------
// VertexBuffer
// ----------------------------------------------------------------------------------

template< typename Vertex >
class VertexBuffer : public VertexBufferBase
{

public:

    typedef typename Vertex Vertex;

    void copy( const Vertex* vertices_ptr, const std::size_t vertices_count );

}; // VertexBuffer


template< typename Vertex >
void VertexBuffer< Vertex >::copy( const Vertex* vertices_ptr, const std::size_t vertices_count )
{
    VertexBufferBase::copy( vertices_ptr, vertices_count * sizeof( Vertex ), vertices_count );
}



}  // namespace Carna :: base

}  // namespace Carna

#endif // VERTEXBUFFER_H_6014714286
