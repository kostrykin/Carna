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

#ifndef INDEXBUFFER_H_6014714286
#define INDEXBUFFER_H_6014714286

#include <LibCarna/LibCarna.h>
#include <LibCarna/base/BaseBuffer.h>
#include <iterator>
#include <cstdint>

/** \file   IndexBuffer.h
  * \brief  Defines \ref Carna::base::IndexBuffer.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// IndexBuffer
// ----------------------------------------------------------------------------------

/** \brief
  * Maintains `GL_ELEMENT_ARRAY_BUFFER` object that holds the vertex indices, that
  * interconnect the vertices to primitives. This class realizes the RAII-idiom.
  *
  * \author Leonid Kostrykin
  * \date   1.9.14 - 14.3.15
  */
class LIBCARNA IndexBufferBase : public BaseBuffer
{

protected:
    
    /** \brief
      * Copies \a indicesCount indices referenced by \a bufferPtr to the maintained
      * index buffer object.
      */
    void copy( const void* bufferPtr, std::size_t bufferSize, std::size_t indicesCount );

public:

    const static unsigned int TYPE_UINT_8;  ///< Indicates `uint8_t`  element type.
    const static unsigned int TYPE_UINT_16; ///< Indicates `uint16_t` element type.
    const static unsigned int TYPE_UINT_32; ///< Indicates `uint32_t` element type.

    /** \brief
      * Draws triangles. Indicates that the indices
      * \f$3i+k, k \in \left\{0,1,2\right\}\f$ make up the \f$i\f$th triangle with
      * \f$i \geq 2\f$.
      */
    const static unsigned int PRIMITIVE_TYPE_TRIANGLES;

    /** \brief
      * Draws triangles. Indicates that the indices
      * \f$i-k, k \in \left\{0,1,2\right\}\f$ make up the \f$i\f$th triangle with
      * \f$i \geq 2\f$.
      */
    const static unsigned int PRIMITIVE_TYPE_TRIANGLE_STRIP;

    /** \brief
      * Draws triangles. Indicates that the indices \f$0, i-2, i-1\f$ make up the
      * \f$i\f$th triangle with \f$i \geq 2\f$.
      */
    const static unsigned int PRIMITIVE_TYPE_TRIANGLE_FAN;

    /** \brief
      * Draws lines. Indicates that the indices \f$2i, 2i-1\f$ make up the \f$i\f$th
      * line segment with \f$i \geq 1\f$.
      */
    const static unsigned int PRIMITIVE_TYPE_LINES;

    /** \brief
      * Draws lines. Indicates that the indices \f$i, i-1\f$ make up a the \f$i\f$th
      * line segment with \f$i \geq 1\f$.
      */
    const static unsigned int PRIMITIVE_TYPE_LINE_STRIP;

    /** \brief
      * Draws lines. Indicates that the indices \f$i, i-1\f$ make up the \f$i\f$th
      * line segment. Another line segment from the last to the first index is added
      * automatically.
      */
    const static unsigned int PRIMITIVE_TYPE_LINE_LOOP;

    /** \brief
      * Draws points. Indicates that each index makes up a single point.
      */
    const static unsigned int PRIMITIVE_TYPE_POINTS;

    /** \brief
      * Holds the element type of the maintained OpenGL buffer object.
      */
    const unsigned int type;

    /** \brief
      * Holds how the indexed vertices do interconnect to primitives.
      */
    const unsigned int primitiveType;

    /** \brief
      * Creates `GL_ELEMENT_ARRAY_BUFFER` object.
      *
      * \param type
      *     indicates the element type of the maintained OpenGL buffer object.
      *
      * \param primitiveType
      *     indicates how primitives should be assembled from the vertices indexed by
      *     this buffer.
      */
    IndexBufferBase( unsigned int type, unsigned int primitiveType );

}; // IndexBufferBase



// ----------------------------------------------------------------------------------
// IndexBufferTypeMapper
// ----------------------------------------------------------------------------------

/** \brief
  * Maps index buffer element types to descriptive constants.
  */
template< typename IndexType >
struct IndexBufferTypeMapper
{
    static_assert( sizeof( IndexType ) < 0, "Unknown index type." );
};


/** \brief
  * Maps `uint8_t` to \ref IndexBufferBase::TYPE_UINT_8.
  */
template< >
struct IndexBufferTypeMapper< uint8_t >
{
    /** \brief
      * Maps `uint8_t` to \ref IndexBufferBase::TYPE_UINT_8. 
      */
    operator unsigned int()
    {
        return IndexBufferBase::TYPE_UINT_8;
    }
};


/** \brief
  * Maps `uint16_t` to \ref IndexBufferBase::TYPE_UINT_16.
  */
template< >
struct IndexBufferTypeMapper< uint16_t >
{
    /** \brief
      * Maps `uint16_t` to \ref IndexBufferBase::TYPE_UINT_16.
      */
    operator unsigned int()
    {
        return IndexBufferBase::TYPE_UINT_16;
    }
};


/** \brief
  * Maps `uint32_t` to \ref IndexBufferBase::TYPE_UINT_32.
  */
template< >
struct IndexBufferTypeMapper< uint32_t >
{
    /** \brief
      * Maps `uint32_t` to \ref IndexBufferBase::TYPE_UINT_32.
      */
    operator unsigned int()
    {
        return IndexBufferBase::TYPE_UINT_32;
    }
};



// ----------------------------------------------------------------------------------
// IndexBuffer
// ----------------------------------------------------------------------------------

/** \brief
  * Specializes \ref IndexBufferBase for particular \a IndexType.
  *
  * \author Leonid Kostrykin
  * \date   1.9.14 - 14.3.15
  */
template< typename IndexType >
class IndexBuffer : public IndexBufferBase
{

public:

    /** \brief
      * Holds the index type maintained by this index buffer object.
      */
    typedef IndexType Index;

    /** \brief
      * Creates `GL_ELEMENT_ARRAY_BUFFER` object.
      *
      * \param primitiveType
      *     indicates how primitives should be assembled from the vertices indexed by
      *     this buffer.
      */
    IndexBuffer( unsigned int primitiveType );

    /** \brief
      * Copies \a indicesCount indices referenced by \a indicesPtr to the
      * maintained index buffer object.
      */
    void copy( const IndexType* indicesPtr, const std::size_t indicesCount );

}; // IndexBuffer


template< typename IndexType >
IndexBuffer< IndexType >::IndexBuffer( unsigned int primitiveType )
    : IndexBufferBase( IndexBufferTypeMapper< IndexType >(), primitiveType )
{
}


template< typename IndexType >
void IndexBuffer< IndexType >::copy( const IndexType* indicesPtr, const std::size_t indicesCount )
{
    IndexBufferBase::copy( indicesPtr, indicesCount * sizeof( Index ), indicesCount );
}



}  // namespace Carna :: base

}  // namespace Carna

#endif // INDEXBUFFER_H_6014714286
