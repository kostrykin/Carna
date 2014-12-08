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

#ifndef WIDECOLORPICKER_H_6014714286
#define WIDECOLORPICKER_H_6014714286

/** \file   WideColorPicker.h
  * \brief  Defines \ref Carna::base::qt::WideColorPicker.
  */

#include <Carna/Carna.h>
#include <QWidget>

class QSpinBox;

namespace Carna
{

namespace base
{

namespace qt
{



// ----------------------------------------------------------------------------------
// WideColorPicker
// ----------------------------------------------------------------------------------

/** \brief  Extra wide widget for selecting colors with alpha values.
  *
  * \ingroup    Widgets
  *
  * The alpha value is displayed in a separate field.
  *
  * Examples:
  *
  * \image  html    WideColorPicker01.png
  *
  * \see \ref ColorPicker
  *
  * \author Leonid Kostrykin
  * \date   28.7.11
  */
class CARNA_EXPORT WideColorPicker : public QWidget
{

    Q_OBJECT

public:

    /** \brief  Instantiates.
      */
    explicit WideColorPicker( const QColor& initial, QWidget* parent = nullptr );


    /** \brief  Tells the currently set color.
      */
    const QColor& getColor() const;


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


private slots:

    void updateColor( const QColor& );

    void updateAlpha( int );


private:

    ColorPicker* const colorPicker;

    QSpinBox* const sbAlpha;

}; // WideColorPicker



}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna

#endif // WIDECOLORPICKER_H_6014714286
