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

#ifndef TEXT_H_6014714286
#define TEXT_H_6014714286

#include <Carna/Carna.h>
#include <string>
#include <sstream>

/** \file   Text.h
  * \brief  Defines \ref Carna::base::Text.
  */

namespace Carna
{

namespace base
{

/** \brief
  * Provides text-related helper functions.
  */
namespace text
{



/** \brief
  * Reads \a file in text mode and returns contents.
  */
std::string CARNA_LIB cat( const std::string& file );



/** \brief
  * Performs lexical cast.
  *
  * Example:
  *
  *     \code
  *     using Carna::base::text::lexical_cast;
  *
  *     const unsigned int i = 3;
  *     const  std::string s = lexical_cast<  std::string >( i );
  *     const unsigned int j = lexical_cast< unsigned int >( s );
  *
  *     CARNA_ASSERT( i == j );
  *     \endcode
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



}  // namespace Carna :: base :: text

}  // namespace Carna :: base

}  // namespace Carna

#endif // TEXT_H_6014714286
