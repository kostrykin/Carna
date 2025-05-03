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

#ifndef ROTATINGCOLOR_H_6014714286
#define ROTATINGCOLOR_H_6014714286

/** \file   RotatingColor.h
  * \brief  Defines \ref Carna::base::RotatingColor.
  */

#include <LibCarna/LibCarna.h>
#include <LibCarna/base/Color.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// RotatingColor
// ----------------------------------------------------------------------------------

/** \brief
  * Supplies colors from a predefined, distinctive palette.
  *
  * \author Leonid Kostrykin
  * \date   28.7.11 - 31.3.15
  */
class LIBCARNA RotatingColor
{

public:

    /** \brief
      * Identifies supported colors.
      */
    enum ColorIdentifier
    {
        blue  = ( 1 << 0 ), ///< Identifies blue.
        green = ( 1 << 1 ), ///< Identifies green.
        red   = ( 1 << 2 ), ///< Identifies red.

        orange = red | green,       ///< Identifies orange.
        teal   = green | blue,      ///< Identifies teal.
        violet = red | blue,        ///< Identifies violet.
        white  = red | green | blue ///< Identifies white.
    };

    /** \brief
      * Instantiates.
      */
    RotatingColor( ColorIdentifier initialColor = blue );

    /** \brief
      * Holds the initial color.
      */
    const ColorIdentifier initialColor;

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

    /** \brief
      * Resets s.t. the current color becomes the initial again.
      */
    void reset();

private:

    unsigned int nextColor;
    Color color;

}; // RotatingColor



}  // namespace Carna :: base

}  // namespace Carna

#endif // ROTATINGCOLOR_H_6014714286
