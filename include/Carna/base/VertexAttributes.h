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

struct VertexAttribute
{

    const static unsigned int TYPE_FLOAT;
    const static unsigned int TYPE_INT;
    const static unsigned int TYPE_UINT;

    std::size_t position;
    std::size_t componentsCount;
    unsigned int type;

    VertexAttribute( std::size_t position, std::size_t componentsCount, unsigned int type )
        : position( position )
        , componentsCount( componentsCount )
        , type( type )
    {
    }

}; // VertexAttribute


typedef std::vector< VertexAttribute > VertexAttributes;



}  // namespace Carna :: base

}  // namespace Carna

#endif // VERTEXATTRIBUTES_H_6014714286
