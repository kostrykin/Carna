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

#ifndef VERTEX_H_6014714286
#define VERTEX_H_6014714286

#include <LibCarna/LibCarna.hpp>
#include <LibCarna/base/VertexAttributes.hpp>

/** \file
  * \brief
  * Defines \ref LibCarna::base::PVertex, \ref LibCarna::base::PNVertex, \ref LibCarna::base::VertexPosition,
  * \ref LibCarna::base::VertexNormal.
  */

namespace LibCarna
{

namespace base
{



// ----------------------------------------------------------------------------------
// VertexPosition
// ----------------------------------------------------------------------------------

/** \brief
  * Defines vertex component for position vectors.
  * Usage is explained \ref CustomVertexFormats "here".
  *
  * \author Leonid Kostrykin
  */
struct VertexPosition
{
    /** \property x
      * \brief Holds the positional x-component of this vertex.
      *
      * \property y
      * \brief Holds the positional y-component of this vertex.
      *
      * \property z
      * \brief Holds the positional z-component of this vertex.
      *
      * \property w
      * \brief Holds the positional w-component of this vertex.
      *        This will be `1` usually.
      */
    float x, y, z, w;

    /** Initializes to \f$ \left( 0, 0, 0, 1 \right) \f$.
      */
    VertexPosition();

    /** Sets the position vector.
      */
    template< typename VectorType >
    void setPosition( const VectorType& position );
};


template< typename VectorType >
void VertexPosition::setPosition( const VectorType& position )
{
    x = position.x();
    y = position.y();
    z = position.z();
}



// ----------------------------------------------------------------------------------
// VertexNormal
// ----------------------------------------------------------------------------------

/** \brief
  * Defines vertex component for normal vectors.
  * Usage is explained \ref CustomVertexFormats "here".
  *
  * \author Leonid Kostrykin
  */
struct VertexNormal
{
    /** \property nx
      * \brief Holds the normal vector x-component of this vertex.
      *
      * \property ny
      * \brief Holds the normal vector y-component of this vertex.
      *
      * \property nz
      * \brief Holds the normal vector z-component of this vertex.
      *
      * \property nw
      * \brief Holds the normal vector w-component of this vertex.
      *        This will be `0` usually.
      */
    float nx, ny, nz, nw;

    /** Initializes to \f$ \left( 0, 0, 0, 0 \right) \f$.
      */
    VertexNormal();

    /** Sets the normal vector.
      */
    template< typename VectorType >
    void setNormal( const VectorType& normal );
};


template< typename VectorType >
void VertexNormal::setNormal( const VectorType& normal )
{
    nx = normal.x();
    ny = normal.y();
    nz = normal.z();
}



// ----------------------------------------------------------------------------------
// VertexColor
// ----------------------------------------------------------------------------------

/** \brief
  * Defines vertex component for colors.
  * Usage is explained \ref CustomVertexFormats "here".
  *
  * \author Leonid Kostrykin
  */
struct VertexColor
{
    /** \property r
      * \brief Holds the red color component of this vertex.
      *
      * \property g
      * \brief Holds the green color component of this vertex.
      *
      * \property b
      * \brief Holds the blue color component of this vertex.
      *
      * \property a
      * \brief Holds the alpha color component of this vertex.
      */
    float r, g, b, a;

    /** Initializes to \f$ \left( 1, 1, 1, 1 \right) \f$.
      */
    VertexColor();

