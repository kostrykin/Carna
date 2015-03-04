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

#include <Carna/base/glew.h>
#include <Carna/base/VertexBuffer.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// VertexBufferBase
// ----------------------------------------------------------------------------------

VertexBufferBase::VertexBufferBase()
    : BaseBuffer( GL_ARRAY_BUFFER )
{
}

void VertexBufferBase::copy( const void* buffer_ptr, std::size_t buffer_size, std::size_t elements_count )
{
    valid = true;
    setSize( elements_count );

    glBindBuffer( GL_ARRAY_BUFFER, id );
    glBufferData( GL_ARRAY_BUFFER, buffer_size, buffer_ptr, GL_STATIC_DRAW );
}



}  // namespace Carna :: base

}  // namespace Carna
