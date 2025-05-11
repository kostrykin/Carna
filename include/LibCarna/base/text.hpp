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

#ifndef TEXT_H_6014714286
#define TEXT_H_6014714286

#include <LibCarna/LibCarna.hpp>
#include <string>
#include <sstream>

/** \file
  * \brief
  * Defines \ref LibCarna::base::text.
  */

namespace LibCarna
{

namespace base
{

/** \brief
  * Provides text-related helper functions.
  */
namespace text
{



// ----------------------------------------------------------------------------------
// cat
// ----------------------------------------------------------------------------------

/** \brief
  * Reads \a file in text mode and returns contents.
  */
std::string LIBCARNA cat( const std::string& file );



// ----------------------------------------------------------------------------------
// lexical_cast
// ----------------------------------------------------------------------------------

/** \brief
  * Performs lexical cast.
  *
  * Example:
  *
  * \code
  * using LibCarna::base::text::lexical_cast;
  *
  * const unsigned int i = 3;
  * const  std::string s = lexical_cast<  std::string >( i );
  * const unsigned int j = lexical_cast< unsigned int >( s );
  *
  * LIBCARNA_ASSERT( i == j );
  * \endcode
  */
template< typename To, typename From >
To lexical_cast( const From& from )
{
    To to;
    std::stringstream ss;
    ss << from;
    ss >> to;
    return to;
}



}  // namespace LibCarna :: base :: text

}  // namespace LibCarna :: base

}  // namespace LibCarna

#endif // TEXT_H_6014714286
