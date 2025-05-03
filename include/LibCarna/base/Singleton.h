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

#ifndef SINGLETON_H_6014714286
#define SINGLETON_H_6014714286

/** \file   Singleton.h
  * \brief  Defines \ref Carna::base::Singleton.
  */

#include <LibCarna/base/CarnaException.h>
#include <LibCarna/base/noncopyable.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Singleton
// ----------------------------------------------------------------------------------

/** \brief
  * %Singleton base class
  *
  * \param InstanceType
  *     Names the class, which derives from this class.
  *
  * Example of usage:
  *
  * \code
  * class Config : public Carna::base::Singleton< Config >
  * {
  * protected:
  *
  *     friend class Carna::base::Singleton< Config >
  *     Config();
  * };
  * \endcode
  *
  * If you do require a special implementation of the default constructor, than there
  * is no need for the friend class declaration.
  *
  * \author Leonid Kostrykin
  * \date   2.3.11 - 16.3.15
  */
template< typename InstanceType >
class Singleton
{

    NON_COPYABLE

    /** \brief
      * References the only instance from type \a InstanceType.
      */
    static InstanceType* instancePtr;

protected:

    /** \brief
      * Denotes that the instance was created. Default constructor is hidden.
      */
    Singleton()
    {
        LIBCARNA_ASSERT_EX( instancePtr == nullptr, "Multiple singleton instances created." );
        instancePtr = static_cast< InstanceType* >( this );
    }

    /** \brief
      * Deletes the only instance from class \a InstanceType.
      */
    static void reset()
    {
        if( instancePtr != nullptr )
        {
            delete instancePtr;
        }
    }

public:

    /** \brief
      * Denotes the class, that is derived from this class template.
      */
    typedef InstanceType Instance;

    /** \brief
      * Denotes that the instance was deleted.
      */
    virtual ~Singleton()
    {
        instancePtr = nullptr;
    }

    /** \brief
      * Returns the only instance from class \a InstanceType.
      */
    static InstanceType& instance()
    {
        return instancePtr == nullptr ? *new InstanceType() : *instancePtr;
    }

    /** \brief
      * Tells whether the instance from class \a InstanceType currently exists.
      */
    static bool exists()
    {
        return instancePtr != nullptr;
    }

}; // Singleton


template< typename InstanceType >
InstanceType* Singleton< InstanceType >::instancePtr = nullptr;



}  // namespace Carna :: base

}  // namespace Carna

#endif // SINGLETON_H_6014714286
