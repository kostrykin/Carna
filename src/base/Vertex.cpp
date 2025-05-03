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

#include <LibCarna/base/Vertex.h>
#include <vector>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// VertexPosition
// ----------------------------------------------------------------------------------

VertexPosition::VertexPosition()
    : x( 0 )
    , y( 0 )
    , z( 0 )
    , w( 1 )
{
}



// ----------------------------------------------------------------------------------
// VertexNormal
// ----------------------------------------------------------------------------------

VertexNormal::VertexNormal()
    : nx( 0 )
    , ny( 0 )
    , nz( 0 )
    , nw( 0 )
{
}



// ----------------------------------------------------------------------------------
// VertexColor
// ----------------------------------------------------------------------------------

VertexColor::VertexColor()
    : r( 1 )
    , g( 1 )
    , b( 1 )
    , a( 1 )
{
}



// ----------------------------------------------------------------------------------
// PVertex
// ----------------------------------------------------------------------------------

const VertexAttributes PVertex::attributes = []()->VertexAttributes
{
    using Carna::base::VertexAttribute;
    std::vector< VertexAttribute > attributes;
    attributes.push_back( VertexAttribute( 0, 4, VertexAttribute::TYPE_FLOAT ) );
    return attributes;
}();



// ----------------------------------------------------------------------------------
// PNVertex
// ----------------------------------------------------------------------------------

const VertexAttributes PNVertex::attributes = []()->VertexAttributes
{
    using Carna::base::VertexAttribute;
    std::vector< VertexAttribute > attributes;
    attributes.push_back( VertexAttribute( 0, 4, VertexAttribute::TYPE_FLOAT ) );
    attributes.push_back( VertexAttribute( 1, 4, VertexAttribute::TYPE_FLOAT ) );
    return attributes;
}();



// ----------------------------------------------------------------------------------
// PCVertex
// ----------------------------------------------------------------------------------

const VertexAttributes PCVertex::attributes = []()->VertexAttributes
{
    using Carna::base::VertexAttribute;
    std::vector< VertexAttribute > attributes;
    attributes.push_back( VertexAttribute( 0, 4, VertexAttribute::TYPE_FLOAT ) );
    attributes.push_back( VertexAttribute( 1, 4, VertexAttribute::TYPE_FLOAT ) );
    return attributes;
}();



}  // namespace Carna :: base

}  // namespace Carna
