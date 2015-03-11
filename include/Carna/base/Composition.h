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

#ifndef COMPOSITION_H_6014714286
#define COMPOSITION_H_6014714286

/** \file   Composition.h
  * \brief  Defines \ref Carna::base::Composition.
  */

#include <Carna/base/Association.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Composition
// ----------------------------------------------------------------------------------

/** \brief  Represents a composition.
  *
  * If an object A \em composes an object B, it means that the lifetime of object B
  * cannot exceed the lifetime of object A: Therefor, object B is \em part \em of object A.
  * One and the same object cannot be part of more than one object at the same time.
  *
  * \see
  * In opposite to that, one and the same object may be \ref Aggregation "aggregated"
  * by any number of other objects at the same time.
  *
  * Saying that object B is part of object A implies that object A has the ownership
  * over object B and is therefor responsible of deleting it at some moment.
  *
  * \author Leonid Kostrykin
  * \date   26.10.11 - 4.10.11
  */
template< typename AssociatedObjectType >
class Composition : public Association< AssociatedObjectType >
{

public:

    /** \brief  Instantiates.
      */
    explicit Composition( AssociatedObjectType* associatedObject )
        : Association< AssociatedObjectType >( associatedObject )
    {
    }


    /** \brief  Releases the associated object if any.
      */
    virtual ~Composition()
    {
        AssociatedObjectType* const associatedObject = Association< AssociatedObjectType >::get();
        if( associatedObject )
        {
            delete associatedObject;
        }
    }

}; // Composition



}  // namespace Carna :: base

}  // namespace Carna

#endif // COMPOSITION_H_6014714286
