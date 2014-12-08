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

#ifndef SINGLETON_H_6014714286
#define SINGLETON_H_6014714286

/** \file   Singleton.h
  * \brief  Defines \ref Carna::base::Singleton.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Singleton
// ----------------------------------------------------------------------------------

/** \brief  Singleton base class
  *
  * \param  Concrete    Names the class, which derives from this class.
  *
  * Example of usage:
  *
  * \code
  * class Config : public Carna::base::Singleton< Config >
  * {
  *
  * protected:
  *
  *     friend class Carna::base::Singleton< Config >;
  *
  *     Config();
  *
  * }; // Config
  * \endcode
  *
  * If you do require a special implementation of the default constructor, than there
  * is no need for the friend class declaration.
  *
  * \author Leonid Kostrykin
  * \date   2.3.2011
  */
template< typename Concrete >
class Singleton
{

protected:

    /** \brief  Default constructor is hidden.
      */
    Singleton()
    {
    }

    /** \brief  Copy constructor is hidden.
      */
    explicit Singleton( const Concrete& )
    {
    }


public:

    /** \brief  Does nothing.
      */
    virtual ~Singleton()
    {
    }


    /** \brief  Returns the only concreteInstance from class \a Concrete
      */
    static Concrete& instance()
    {
        static Concrete concreteInstance;
        return concreteInstance;
    }

}; // Singleton



}  // namespace Carna :: base

}  // namespace Carna

#endif // SINGLETON_H_6014714286
