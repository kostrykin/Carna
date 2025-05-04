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

#ifndef VERTEXATTRIBUTES_H_6014714286
#define VERTEXATTRIBUTES_H_6014714286

#include <LibCarna/LibCarna.h>
#include <vector>

/** \file   VertexAttributes.h
  * \brief  Defines \ref LibCarna::base::VertexAttributes.
  */

namespace LibCarna
{

namespace base
{



// ----------------------------------------------------------------------------------
// VertexAttributes
// ----------------------------------------------------------------------------------

/** \brief
  * Declares a vertex attribute.
  *
  * \author Leonid Kostrykin
  */
struct VertexAttribute
{

    /** \brief
      * Indicates vertex attribute of `GL_FLOAT` type.
      */
    const static unsigned int TYPE_FLOAT;

    /** \brief
      * Indicates vertex attribute of `GL_INT` type.
      */
    const static unsigned int TYPE_INT;

    /** \brief
      * Indicates vertex attribute of `GL_UNSIGNED_INT` type.
      */
    const static unsigned int TYPE_UINT;

    /** Holds position of the vertex attribute within the vertex definition.
      */
    std::size_t position;
    
    /** Holds number of components from \ref type within this vertex attribute.
      * The attribute is scalar if the value is \f$1\f$, it is a vector otherwise.
      */
    std::size_t componentsCount;
    
    /** Holds the type of this vertex attribute if it
      * \ref componentsCount "is scalar". Holds the type of the vector components
      * otherwise.
      */
    unsigned int type;

    /** Instantiates.
      *
      * The attribute is scalar if \a componentsCount is \f$1\f$. It is vector-valued
      * otherwise.
      *
      * \param position
      *     is the position of the vertex attribute within the vertex definition.
      *
      * \param componentsCount
      *     is the number of components from \a type within this vertex attribute.
      *
      * \param type
      *     is the type of this vertex attribute if it is scalar or the type of the
      *     vector components otherwise.
      */
    VertexAttribute( std::size_t position, std::size_t componentsCount, unsigned int type )
        : position( position )
        , componentsCount( componentsCount )
        , type( type )
    {
    }

}; // VertexAttribute


/** \brief
  * Defines \ref VertexAttribute vector.
  */
typedef std::vector< VertexAttribute > VertexAttributes;



}  // namespace LibCarna :: base

}  // namespace LibCarna

#endif // VERTEXATTRIBUTES_H_6014714286
