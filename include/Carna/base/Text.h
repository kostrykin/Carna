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



// ----------------------------------------------------------------------------------
// Text
// ----------------------------------------------------------------------------------
    
/** \brief  Provides set of text-related helper functions.
  *
  * \since  \ref v_2_4
  * \date   26.1.14
  * \author Leonid Kostrykin
  */
class CARNA_EXPORT Text
{

    Text()
    {
    }

public:

    /** \brief      Reads file in text mode and returns contents.
      *
      * \ingroup    StringTools
      */
    static std::string cat( const std::string& fileName );

    /** \brief  Performs lexical cast.
      *
      * Example:
      * \code
      * using Carna::base::lexical_cast;
      *
      * const unsigned int i = 3;
      * const std::string s = lexical_cast< std::string >( i );
      * const unsigned int j = lexical_cast< unsigned int >( s );
      *
      * assert( i == j );
      * \endcode
      *
      * \ingroup StringTools
      */
    template< typename To, typename From >
    inline static To lexical_cast( const From& from )
    {
        To to;
        std::stringstream ss;
        ss << from;
        ss >> to;
        return to;
    }

}; // Text



}  // namespace Carna :: base

}  // namespace Carna

#endif // TEXT_H_6014714286
