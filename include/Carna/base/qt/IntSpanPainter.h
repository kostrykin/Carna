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

#ifndef INTSPANPAINTER_H_6014714286
#define INTSPANPAINTER_H_6014714286

/** \file   IntSpanPainter.h
  * \brief  Defines \ref Carna::base::qt::IntSpanPainter.
  */

#include <Carna/Carna.h>
#include <Carna/base/IntSpan.h>

class QPainter;

namespace Carna
{

namespace base
{

namespace qt
{



// ----------------------------------------------------------------------------------
// IntSpanPainter
// ----------------------------------------------------------------------------------

/** \brief  IntSpan visualization definition
  *
  * Classes implementing this interface are capable of visualizing \ref IntSpan
  * instances within the context of a \ref MultiSpanSlider.
  *
  * \author Leonid Kostrykin
  * \date   12.8.11 - 29.8.11
  */
class CARNA_EXPORT IntSpanPainter
{

public:

    /** \brief  Does nothing.
      */
    virtual ~IntSpanPainter()
    {
    }


    /** \brief  Draws the given IntSpan using supplied \a painter.
      *
      * \param  painter     is the \c QPainter to use for drawing.
      * \param  span        is the span to draw.
      * \param  span_index  tells what number \a span is in the context of the
      *                     MultiSpanSlider it is associated to.
      */
    virtual void paint( QPainter& painter, const IntSpan& span, int span_index ) = 0;

}; // IntSpanPainter



}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna

#endif // INTSPANPAINTER_H_6014714286
