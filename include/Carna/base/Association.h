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

#ifndef ASSOCIATION_H_6014714286
#define ASSOCIATION_H_6014714286

/** \file   Association.h
  * \brief  Defines \ref Carna::base::Association.
  */

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Association
// ----------------------------------------------------------------------------------

/** \brief  Represents an association.
  *
  * By using this class it is possible to let the user of some software module decide
  * whether he wants to pass the ownership of some object to that module or not.
  *
  * \see    Take a look at the \ref Carna::base::Scene::Scene "Scene constructor"
  *         for an example.
  * \author Leonid Kostrykin
  * \date   26.10.11 - 4.10.11
  */
template< typename AssociatedObjectType >
class Association
{

    NON_COPYABLE

public:

    /** \brief  Does nothing.
      */
    virtual ~Association()
    {
    }

    /** \brief  Returns a raw pointer to the referenced object.
      */
    AssociatedObjectType* get() const
    {
        return associatedObject;
    }

    /** \brief  Returns a raw pointer to the referenced object.
      */
    AssociatedObjectType* operator->() const
    {
        return this->get();
    }

    /** \brief	Makes this association forget it's referenced object.
      *
      * Using this method it is possible to bypass the default behavior of the association
      * when it is released.
      */
    void forget()
    {
        associatedObject = 0;
    }

protected:

    /** \brief  Instantiates.
      */
    explicit Association( AssociatedObjectType* associatedObject = 0 )
        : associatedObject( associatedObject )
    {
    }

private:

    AssociatedObjectType* associatedObject;

}; // Association



}  // namespace Carna :: base

}  // namespace Carna

#endif // ASSOCIATION_H_6014714286
