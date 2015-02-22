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

#include <Carna/base/view/GLContext.h>
#include <Carna/base/CarnaException.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// GLContext
// ----------------------------------------------------------------------------------

GLContext* GLContext::myCurrent = nullptr;


GLContext::GLContext( bool isDoubleBuffered )
    : isDoubleBuffered( isDoubleBuffered )
{
    if( myCurrent == nullptr )
    {
        myCurrent = this;
    }
}


GLContext::~GLContext()
{
    if( myCurrent == this )
    {
        myCurrent = nullptr;
    }
}


GLContext& GLContext::current()
{
    CARNA_ASSERT( myCurrent != nullptr );
    return *myCurrent;
}


bool GLContext::isActive() const
{
    return myCurrent == this;
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
