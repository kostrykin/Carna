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
  * \brief  Defines \ref Carna::base::RotatingColor.
  */

#include <Carna/Carna.h>
#include <Carna/base/Color.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// RotatingColor
// ----------------------------------------------------------------------------------

/** \brief
  * Implements a rotating color.
  *
  * \author Leonid Kostrykin
  * \date   28.7.11 - 31.3.15
  */
class CARNA_LIB RotatingColor
{

public:

    /** \brief
      * Identifies supported colors.
      */
    enum ColorIdentifier
    {
        blue  = ( 1 << 0 ),
        green = ( 1 << 1 ),
        red   = ( 1 << 2 ),

        orange = red | green,
        teal   = green | blue,
        violet = red | blue,
        white  = red | green | blue
    };

    /** \brief
      * Instantiates.
      */
    RotatingColor( ColorIdentifier initialColor = blue );

    /** \brief
      * Pre-increments the color.
      */
    RotatingColor& operator++();

    /** \brief
      * Post-increments the color.
      */
    RotatingColor operator++( int )
    {
        RotatingColor rc = RotatingColor( *this );
        ++( *this );
        return rc;
    }

    /** \brief
      * Tells current color.
      */
    operator const Color&() const
    {
        return color;
    }

private:

    unsigned int nextColor;
    Color color;

}; // RotatingColor



}  // namespace Carna :: base

}  // namespace Carna

#endif // ROTATINGCOLOR_H_6014714286
