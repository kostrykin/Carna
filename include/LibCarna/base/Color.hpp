/*
 *  Copyright (C) 2010 - 2016 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 * 
 * 
 *  Copyright (C) 2021 - 2025 Leonid Kostrykin
 * 
 */

#ifndef COLOR_H_6014714286
#define COLOR_H_6014714286

#include <LibCarna/LibCarna.hpp>
#include <LibCarna/base/noncopyable.hpp>
#include <LibCarna/base/math.hpp>

/** \file   Color.h
  * \brief  Defines \ref LibCarna::base::Color.
  */

namespace LibCarna
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
  */
class LIBCARNA Color
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
      * Compares to \a other.
      */
    bool operator==( const Color& other ) const;
    
    /** \brief
      * Compares to \a other.
      */
    bool operator<( const Color& other ) const;

    /** \brief
      * Assigns values from \a other.
      */
    Color& operator=( const Color& other );

}; // Color



}  // namespace LibCarna :: base

}  // namespace LibCarna

#endif // COLOR_H_6014714286
