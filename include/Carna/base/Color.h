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

#ifndef COLOR_H_6014714286
#define COLOR_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>
#include <Carna/base/math.h>

/** \file   Color.h
  * \brief  Defines \ref Carna::base::Color.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Color
// ----------------------------------------------------------------------------------

/** \brief
  * Represents a color. Objects from this class are copyable and assignable.
  *
  * \author Leonid Kostrykin
  * \date   22.2.15 - 17.3.15
  */
class CARNA_LIB Color
{

public:

    const static Color WHITE;           ///< Holds `Color(255, 255, 255, 255)`.
    const static Color WHITE_NO_ALPHA;  ///< Holds `Color(255, 255, 255, 0)`.
    const static Color BLACK;           ///< Holds `Color(0, 0, 0, 255)`.
    const static Color BLACK_NO_ALPHA;  ///< Holds `Color(0, 0, 0, 0)`.
    const static Color RED;             ///< Holds `Color(255, 0, 0, 255)`.
    const static Color RED_NO_ALPHA;    ///< Holds `Color(255, 0, 0, 0)`.
    const static Color GREEN;           ///< Holds `Color(0, 255, 0, 255)`.
    const static Color GREEN_NO_ALPHA;  ///< Holds `Color(0, 255, 0, 0)`.
    const static Color BLUE;            ///< Holds `Color(0, 0, 255, 255)`.
    const static Color BLUE_NO_ALPHA;   ///< Holds `Color(0, 0, 255, 0)`.

    /** \brief
      * Instantiates from floating point values, clamped to interval
      * \f$\left[0, 1\right]\f$.
      */
    Color( const math::Vector4f& );

    /** \brief
      * Instantiates.
      */
    Color( unsigned char r, unsigned char g, unsigned char b, unsigned char a );

    /** \brief
      * Instantiates with \ref BLACK_NO_ALPHA.
      */
    Color();

    unsigned char r;    ///< Holds the   red component.
    unsigned char g;    ///< Holds the green component.
    unsigned char b;    ///< Holds the  blue component.
    unsigned char a;    ///< Holds the alpha component.

    /** \brief
      * Converts to floating point representation with values in interval
      * \f$\left[0, 1\right]\f$.
      */
    operator math::Vector4f() const;

    /** \brief
      * Assigns values from \a other.
      */
    bool operator==( const Color& other ) const;

}; // Color



}  // namespace Carna :: base

}  // namespace Carna

#endif // COLOR_H_6014714286
