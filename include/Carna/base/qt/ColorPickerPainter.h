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

#ifndef COLORPICKERPAINTER_H_6014714286
#define COLORPICKERPAINTER_H_6014714286

/** \file   ColorPickerPainter.h
  * \brief  Defines \ref Carna::base::qt::ColorPickerPainter.
  */

#include <Carna/Carna.h>

class QPainter;
class QColor;
class QRect;

namespace Carna
{

namespace base
{

namespace qt
{



// ----------------------------------------------------------------------------------
// ColorPickerPainter
// ----------------------------------------------------------------------------------

/** \brief  Utilized by \ref ColorPicker and \ref WideColorPicker to paint visual
  *         representations for colors and alpha values.
  *
  * Examples:
  *
  * \image  html    ColorPicker01.png
  *
  * \author Leonid Kostrykin
  * \date   28.7.11
  */
class CARNA_EXPORT ColorPickerPainter
{

public:

    /** \brief  Paints box of given size, filled with given color.
      */
    void paint( QPainter& painter
              , const QRect& rect
              , const QColor& color
              , bool ignoreAlpha = false );

}; // ColorPickerPainter



}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna

#endif // COLORPICKERPAINTER_H_6014714286
