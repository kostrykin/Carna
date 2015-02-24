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
#include <Carna/base/view/VertexAttributes.h>

/** \file   Vertex.h
  * \brief  Defines \ref Carna::base::view::Vertex.
  */

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// VertexBase
// ----------------------------------------------------------------------------------

struct CARNA_LIB VertexBase
{
    static const VertexAttributes attributes;

    float x, y, z, w;
};

struct VertexNormal
{
    float nx, ny, nz, nw;
};

struct VertexTexCoord2
{
    float u, v;
};



}  // namespace Carna :: base view

}  // namespace Carna :: base

}  // namespace Carna

#endif // VERTEX_H_6014714286
