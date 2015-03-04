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

    std::size_t position;
    std::size_t componentsCount;

    VertexAttribute( std::size_t position, std::size_t componentsCount )
        : position( position )
        , componentsCount( componentsCount )
    {
    }

}; // VertexAttribute


typedef std::vector< VertexAttribute > VertexAttributes;



}  // namespace Carna :: base

}  // namespace Carna

#endif // VERTEXATTRIBUTES_H_6014714286
