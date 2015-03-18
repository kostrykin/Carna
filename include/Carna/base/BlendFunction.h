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

#ifndef BLENDFUNCTION_H_6014714286
#define BLENDFUNCTION_H_6014714286

/** \file   BlendFunction.h
  * \brief  Defines \ref Carna::VolumeRenderings::MIP::BlendFunction.
  */

#include <Carna/Carna.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// BlendFunction
// ----------------------------------------------------------------------------------

/** \brief
  * Defines how alpha blending is to be performed.
  *
  * \author Leonid Kostrykin
  * \date   22.2.15 - 17.3.15
  */
class CARNA_LIB BlendFunction
{

public:

    BlendFunction( int sourceFactor, int destinationFactor );

    int sourceFactor;

    int destinationFactor;

}; // BlendFunction



}  // namespace Carna :: base

}  // namespace Carna

#endif // BLENDFUNCTION_H_6014714286
