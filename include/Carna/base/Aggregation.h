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

/** \brief
  * Represents an aggregation, i.e. a weak reference.
  *
  * \author Leonid Kostrykin
  * \date   26.10.11 - 4.3.15
  */
template< typename AssociatedObjectType >
class Aggregation : public Association< AssociatedObjectType >
{

    Aggregation()
        : Association< AssociatedObjectType >( nullptr )
    {
    }

public:

    /** \brief
      * Holds instance of this class template that behaves like a `nullptr` pointer.
      */
    const static Aggregation< AssociatedObjectType > NULL_PTR;

    /** \brief
      * Creates instance of this class template that behaves like a `nullptr`
      * pointer. For internal use only. Use \ref NULL_PTR instead.
      */
    static Aggregation< AssociatedObjectType > createNullPtr()
    {
        return Aggregation< AssociatedObjectType >();
    }

    /** \brief
      * Instantiates.
      */
    explicit Aggregation( AssociatedObjectType& associatedObject )
        : Association< AssociatedObjectType >( &associatedObject )
    {
    }

    /** \brief
      * Copies reference from \a other.
      */
    Aggregation( const Aggregation< AssociatedObjectType >& other )
        : Association< AssociatedObjectType >( other.get() )
    {
    }

}; // Aggregation


template< typename AssociatedObjectType >
const Aggregation< AssociatedObjectType > Aggregation< AssociatedObjectType >::NULL_PTR
    ( Aggregation< AssociatedObjectType >::createNullPtr() );



}  // namespace Carna :: base

}  // namespace Carna

#endif // AGGREGATION_H_6014714286
