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

/** \brief
  * Defines simple-most vertex that only consists of a positional attribute.
  *
  * \section CustomVertexFormats Custom Vertex Formats
  *
  * It is easy to define custom vertex formats. The paradigm is best explained with
  * an example. Lets assume you want to define a vertex that has additional
  * properties for normal vectors and colors.
  *
  * The first step is to define the missing vertex components. The \ref VertexNormal
  * type already provides a vertex component for normal vectors, so lets define a
  * component for colors:
  *
  *     \code
  *     struct VertexColor
  *     {
  *         float r, g, b, a;
  *     };
  *     \endcode
  *
  * It is very important that a vertex component is implemented as a POD, i.e. *plain
  * old data type*. Virtual methods would mess up the memory layout. However, you
  * might define a constructor that initializes default values, if you wanted.
  *
  * The next step is to compose the vertex format:
  *
  *     \code
  *     using namespace Carna::base;
  *     struct LightedVertex
  *         : public VertexBase
  *         , public VertexNormal
  *         , public VertexColor
  *     {
  *         static const VertexAttributes attributes;
  *     };
  *     \endcode
  *
  * The order of the base classes is arbitrary, but it must be consistent with what
  * comes next, namely the specification of the vertex format.
  *
  *     \code
  *     #include <vector>
  *     using namespace Carna::base;
  *     const VertexAttributes LightedVertex::attributes = []()->VertexAttributes
  *     {
  *         using Carna::base::VertexAttribute;  // msvc++ requires us to repeat this
  *         std::vector< VertexAttribute > attributes;
  *         attributes.push_back( VertexAttribute( 0, 4, VertexAttribute::TYPE_FLOAT ) );
  *         attributes.push_back( VertexAttribute( 4, 4, VertexAttribute::TYPE_FLOAT ) );
  *         attributes.push_back( VertexAttribute( 8, 4, VertexAttribute::TYPE_FLOAT ) );
  *         return attributes;
  *     }();
  *     \endcode
  *
  * You should read the above like:
  *
  *   - Attribute 1 starts at offset 0 and has 4 components, namely the
  *     positional \c x, \c y, \c z, \c w.
  *   - Attribute 2 starts at offset 4 and has 4 components, namely the
  *     \c nx, \c ny, \c nz, \c nw of the normal vector.
  *   - Attribute 3 starts at offset 8 and has 4 components, namely the
  *     \c r, \c g, \c b, \c a of the color vector.
  *
  * When writing your shader, you must declare the vertex format consistently:
  *
  *     \code
  *     layout( location = 0 ) in vec4 inPosition;
  *     layout( location = 1 ) in vec4 inNormal;
  *     layout( location = 2 ) in vec4 inColor;
  *     \endcode
  *
  * \author Leonid Kostrykin
  * \date   1.9.14 - 10.3.15
  */
struct CARNA_LIB VertexBase
{
    static const VertexAttributes attributes;

    float x, y, z, w;

    /** \brief
      * Initializes position to \f$\left(0, 0, 0, 1\right)\f$.
      */
    VertexBase();
};



// ----------------------------------------------------------------------------------
// VertexNormal
// ----------------------------------------------------------------------------------

/** \brief
  * Defines vertex component for normal vectors.
  * Usage is explained \ref CustomVertexFormats "here".
  *
  * \author Leonid Kostrykin
  * \date   1.9.14 - 10.3.15
  */
struct VertexNormal
{
    float nx, ny, nz, nw;
};



// ----------------------------------------------------------------------------------
// VertexTexCoord2
// ----------------------------------------------------------------------------------

/** \brief
  * Defines vertex component for 2D texture coordinates.
  * Usage is explained \ref CustomVertexFormats "here".
  *
  * \author Leonid Kostrykin
  * \date   1.9.14 - 10.3.15
  */
struct VertexTexCoord2
{
    float u, v;
};



}  // namespace Carna :: base

}  // namespace Carna

#endif // VERTEX_H_6014714286