    /** Sets the color vector.
      */
    template< typename VectorType >
    void setColor( const VectorType& color );
};


template< typename VectorType >
void VertexColor::setColor( const VectorType& color )
{
    r = color.x();
    g = color.y();
    b = color.z();
    a = color.w();
}



// ----------------------------------------------------------------------------------
// VERTEX_NULL_COMPONENT
// ----------------------------------------------------------------------------------

/** \brief
  * Adds null implementation for setter corresponding the the given vertex component.
  *
  * \author Leonid Kostrykin
  */
#define VERTEX_NULL_COMPONENT( name ) \
    template< typename VectorType > \
    void set ## name( const VectorType& ) \
    { \
    }



// ----------------------------------------------------------------------------------
// PVertex
// ----------------------------------------------------------------------------------

/** \brief
  * Defines simple-most vertex that only consists of a positional attribute.
  *
  * \section CustomVertexFormats Custom Vertex Formats
  *
  * It is easy to define custom vertex formats. The procedure is best explained with
  * an example. Lets assume you want to define a vertex that has additional
  * properties for normal vectors and 2D texture coordinates.
  *
  * The first step is to define the missing vertex components. The \ref VertexNormal
  * type already provides a vertex component for normal vectors, so lets define a
  * component for 2D texture coordinates:
  *
  *     \code
  *     struct VertexTexCoord2
  *     {
  *         float u, v;
  *     };
  *     \endcode
  *
  * It is necessary that a vertex component is implemented as a POD, i.e. *plain old
  * data type*. Virtual methods would mess up the memory layout. However, you might
  * define a constructor that initializes default values, if you wanted.
  *
  * The next step is to compose the vertex format:
  *
  *     \code
  *     using namespace LibCarna::base;
  *     struct PNT2Vertex // P for Position, N for Normal, T2 for TexCoord2
  *         : public VertexPosition
  *         , public VertexNormal
  *         , public VertexTexCoord2
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
  *     using namespace LibCarna::base;
  *     const VertexAttributes PNT2Vertex::attributes = []()->VertexAttributes
  *     {
  *         using LibCarna::base::VertexAttribute;  // msvc++ requires us to repeat this
  *         std::vector< VertexAttribute > attributes;
  *         attributes.push_back( VertexAttribute( 0, 4, VertexAttribute::TYPE_FLOAT ) );
  *         attributes.push_back( VertexAttribute( 4, 4, VertexAttribute::TYPE_FLOAT ) );
  *         attributes.push_back( VertexAttribute( 8, 2, VertexAttribute::TYPE_FLOAT ) );
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
  *   - Attribute 3 starts at offset 8 and has 2 components, namely the
  *     \c u, \c v of the texture coordinates vector.
  *
  * When writing your shader, you must declare the vertex format consistently:
  *
  *     \code
  *     layout( location = 0 ) in vec4 inPosition;
  *     layout( location = 1 ) in vec4 inNormal;
  *     layout( location = 2 ) in vec2 inTexCoord;
  *     \endcode
  *
  * \author Leonid Kostrykin
  */
struct LIBCARNA PVertex
    : public VertexPosition
{
    /** \brief
      * Holds the declaration of the \ref CustomVertexFormats "vertex format".
      */
    static const VertexAttributes attributes;

    /** Declares missing component.
      */
    VERTEX_NULL_COMPONENT( Normal );

    /** Declares missing component.
      */
    VERTEX_NULL_COMPONENT( Color  );
};



// ----------------------------------------------------------------------------------
// PNVertex
// ----------------------------------------------------------------------------------

/** \brief
  * Defines vertex that consists of the two attributes position and normal vector.
  *
  * \author Leonid Kostrykin
  */
struct LIBCARNA PNVertex
    : public VertexPosition
    , public VertexNormal
{
    /** \brief
      * Holds the declaration of the \ref CustomVertexFormats "vertex format".
      */
    static const VertexAttributes attributes;

    /** Declares missing component.
      */
    VERTEX_NULL_COMPONENT( Color );
};



// ----------------------------------------------------------------------------------
// PCVertex
// ----------------------------------------------------------------------------------

/** \brief
  * Defines vertex that consists of the two attributes position and color.
  *
  * \author Leonid Kostrykin
  */
struct LIBCARNA PCVertex
    : public VertexPosition
    , public VertexColor
{
    /** \brief
      * Holds the declaration of the \ref CustomVertexFormats "vertex format".
      */
    static const VertexAttributes attributes;

    /** Declares missing component.
      */
    VERTEX_NULL_COMPONENT( Normal );
};



}  // namespace LibCarna :: base

}  // namespace LibCarna

#endif // VERTEX_H_6014714286
