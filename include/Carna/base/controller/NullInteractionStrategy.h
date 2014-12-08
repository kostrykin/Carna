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

#ifndef NULLINTERACTIONSTRATEGY_H_6014714286
#define NULLINTERACTIONSTRATEGY_H_6014714286

/** \file   NullInteractionStrategy.h
  * \brief  Defines \ref Carna::base::controller::NullInteractionStrategy.
  */

#include <Carna/Carna.h>
#include <Carna/base/controller/InteractionStrategy.h>

namespace Carna
{

namespace base
{

namespace controller
{



// ----------------------------------------------------------------------------------
// NullInteractionStrategy
// ----------------------------------------------------------------------------------

/** \brief  Null-implementation of \ref InteractionStrategy
  *
  * \author Leonid Kostrykin
  * \date   28.2.12 - 21.3.13
  */
class CARNA_EXPORT NullInteractionStrategy : public InteractionStrategy
{

public:

    /** \brief  Does nothing.
      */
    virtual void event( Visualization& sourceModule, QEvent* event )
    {
    }

}; // NullInteractionStrategy



}  // namespace Carna :: base :: controller

}  // namespace Carna :: base

}  // namespace Carna

#endif // NULLINTERACTIONSTRATEGY_H_6014714286