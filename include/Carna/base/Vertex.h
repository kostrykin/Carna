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

#ifndef VERTEX_H_6014714286
#define VERTEX_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/VertexAttributes.h>

/** \file   Vertex.h
  * \brief  Defines \ref Carna::base::Vertex.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// VertexBase
// ----------------------------------------------------------------------------------

struct CARNA_LIB VertexBase
{
    static const VertexAttributes attributes;

    float x, y, z, w;

    /** \brief
      * Initializes position to \f$\left(0, 0, 0, 1\right)\f$.
      */
    VertexBase();
};

struct VertexNormal
{
    float nx, ny, nz, nw;
};

struct VertexTexCoord2
{
    float u, v;
};



}  // namespace Carna :: base

}  // namespace Carna

#endif // VERTEX_H_6014714286
