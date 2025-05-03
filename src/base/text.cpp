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

#include <LibCarna/base/text.h>
#include <fstream>

namespace Carna
{

namespace base
{

namespace text
{



// ----------------------------------------------------------------------------------
// Text
// ----------------------------------------------------------------------------------

std::string cat( const std::string& fileName )
{
    std::ifstream in( fileName.c_str(), std::ios::in );
    return std::string( std::istreambuf_iterator< char >( in ), std::istreambuf_iterator< char >() );
}



}  // namespace Carna :: base :: text

}  // namespace Carna :: base

}  // namespace Carna
