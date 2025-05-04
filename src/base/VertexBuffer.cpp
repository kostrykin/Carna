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

#include <LibCarna/base/glew.hpp>
#include <LibCarna/base/VertexBuffer.hpp>

namespace LibCarna
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

void VertexBufferBase::copy( const void* bufferPtr, std::size_t bufferSize, std::size_t elementsCount )
{
    valid = true;
    setSize( elementsCount );

    glBindBuffer( GL_ARRAY_BUFFER, id );
    glBufferData( GL_ARRAY_BUFFER, bufferSize, bufferPtr, GL_STATIC_DRAW );
}



}  // namespace LibCarna :: base

}  // namespace LibCarna
