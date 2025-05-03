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

#include <LibCarna/base/math.h>

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
class Span
{

public:
    
    /** \brief
      * Instantiates.
      */
    Span();

    /** \overload
      */
    Span( const T& first, const T& last );
    
    /** \brief
      * Copies \a other.
      */
    Span( const Span< T >& other );

    /** \brief
      * Returns whether this span equals \a other.
      */
    bool operator==( const Span< T >& other ) const;

    /** \brief
      * Compares this span to \a other.
      *
      * The ordering is determined by the spans' `first` attribute. If they are equal
      * in `first`, the ordering is done through comparison of the `last` attributes.
      */
    bool operator<( const Span< T >& other ) const;

    /** \brief
      * Holds the \f$a\f$ of \f$\left[a, b\right]\f$.
      */
    T first;

    /** \brief
      * Holds the \f$b\f$ of \f$\left[a, b\right]\f$.
      */
    T last;

}; // Span


template< typename T >
Span< T >::Span()
    : first( T() ), last( T() )
{
}


template< typename T >
Span< T >::Span( const T& first, const T& last )
    : first( first ), last( last )
{
}


template< typename T >
Span< T >::Span( const Span< T >& other )
    : first( other.first )
    , last( other.last )
{
}


template< typename T >
bool Span< T >::operator==( const Span< T >& other ) const
{
    return isEqual( first, other.first ) && isEqual( last, other.last );
}


template< typename T >
bool Span< T >::operator<( const Span< T >& other ) const
{
    return first != other.first ? first < other.first : last < other.last;
}



}  // namespace Carna :: base :: math

}  // namespace Carna :: base

}  // namespace Carna

#endif // SPAN_H_6014714286
