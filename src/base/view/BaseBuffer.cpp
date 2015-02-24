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
#include <Carna/base/view/BaseBuffer.h>

namespace Carna
{

namespace base
{

namespace view
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
    : target( target )
    , id( createGLBufferObject() )
    , mySize( 0 )
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



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
