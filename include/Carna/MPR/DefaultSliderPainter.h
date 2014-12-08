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

#ifndef AXIALPLANE_DEFAULTSLIDERPAINTER_H_6014714286
#define AXIALPLANE_DEFAULTSLIDERPAINTER_H_6014714286

/** \file   DefaultSliderPainter.h
  * \brief  Defines \ref Carna::MPR::DefaultSliderPainter.
  */

#include <Carna/MPR/AbstractSliderPainter.h>

namespace Carna
{

namespace MPR
{



// ----------------------------------------------------------------------------------
// MPR :: DefaultSliderPainter
// ----------------------------------------------------------------------------------

/** \brief  Implementation of \ref AbstractSliderPainter, that paints all sliders.
  *
  * \author Leonid Kostrykin
  * \date   26.9.11 - 21.8.12
  */
class CARNA_EXPORT DefaultSliderPainter : public AbstractSliderPainter
{

public:

    /** \brief  Instantiates.
      */
    DefaultSliderPainter();


    virtual void paint( const base::Vector& ) override;

}; // MPR :: DefaultSliderPainter



}  // namespace Carna :: MPR

}  // namespace Carna

#endif // AXIALPLANE_DEFAULTSLIDERPAINTER_H_6014714286
