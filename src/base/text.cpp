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

#include <LibCarna/base/text.hpp>
#include <fstream>

namespace LibCarna
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



}  // namespace LibCarna :: base :: text

}  // namespace LibCarna :: base

}  // namespace LibCarna
