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

    Color( unsigned char r, unsigned char g, unsigned char b, unsigned char a );

    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;

}; // Color



}  // namespace Carna :: base

}  // namespace Carna

#endif // COLOR_H_6014714286
