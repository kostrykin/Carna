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

#ifndef MOVEMENTINTENTPROCESSOR_H_6014714286
#define MOVEMENTINTENTPROCESSOR_H_6014714286

/** \file   MovementIntentProcessor.h
  * \brief  Defines \ref Carna::base::controller::MovementIntentProcessor.
  */

#include <Carna/Carna.h>

namespace Carna
{

namespace base
{

namespace controller
{



// ----------------------------------------------------------------------------------
// MovementIntentProcessor
// ----------------------------------------------------------------------------------

/** \brief	Implementations of this abstract class process MovementIntent objects.
  */
class CARNA_EXPORT MovementIntentProcessor
{
    
public:

    /** \brief	Does nothing.
      */
    virtual ~MovementIntentProcessor()
    {
    }


    /** \brief	Processes the given intent.
      */
    virtual void processIntent( const MovementIntent& ) = 0;

}; // MovementIntentProcessor



}  // namespace Carna :: base :: controller

}  // namespace Carna :: base

}  // namespace Carna

#endif // MOVEMENTINTENTPROCESSOR_H_6014714286
