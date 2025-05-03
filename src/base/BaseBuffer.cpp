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

#include <LibCarna/base/glew.h>
#include <LibCarna/base/BaseBuffer.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// BaseBuffer
// ----------------------------------------------------------------------------------

static unsigned int createGLBufferObject()
{
    unsigned int id;
    glGenBuffers( 1, &id );
    return id;
}


BaseBuffer::BaseBuffer( unsigned int target )
    : mySize( 0 )
    , valid( false )
    , target( target )
    , id( createGLBufferObject() )
{
}


BaseBuffer::~BaseBuffer()
{
    glDeleteBuffers( 1, &id );
}


void BaseBuffer::setSize( std::size_t size )
{
    mySize = size;
}


std::size_t BaseBuffer::size() const
{
    return mySize;
}


void BaseBuffer::bind() const
{
    glBindBuffer( target, id );
}


bool BaseBuffer::isValid() const
{
    return valid;
}



}  // namespace Carna :: base

}  // namespace Carna
