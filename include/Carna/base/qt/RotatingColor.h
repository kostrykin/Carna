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

#ifndef ROTATINGCOLOR_H_6014714286
#define ROTATINGCOLOR_H_6014714286

/** \file   RotatingColor.h
  * \brief  Defines \ref Carna::base::qt::RotatingColor.
  */

#include <Carna/Carna.h>
#include <QColor>

namespace Carna
{

namespace base
{

namespace qt
{



// ----------------------------------------------------------------------------------
// RotatingColor
// ----------------------------------------------------------------------------------

/** \brief  Represents rotating colors.
  *
  * \author Leonid Kostrykin
  * \date   28.7.11
  */
class CARNA_EXPORT RotatingColor
{

public:

    /** \brief  Enumeration of all supported colors
      */
    enum Color
    {
        blue  = ( 1 << 0 ),
        green = ( 1 << 1 ),
        red   = ( 1 << 2 ),

        orange = red | green,
        teal   = green | blue,
        violet = red | blue,
        white  = red | green | blue
    };


    /** \brief  Instantiates.
      */
    explicit RotatingColor( Color initialColor = blue );


    /** \brief  Pre-increments the color.
      */
    RotatingColor& operator++();

    /** \brief  Post-increments the color.
      */
    RotatingColor operator++( int )
    {
        RotatingColor rc = RotatingColor( *this );
        ++( *this );
        return rc;
    }

    /** \brief  Tells current color.
      */
    operator const QColor&() const
    {
        return color;
    }


private:

    /** \brief  Holds a description of the next color.
      */
    unsigned int nextColor;

    /** \brief  Holds current color.
      */
    QColor color;

}; // RotatingColor



}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna

#endif // ROTATINGCOLOR_H_6014714286
