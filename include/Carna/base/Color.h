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

class CARNA_LIB Color
{

public:

    const static Color WHITE;
    const static Color WHITE_NO_ALPHA;
    const static Color BLACK;
    const static Color BLACK_NO_ALPHA;
    const static Color RED;
    const static Color RED_NO_ALPHA;
    const static Color GREEN;
    const static Color GREEN_NO_ALPHA;
    const static Color BLUE;
    const static Color BLUE_NO_ALPHA;

    Color( unsigned char r, unsigned char g, unsigned char b, unsigned char a );

    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;

    operator math::Vector4f() const;

    bool operator==( const Color& other ) const;

}; // Color



}  // namespace Carna :: base

}  // namespace Carna

#endif // COLOR_H_6014714286
