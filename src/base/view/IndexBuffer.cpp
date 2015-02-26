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

#include <Carna/base/view/glew.h>
#include <Carna/base/view/IndexBuffer.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// IndexBufferBase
// ----------------------------------------------------------------------------------

const unsigned int IndexBufferBase::TYPE_UINT_8  = GL_UNSIGNED_BYTE;
const unsigned int IndexBufferBase::TYPE_UINT_16 = GL_UNSIGNED_SHORT;
const unsigned int IndexBufferBase::TYPE_UINT_32 = GL_UNSIGNED_INT;

const unsigned int IndexBufferBase::PRIMITIVE_TYPE_TRIANGLES         = GL_TRIANGLES;
const unsigned int IndexBufferBase::PRIMITIVE_TYPE_TRIANGLE_STRIP    = GL_TRIANGLE_STRIP;
const unsigned int IndexBufferBase::PRIMITIVE_TYPE_TRIANGLE_FAN      = GL_TRIANGLE_FAN;
const unsigned int IndexBufferBase::PRIMITIVE_TYPE_LINES             = GL_LINES;
const unsigned int IndexBufferBase::PRIMITIVE_TYPE_LINE_STRIP        = GL_LINE_STRIP;
const unsigned int IndexBufferBase::PRIMITIVE_TYPE_LINE_LOOP         = GL_LINE_LOOP;
const unsigned int IndexBufferBase::PRIMITIVE_TYPE_POINTS            = GL_POINTS;


IndexBufferBase::IndexBufferBase( unsigned int type, unsigned int primitiveType )
    : BaseBuffer( GL_ELEMENT_ARRAY_BUFFER )
    , type( type )
    , primitiveType( primitiveType )
{
}


void IndexBufferBase::copy( const void* buffer_ptr, std::size_t buffer_size, std::size_t elements_count )
{
    valid = true;
    setSize( elements_count );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, id );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, buffer_size, buffer_ptr, GL_STATIC_DRAW );
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
