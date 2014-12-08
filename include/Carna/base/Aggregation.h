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

#ifndef AGGREGATION_H_6014714286
#define AGGREGATION_H_6014714286

/** \file   Aggregation.h
  * \brief  Defines \ref Carna::base::Aggregation.
  */

#include <Carna/base/Association.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Aggregation
// ----------------------------------------------------------------------------------

/** \brief  Represents an aggregation.
  *
  * \author Leonid Kostrykin
  * \date   26.10.11 - 4.10.11
  */
template< typename AssociatedObjectType >
class Aggregation : public Association< AssociatedObjectType >
{

public:

    /** \brief  Instantiates.
      */
    explicit Aggregation( AssociatedObjectType& associatedObject )
        : Association< AssociatedObjectType >( &associatedObject )
    {
    }

}; // Aggregation



}  // namespace Carna :: base

}  // namespace Carna

#endif // AGGREGATION_H_6014714286
