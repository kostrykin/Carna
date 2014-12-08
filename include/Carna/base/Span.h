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

#ifndef SPAN_H_6014714286
#define SPAN_H_6014714286

/** \file   Span.h
  * \brief  Defines \ref Carna::base::Span.
  */

#include <Carna/base/Math.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Span
// ----------------------------------------------------------------------------------
    
/** \brief  Defines an interval \f$ [ \mathrm{ first }, \mathrm{ last } ] \f$ with
  * \f$ first, last \in \mathrm T \f$.
  *
  * \author Leonid Kostrykin
  * \date   12.3.13
  */
template< typename T >
class CARNA_EXPORT Span
{

public:
    
    /** \brief  Instantiates.
      */
    Span()
        : first( T() ), last( T() )
    {
    }

    /** \brief  Instantiates.
      */
    Span( const T& first, const T& last )
        : first( first ), last( last )
    {
    }
    
    /** \brief  Instantiates.
      */
    Span( const Span< T >& other )
        : first( other.first )
        , last( other.last )
    {
    }


    /** \brief  Returns whether this span equals \a another span.
      *
      * Since \ref v_2_1_2 the equality is tested through \ref base::Math::isEqual.
      */
    bool operator==( const Span< T >& another ) const
    {
        return base::Math::isEqual( first, another.first ) && base::Math::isEqual( last, another.last );
    }

    /** \brief  Compares this span to \a another.
      *
      * The ordering is determined by the spans' \c first attribute.
      * If they are equal in \c first, the ordering is defined based on the comparison of their \c last attribute.
      * The spans' natural order is implied by the natural order of \a T.
      */
    bool operator<( const Span< T > another ) const
    {
        return first != another.first ? first < another.first : last < another.last;
    }


    /** \brief  Holds \f$ \mathrm{ first } \f$ within \f$ [ \mathrm{ first }, \mathrm{ last } ] \f$.
      */
    T first;

    /** \brief  Holds \f$ \mathrm{ last } \f$ within \f$ [ \mathrm{ first }, \mathrm{ last } ] \f$.
      */
    T last;

}; // Span



}  // namespace Carna :: base

}  // namespace Carna

#endif // SPAN_H_6014714286
