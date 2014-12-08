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

#ifndef NONCOPYABLE_H_6014714286
#define NONCOPYABLE_H_6014714286

/** \file noncopyable.h
  *
  * Defines \ref Carna::base::noncopyable and \ref NON_COPYABLE.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// noncopyable
// ----------------------------------------------------------------------------------

/** \brief  Objects from classes inheriting this cannot be copied.
  *
  * In application, \ref NON_COPYABLE is to be preferred.
  * See documentation for reasons.
  *
  * Inspired by boost::noncopyable.
  *
  * \author Leonid Kostrykin
  * \date   2.3.2011
  */
class noncopyable
{

private:

    /** \brief  Disabled copy constructor
      */
    noncopyable( const noncopyable& )
    {
    }

    /** \brief  Disabled assignment operator
      */
    noncopyable& operator=( const noncopyable& )
    {
        return *this;
    }


public:

    /** \brief  Enabled default constructor
      */
    noncopyable()
    {
    }

}; // noncopyable



/** \def    NON_COPYABLE
  *
  * \brief  Features class it is placed in as non-copyable.
  *
  * Example:
  *
  * \code
  * class MyClass
  * {
  *
  *     NON_COPYABLE
  *
  * public:
  *
  *     MyClass();
  *
  * }; // MyClass
  *
  * class MyCopyableClass : public MyClass
  * {
  *
  * public:
  *
  *     MyCopyableClass();
  *
  *     MyCopyableClass( const MyCopyableClass& );
  *
  *     MyCopyableClass& operator=( const MyCopyableClass& );
  *
  * }; // MyCopyableClass
  * \endcode
  *
  * This example also shows why the macro variant is to be preferred in place of the
  * \ref Carna::base::noncopyable "noncopyable" super-class: In case some derivation
  * becomes copyable, the \em "is" semantics stay valid.
  */
#define NON_COPYABLE private: ::Carna::base::noncopyable __noncopyable;



}  // namespace Carna :: base

}  // namespace Carna

#endif // NONCOPYABLE_H_6014714286
