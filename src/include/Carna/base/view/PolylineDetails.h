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

#ifndef POLYLINE_DETAILS_H_6014714286
#define POLYLINE_DETAILS_H_6014714286

/** \file   PolylineDetails.h
  * \brief  Defines \ref Carna::base::view::Polyline::Details
  */

#include <Carna/base/view/Polyline.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// Polyline :: Details
// ----------------------------------------------------------------------------------

class Polyline::Details
{

    NON_COPYABLE

public:

    explicit Details( Polyline& self, Polyline::Type lineTyper );

    Polyline& self;


    static base::qt::RotatingColor nextColor;

    bool looped;

    bool withBorder;

    bool visible;


    typedef std::deque< Association< Polyline3DVertex >* > Nodes;

    Nodes nodes;

}; // Polyline :: Details



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // POLYLINE_DETAILS_H_6014714286
