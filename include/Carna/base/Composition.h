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

/** \brief
  * Represents a composition, i.e. a strong reference. This basically is a
  * `std::unique_ptr` that supports the \ref Association interface.
  *
  * If an object `B` is a *component* of an object `A`, it means that the lifetime of
  * `B` is bounded to the lifetime of `A`: Therefore, `B` is a *part* of object `A`.
  * The same object cannot be part of more than one object at the same time.
  *
  * \see
  * In opposite to this, the same object may be \ref Aggregation "aggregated" by any
  * number of other objects at the same time.
  *
  * Saying that object `B` is part of object `A` implies that object A possesses `B`
  * and is therefore responsible of deleting it at some time.
  *
  * \author Leonid Kostrykin
  * \date   26.10.11 - 17.3.15
  */
template< typename AssociatedObjectType >
class Composition : public Association< AssociatedObjectType >
{

public:

    /** \brief
      * Instantiates.
      */
    explicit Composition( AssociatedObjectType* associatedObject )
        : Association< AssociatedObjectType >( associatedObject )
    {
    }


    /** \brief
      * Deletes the associated object, if there is one.
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
