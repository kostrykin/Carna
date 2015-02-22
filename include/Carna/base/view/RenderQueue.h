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
#include <Carna/base/Matrix4f.h>
#include <Carna/base/CarnaException.h>
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

template< typename RenderableCompare >
class CARNA_LIB RenderQueue
{

    NON_COPYABLE

    std::vector< Renderable > renderables;
    std::size_t nextRenderableIndex;

public:

    const int geometryType;

    RenderQueue( int geometryType );
    
    void build( const Camera& cam, const Node& root );
    
    void rewind();
    
    bool isEmpty() const;
    
    const Renderable& poll();

}; // RenderQueue


template< typename RenderableCompare >
RenderQueue::RenderQueue( int geometryType )
    : geometryType( geometryType )
{
}


template< typename RenderableCompare >
void RenderQueue::build( const Camera& cam, const Node& root )
{
    renderables.clear();
    nextRenderableIndex = 0;
    
    // collect all geometries
    root.visitChildren( [&renderables, &cam, geometryType]( const Spatial& spatial )
        {
            const Geometry* const geom = dynamic_cast< const Geometry* >( &spatial );
            if( geom != nullptr && geom->geometryType == geometryType )
            {
                const Matrix4f modelViewTransform = cam.worldTransform().inverse() * geom.worldTransform();
                renderables.push_back( Renderable( geom, modelViewTransform ) );
            }
        }
    );
    
    // order geometries as required
    std::sort( renderables.begin(), renderables.end(), RenderableCompare() );
}


template< typename RenderableCompare >
void RenderQueue::rewind()
{
    nextRenderableIndex = 0;
}


template< typename RenderableCompare >
bool RenderQueue::isEmpty() const
{
    return nextRenderableIndex >= renderables.size();
}


template< typename RenderableCompare >
const Renderable& RenderQueue::poll()
{
    CARNA_ASSERT( !isEmpty() );
    return *renderables[ nextRenderableIndex++ ];
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // RENDERQUEUE_H_6014714286
