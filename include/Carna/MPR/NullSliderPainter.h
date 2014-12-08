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

#ifndef AXIALPLANE_NULLSLIDERPAINTER_H_6014714286
#define AXIALPLANE_NULLSLIDERPAINTER_H_6014714286

/** \file   NullSliderPainter.h
  * \brief  Defines \ref Carna::MPR::NullSliderPainter.
  */

#include <Carna/MPR/SliderPainter.h>

namespace Carna
{

namespace MPR
{



// ----------------------------------------------------------------------------------
// MPR :: NullSliderPainter
// ----------------------------------------------------------------------------------

/** \brief  Null-implementation of \ref SliderPainter.
  *
  * \author Leonid Kostrykin
  * \date   16.11.11
  */
class CARNA_EXPORT NullSliderPainter : public SliderPainter
{

    Q_OBJECT

public:

    /** \brief  Does nothing.
      */
    virtual void paint( const base::Vector& )
    {
    }

}; // MPR :: NullSliderPainter



}  // namespace Carna :: MPR

}  // namespace Carna

#endif // AXIALPLANE_NULLSLIDERPAINTER_H_6014714286
