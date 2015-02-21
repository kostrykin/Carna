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

#ifndef RENDERQUEUE_H_6014714286
#define RENDERQUEUE_H_6014714286

#include <Carna/Carna.h>
#include <queue>

/** \file   RenderQueue.h
  * \brief  Defines \ref Carna::base::view::RenderQueue.
  */

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// RenderQueue
// ----------------------------------------------------------------------------------

template< class Compare = std::less< const Geometry* > >
class CARNA_LIB RenderQueue
{

	std::priority_queue< const Geometry*, std::vector< const Geometry* >, Compare > queue;

public:

	const int geometryType;

	RenderQueue( int geometryType );
	
	void build( Node& root );
	
	bool isEmpty() const;
	
	void const Geometry& poll();

}; // RenderQueue



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // RENDERQUEUE_H_6014714286
