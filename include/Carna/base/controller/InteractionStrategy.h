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

#ifndef INTERACTIONSTRATEGY_H_6014714286
#define INTERACTIONSTRATEGY_H_6014714286

/** \file   InteractionStrategy.h
  * \brief  Defines \ref Carna::base::controller::InteractionStrategy.
  */

#include <Carna/Carna.h>

class QEvent;

namespace Carna
{

namespace base
{

namespace controller
{



// ----------------------------------------------------------------------------------
// InteractionStrategy
// ----------------------------------------------------------------------------------

/** \brief  Defines the controller component of a \ref Visualization "visualization".
  *
  * \author Leonid Kostrykin
  * \date   28.2.12 - 18.3.15
  */
class CARNA_EXPORT InteractionStrategy
{

public:

    /** \brief  Does nothing.
      */
    virtual ~InteractionStrategy()
    {
    }

    /** \brief  Optionally performs some action.
      */
    virtual void event( Visualization& sourceModule, QEvent* event ) = 0;

}; // InteractionStrategy



}  // namespace Carna :: base :: controller

}  // namespace Carna :: base

}  // namespace Carna

#endif // INTERACTIONSTRATEGY_H_6014714286
