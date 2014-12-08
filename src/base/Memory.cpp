/*
 *  Copyright (C) 2010 - 2014 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#include <Carna/base/Memory.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Memory
// ----------------------------------------------------------------------------------

bool Memory::testHeapMemory( unsigned int bytes )
{
    char* const pcMemTest = new( std::nothrow ) char[ bytes ];
    if( pcMemTest )
    {
        delete[] pcMemTest;
        return true;
    }
    else
    {
        return false;
    }
}



}  // namespace Carna :: base

}  // namespace Carna
