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

#ifndef INDEXBUFFER_H_6014714286
#define INDEXBUFFER_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/view/BaseBuffer.h>
#include <iterator>
#include <cstdint>

/** \file   IndexBuffer.h
  * \brief  Defines \ref Carna::base::view::IndexBuffer.
  */

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// IndexBuffer
// ----------------------------------------------------------------------------------

class CARNA_LIB IndexBufferBase : public BaseBuffer
{

protected:

    void copy( const void* buffer_ptr, std::size_t buffer_size, std::size_t indices_count );

public:

    const static unsigned int TYPE_UINT_8;
    const static unsigned int TYPE_UINT_16;
    const static unsigned int TYPE_UINT_32;

    const static unsigned int PRIMITIVE_TYPE_TRIANGLES;
    const static unsigned int PRIMITIVE_TYPE_TRIANGLE_STRIP;
    const static unsigned int PRIMITIVE_TYPE_TRIANGLE_FAN;
    const static unsigned int PRIMITIVE_TYPE_LINES;
    const static unsigned int PRIMITIVE_TYPE_LINE_STRIP;
    const static unsigned int PRIMITIVE_TYPE_LINE_LOOP;
    const static unsigned int PRIMITIVE_TYPE_POINTS;

    const unsigned int type;

    const unsigned int primitiveType;

    IndexBufferBase( unsigned int type, unsigned int primitiveType );

}; // IndexBufferBase



// ----------------------------------------------------------------------------------
// IndexBufferTypeMapper
// ----------------------------------------------------------------------------------

template< typename Index >
struct IndexBufferTypeMapper
{
};


template< >
struct IndexBufferTypeMapper< uint8_t >
{
    operator unsigned int()
    {
        return IndexBufferBase::TYPE_UINT_8;
    }
};


template< >
struct IndexBufferTypeMapper< uint16_t >
{
    operator unsigned int()
    {
        return IndexBufferBase::TYPE_UINT_16;
    }
};


template< >
struct IndexBufferTypeMapper< uint32_t >
{
    operator unsigned int()
    {
        return IndexBufferBase::TYPE_UINT_32;
    }
};



// ----------------------------------------------------------------------------------
// IndexBuffer
// ----------------------------------------------------------------------------------

template< typename Index >
class IndexBuffer : public IndexBufferBase
{

public:

    typedef typename Index Index;

    IndexBuffer( unsigned int primitiveType );

    void copy( const Index* indices_ptr, const std::size_t indices_count );

}; // IndexBuffer


template< typename Index >
IndexBuffer< Index >::IndexBuffer( unsigned int primitiveType )
    : IndexBufferBase( IndexBufferTypeMapper< Index >(), primitiveType )
{
}


template< typename Index >
void IndexBuffer< Index >::copy( const Index* indices_ptr, const std::size_t indices_count )
{
    IndexBufferBase::copy( indices_ptr, indices_count * sizeof( Index ), indices_count );
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // INDEXBUFFER_H_6014714286
