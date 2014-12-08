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

#ifndef INTSPAN_H_6014714286
#define INTSPAN_H_6014714286

/** \file   IntSpan.h
  * \brief  Defines \ref Carna::base::IntSpan.
  */

#include <Carna/base/Span.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// IntSpan
// ----------------------------------------------------------------------------------

/** \brief
  * Defines an interval \f$ [ \mathrm{ first }, \mathrm{ last } ] \f$ with
  * \f$ first, last \in \mathrm Z \f$.
  *
  * \author Leonid Kostrykin
  * \date   5.3.13
  */
typedef Span< int > IntSpan;



}  // namespace Carna :: base

}  // namespace Carna

#endif // INTSPAN_H_6014714286
