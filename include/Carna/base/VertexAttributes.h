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

#ifndef VERTEXATTRIBUTES_H_6014714286
#define VERTEXATTRIBUTES_H_6014714286

#include <Carna/Carna.h>
#include <vector>

/** \file   VertexAttributes.h
  * \brief  Defines \ref Carna::base::VertexAttributes.
  */

namespace Carna
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
  * \date   22.2.15 - 19.3.15
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



}  // namespace Carna :: base

}  // namespace Carna

#endif // VERTEXATTRIBUTES_H_6014714286
