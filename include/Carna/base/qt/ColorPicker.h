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

#ifndef COLORPICKER_H_6014714286
#define COLORPICKER_H_6014714286

/** \file   ColorPicker.h
  * \brief  Defines \ref Carna::base::qt::ColorPicker.
  */

#include <Carna/Carna.h>
#include <QFrame>
#include <QColor>

namespace Carna
{

namespace base
{

namespace qt
{



// ----------------------------------------------------------------------------------
// ColorPicker
// ----------------------------------------------------------------------------------

/** \brief  Widget for selecting colors with or without alpha values.
  *
  * \ingroup    Widgets
  *
  * Examples:
  *
  * \image  html    ColorPicker01.png
  *
  * \par    User interaction and behavior
  *
  * The alpha value can be adjusted quickly by using the scroll wheel.
  *
  * \par    Details
  *
  * The painting routine is delegated to an instance of \ref ColorPickerPainter.
  *
  * \see \ref WideColorPicker
  *
  * \author Leonid Kostrykin
  * \date   28.7.11
  */
class CARNA_EXPORT ColorPicker : public QFrame
{

    Q_OBJECT

public:

    /** \brief  Instantiates.
      */
    explicit ColorPicker
        ( const QColor& initial
        , bool ignoreAlpha = false
        , QWidget* parent = nullptr );


    /** \brief  Fixed size is used.
      */
    virtual QSize sizeHint() const;

    /** \brief  Tells the currently set color.
      */
    const QColor& getColor() const
    {
        return color;
    }


protected:

    /** \brief  Opens a color picking dialog.
      */
    virtual void mouseReleaseEvent( QMouseEvent* );

    /** \brief  Paints the widget.
      */
    virtual void paintEvent( QPaintEvent* );

    /** \brief  Manipulates the alpha value.
      */
    virtual void wheelEvent( QWheelEvent* ev );


signals:

    /** \brief  The current color may have changed.
      */
    void colorChanged();

    /** \brief  The current color may have changed to the given one.
      */
    void colorChanged( const QColor& );


public slots:

    /** \brief  Sets current color.
      */
    void setColor( const QColor& color );


private:

    QColor color;
    
    const bool ignoreAlpha;

}; // ColorPicker



}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna

#endif // COLORPICKER_H_6014714286
