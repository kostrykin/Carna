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

#ifndef COLORCODINGSTAGE_H_6014714286
#define COLORCODINGSTAGE_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/GeometryStage.h>

/** \file   ColorCodingStage.h
  * \brief  Defines \ref Carna::base::ColorCodingStage.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// ColorCodingStageBase
// ----------------------------------------------------------------------------------

class ColorCodingStageBase : public GeometryStage< void >
{

public:

    ColorCodingStageBase();

}; // ColorCodingStageBase



// ----------------------------------------------------------------------------------
// ColorCodingStage
// ----------------------------------------------------------------------------------

template< typename X >
class ColorCodingStage : public ColorCodingStageBase
{
}; // ColorCodingStage



}  // namespace Carna :: base

}  // namespace Carna

#endif // COLORCODINGSTAGE_H_6014714286
