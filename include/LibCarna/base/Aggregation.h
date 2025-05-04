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

#ifndef AGGREGATION_H_6014714286
#define AGGREGATION_H_6014714286

/** \file   Aggregation.h
  * \brief  Defines \ref LibCarna::base::Aggregation.
  */

#include <LibCarna/base/Association.h>

namespace LibCarna
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
      * pointer. For internal usage only. Use \ref NULL_PTR instead.
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



}  // namespace LibCarna :: base

}  // namespace LibCarna

#endif // AGGREGATION_H_6014714286
