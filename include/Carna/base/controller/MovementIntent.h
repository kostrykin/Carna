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

#ifndef MOVEMENTINTENT_H_6014714286
#define MOVEMENTINTENT_H_6014714286

/** \file   MovementIntent.h
  * \brief  Defines \ref Carna::base::controller::MovementIntent.
  */

#include <Carna/Carna.h>
#include <QPoint>

namespace Carna
{

namespace base
{

namespace controller
{



// ----------------------------------------------------------------------------------
// MovementIntent
// ----------------------------------------------------------------------------------

/** \brief	Represents an user's intent to move something.
  *
  * Is processed by MovementIntentProcessor implementations.
  *
  * \author Leonid Kostrykin
  * \date   8.3.12
  */
class CARNA_EXPORT MovementIntent
{

public:

    /** \brief	Instantiates.
      */
    explicit MovementIntent( const QPoint& screenspaceDestination );


    /** \brief	Describes the destination of the movement within screen space.
      */
    QPoint screenspaceDestination;

}; // MovementIntent



}  // namespace Carna :: base :: controller

}  // namespace Carna :: base

}  // namespace Carna

#endif // MOVEMENTINTENT_H_6014714286