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

/** \brief  Interfaces channel overlay behavior definitions.
  *
  * \author Leonid Kostrykin
  * \date   26.7.11 - 26.2.15
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
