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
#include <Carna/base/view/Node.h>
#include <Carna/base/noncopyable.h>
#include <vector>
#include <algorithm>

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

template< class GeometryCompare = std::less< const Geometry* > >
class CARNA_LIB RenderQueue
{

	NON_COPYABLE

	std::vector< const Geometry* > geometries;
	std::size_t nextGeometryIndex;

public:

	const int geometryType;

	RenderQueue( int geometryType );
	
	void build( Node& root );
	
	void rewind();
	
	bool isEmpty() const;
	
	void const Geometry& poll();

}; // RenderQueue


template< class GeometryCompare >
RenderQueue::RenderQueue( int geometryType )
	: geometryType( geometryType )
{
}


template< class GeometryCompare >
void RenderQueue::build( Node& root )
{
	geometries.clear();
	nextGeometryIndex = 0;
	
	// collect all geometries
	root.visitChildren( [&geometries]( const Spatial& spatial )
		{
			const Geometry* const geom = dynamic_cast< const Geometry* >( &spatial );
			if( geom != nullptr )
			{
				geometries.push_back( geom );
			}
		}
	);
	
	// order geometries as required
	std::sort( geometries.begin(), geometries.end(), GeometryCompare() );
}


template< class GeometryCompare >
void RenderQueue::rewind()
{
	nextGeometryIndex = 0;
}


template< class GeometryCompare >
bool RenderQueue::isEmpty() const
{
	return nextGeometryIndex >= geometries.size();
}


template< class GeometryCompare >
void const Geometry& RenderQueue::poll()
{
	CARNA_ASSERT( !isEmpty() );
	return *geometries[ nextGeometryIndex++ ];
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // RENDERQUEUE_H_6014714286
