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

#ifndef NULLINTSPANPAINTER_H_6014714286
#define NULLINTSPANPAINTER_H_6014714286

/** \file   NullIntSpanPainter.h
  * \brief  Defines \ref Carna::base::qt::NullIntSpanPainter.
  */

#include <Carna/base/qt/IntSpanPainter.h>

namespace Carna
{

namespace base
{

namespace qt
{



// ----------------------------------------------------------------------------------
// NullIntSpanPainter
// ----------------------------------------------------------------------------------

/** \brief  Null-implementation of \ref IntSpanPainter
  *
  * \author Leonid Kostrykin
  * \date   12.8.11 - 29.8.11
  */
class CARNA_EXPORT NullIntSpanPainter : public IntSpanPainter
{

public:

    /** \brief  Does nothing.
      */
    virtual void paint( QPainter&, const IntSpan&, int ) override
    {
    }

}; // NullIntSpanPainter



}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna

#endif // NULLINTSPANPAINTER_H_6014714286
