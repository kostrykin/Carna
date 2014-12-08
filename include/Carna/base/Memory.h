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

#ifndef MEMORY_H_6014714286
#define MEMORY_H_6014714286

#include <Carna/Carna.h>

/** \file   Memory.h
  * \brief  Defines \ref Carna::base::Memory.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Memory
// ----------------------------------------------------------------------------------

/** \brief  Provides set of memory-related helper functions.
  *
  * \since  \ref v_2_4
  * \date   26.1.14
  * \author Leonid Kostrykin
  */
class CARNA_EXPORT Memory
{

    Memory()
    {
    }

public:

    /** \brief  Tests whether a specified amount of free heap memory is available.
      *
      * \ingroup MemoryTools
      *
      * Acquires the specified amount of memory and releases it immediately.
      *
      * \date   20.4.2011
      * \author Leonid Kostrykin
      */
    static bool testHeapMemory( unsigned int bytes );

    /** \brief  Default exception thrown by
      *         \ref assureHeapMemory when test fails.
      *
      * \ingroup MemoryTools
      *
      * \date   20.4.2011
      * \author Leonid Kostrykin
      */
    class heap_defile : public std::exception
    {

    public:

        /** \brief  Instantiates.
          */
        explicit heap_defile( const std::string& hint = "" )
            : hint( hint )
        {
        }

        /** \brief  Does nothing.
          */
        virtual ~heap_defile() throw()
        {
        }

        /** \brief  Returns some custom hint, which was passed to the constructor.
          */
        virtual const char* what() const throw()
        {
            return hint.c_str();
        }

    private:

        /** \brief  Some custom hint, initialized by the constructor.
          */
        const std::string hint;

    }; // heap_defile

    /** \brief  Tests whether a specified amount of free heap memory is available.
      *
      * \ingroup MemoryTools
      *
      * Throws exception when testHeapMemory fails.
      *
      * \date   20.4.2011
      * \author Leonid Kostrykin
      */
    inline static void assureHeapMemory( unsigned int bytes
                                , const heap_defile exception
                                    = heap_defile( "Not enough memory to finish operation." ) )
    {
        if( !testHeapMemory( bytes ) )
        {
            throw exception;
        }
    }

}; // Memory



}  // namespace Carna :: base

}  // namespace Carna

#endif // MEMORY_H_6014714286
