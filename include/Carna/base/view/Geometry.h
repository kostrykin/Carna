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

#ifndef GEOMETRY_H_6014714286
#define GEOMETRY_H_6014714286

#include <Carna/base/view/Spatial.h>

/** \file   Geometry.h
  * \brief  Defines \ref Carna::base::view::Geometry.
  */

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// Geometry
// ----------------------------------------------------------------------------------

class CARNA_LIB Geometry : public Spatial
{

public:

	const int geometryType;

	Geometry( int geometryType );

}; // Geometry



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // GEOMETRY_H_6014714286
