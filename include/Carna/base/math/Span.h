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

#ifndef SPAN_H_6014714286
#define SPAN_H_6014714286

/** \file   Span.h
  * \brief  Defines \ref Carna::base::math::Span.
  */

#include <Carna/base/math.h>

namespace Carna
{

namespace base
{

namespace math
{



// ----------------------------------------------------------------------------------
// Span
// ----------------------------------------------------------------------------------
    
/** \brief
  * Defines an interval \f$\left[a, b\right]\f$ with \f$a,b \in\f$ \a T.
  *
  * \author Leonid Kostrykin
  * \date   12.3.13
  */
template< typename T >
class CARNA_LIB Span
{

public:
    
    /** \brief
      * Instantiates.
      */
    Span()
        : first( T() ), last( T() )
    {
    }

    /** \overload
      */
    Span( const T& first, const T& last )
        : first( first ), last( last )
    {
    }
    
    /** \brief
      * Copies \a other.
      */
    Span( const Span< T >& other )
        : first( other.first )
        , last( other.last )
    {
    }

    /** \brief
      * Returns whether this span equals \a other.
      */
    bool operator==( const Span< T >& other ) const
    {
        return isEqual( first, other.first ) && isEqual( last, other.last );
    }

    /** \brief
      * Compares this span to \a other.
      *
      * The ordering is determined by the spans' `first` attribute. If they are equal
      * in `first`, the ordering is done through comparison of the `last` attributes.
      */
    bool operator<( const Span< T > other ) const
    {
        return first != other.first ? first < other.first : last < other.last;
    }

    /** \brief
      * Holds the \f$a\f$ of \f$\left[a, b\right]\f$.
      */
    T first;

    /** \brief
      * Holds the \f$b\f$ of \f$\left[a, b\right]\f$.
      */
    T last;

}; // Span



}  // namespace Carna :: base :: math

}  // namespace Carna :: base

}  // namespace Carna

#endif // SPAN_H_6014714286
