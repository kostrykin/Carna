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

#ifndef ASSOCIATION_H_6014714286
#define ASSOCIATION_H_6014714286

/** \file
  * \brief
  * Defines \ref LibCarna::base::Association.
  */

#include <LibCarna/LibCarna.hpp>
#include <LibCarna/base/noncopyable.hpp>

namespace LibCarna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Association
// ----------------------------------------------------------------------------------

/** \brief
  * Represents an association.
  *
  * By using this class it is possible to let the user of some software module decide
  * whether he wants to pass the ownership of some object to that module or not.
  *
  * \author Leonid Kostrykin
  */
template< typename AssociatedObjectType >
class Association
{

    NON_COPYABLE

public:

    /** \brief
      * Does nothing.
      */
    virtual ~Association()
    {
    }

    /** \brief
      * Returns raw pointer to the referenced object.
      */
    AssociatedObjectType* get() const
    {
        return associatedObject;
    }

    /** \overload
      */
    AssociatedObjectType* operator->() const
    {
        return this->get();
    }

    /** \brief
      * Returns raw reference to the referenced object.
      */
    AssociatedObjectType& operator*() const
    {
        return *this->get();
    }
    
    /** \brief
      * Tells whether any object is referenced by this association.
      */
    operator bool() const
    {
        return this->get() != nullptr;
    }

    /** \brief
      * Makes this association forget it's referenced object.
      *
      * Using this method it is possible to bypass the default behavior of the
      * association when it is released.
      */
    void forget()
    {
        associatedObject = 0;
    }

protected:

    /** \brief
      * Instantiates.
      */
    explicit Association( AssociatedObjectType* associatedObject = nullptr )
        : associatedObject( associatedObject )
    {
    }

private:

    AssociatedObjectType* associatedObject;

}; // Association



}  // namespace LibCarna :: base

}  // namespace LibCarna



/** \brief
  * Tells whether \a l and \a r do reference the same object, that may be \c nullptr.
  *
  * \relates LibCarna::base::Association
  */
template< typename AssociatedObjectType >
bool operator==( const LibCarna::base::Association< AssociatedObjectType >& l, const LibCarna::base::Association< AssociatedObjectType >& r )
{
    return l.get() == r.get();
}



#endif // ASSOCIATION_H_6014714286
