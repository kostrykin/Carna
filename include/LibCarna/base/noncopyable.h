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

#ifndef NONCOPYABLE_H_6014714286
#define NONCOPYABLE_H_6014714286

/** \file noncopyable.h
  *
  * Defines \ref LibCarna::base::noncopyable and \ref NON_COPYABLE.
  */

namespace LibCarna
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
  * \ref LibCarna::base::noncopyable "noncopyable" super-class: In case some derivation
  * becomes copyable, the \em "is" semantics stay valid.
  */
#define NON_COPYABLE private: ::LibCarna::base::noncopyable __noncopyable;



}  // namespace LibCarna :: base

}  // namespace LibCarna

#endif // NONCOPYABLE_H_6014714286
