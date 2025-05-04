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

#ifndef STOPWATCH_H_6014714286
#define STOPWATCH_H_6014714286

/** \file   Stopwatch.h
  * \brief  Defines \ref Carna::base::Stopwatch.
  */

#include <LibCarna/LibCarna.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Stopwatch
// ----------------------------------------------------------------------------------

/** \brief
  * Implements stopwatch using the robust wallclock time from OMP that most compilers
  * support.
  *
  * \author Leonid Kostrykin
  */
class LIBCARNA Stopwatch
{

    NON_COPYABLE
    
    double time0;

public:

    /** \brief
      * Starts the time measurement.
      */
    Stopwatch();
    
    /** \brief
      * Restarts the time measurement.
      */
    void restart();
    
    /** \brief
      * Tells result in seconds from current time measurement.
      */
    double result() const;

}; // Stopwatch



}  // namespace Carna :: base

}  // namespace Carna

#endif // STOPWATCH_H_6014714286
