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

#ifndef RENDERQUEUE_H_6014714286
#define RENDERQUEUE_H_6014714286

#include <LibCarna/LibCarna.h>
#include <LibCarna/base/Node.h>
#include <LibCarna/base/Geometry.h>
#include <LibCarna/base/Renderable.h>
#include <LibCarna/base/math.h>
#include <LibCarna/base/CarnaException.h>
#include <LibCarna/base/noncopyable.h>
#include <vector>
#include <algorithm>
#include <climits>

/** \file   RenderQueue.h
  * \brief  Defines \ref Carna::base::RenderQueue.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// RenderQueue
// ----------------------------------------------------------------------------------

/** \brief
  * Gathers \ref Renderable "renderable" geometry nodes from scene graph and provides
  * those in a particular order, if required.
  *
  * \param RenderableCompare
  *     Binary function that establishes partial order on `%Renderable` objects.
  *     Typical choices are \ref Renderable::BackToFront,
  *     \ref Renderable::FrontToBack or \ref Renderable::VideoResourcesOrder. Use
  *     `void` if no particular order is required.
  *
  * The geometry nodes are only enqueued if their geometry type matches. The matching
  * is done as follows. First a bit-wise *AND* operation is applied to the node's
  * geometry type and the \ref geometryTypeMask "mask" of this `%RenderQueue`
  * instance. Than the result from this operation is compared to the
  * \ref geometryType of this instance. Only if they match, the `%Geometry` node is
  * enqueued.
  *
  * \author Leonid Kostrykin
  * \date   22.2.15 - 17.3.15
  */
template< typename RenderableCompare >
class RenderQueue
{

    NON_COPYABLE

    std::vector< Renderable > renderables;
    std::size_t nextRenderableIndex;

public:

    /** \brief
      * Holds the geometry type that this queue uses for matching geometry nodes.
      */
    const unsigned int geometryType;

    /** \brief
      * Holds the mask that this queue uses for matching geometry nodes.
      */
    const unsigned int geometryTypeMask;

    /** \brief
      * Holds the mask that makes this queue only accept such geometry nodes whose
      * geometry type matches the geometry type of this queue exactly.
      */
    const static unsigned int EXACT_MATCH_GEOMETRY_TYPE_MASK;

    /** \brief
      * Creates new instance that enqueues \ref Geometry scene graph nodes if their
      * geometry type *AND*-linked with \a geometryTypeMask equals the
      * \a geometryType of this `RenderQueue` instance.
      */
    RenderQueue( unsigned int geometryType, unsigned int geometryTypeMask = EXACT_MATCH_GEOMETRY_TYPE_MASK );
    
    /** \brief
      * Rebuilds this queue by gathering matching geometry nodes from children of
      * \a root recursively. The argument \a viewTransform is required for the
      * computation of the \ref ViewSpace "model-view matrix".
      */
    void build( const Node& root, const math::Matrix4f& viewTransform );
    
    /** \brief
      * Rewinds this queue. This is an \f$\mathcal O\left(1\right)\f$ operation in
      * contrast to \ref build, so prefer it whenever possible. You might also need
      * to \ref updateModelViewTransforms if you use this.
      */
    void rewind();
    
    /** \brief
      * Recomputes the \ref Renderable::modelViewTransform "model-view transforms" of
      * all enqueued renderables. This only is neccessary in certain cases when you
      * have \ref rewind "rewinded" this queue previously. Also recomputes the
      * elements order of this queue if necessary.
      */
    void updateModelViewTransforms( const math::Matrix4f& viewTransform );
    
    /** \brief
      * Tells whether this queue has reached it's end.
      */
    bool isEmpty() const;
    
    /** \brief
      * References the next element of the queue and moves ahead. The referenced
      * object stays alive until the queue is deleted or rebuilt.
      *
      * \pre `isEmpty() == false`
      */
    const Renderable& poll();

    /** \brief
      * References the next element of the queue, but does not move ahead. The
      * referenced object stays alive until the queue is deleted or rebuilt.
      *
      * \pre `isEmpty() == false`
      */
    const Renderable& first() const;
    
    /** \brief
      * References the last element of the queue. The referenced object stays alive
      * until the queue is deleted or rebuilt.
      *
      * \pre `isEmpty() == false`
      */
    const Renderable& last() const;

}; // RenderQueue


template< typename RenderableCompare >
const unsigned int RenderQueue< RenderableCompare >::EXACT_MATCH_GEOMETRY_TYPE_MASK = std::numeric_limits< unsigned int >::max();


template< typename RenderableCompare >
RenderQueue< RenderableCompare >::RenderQueue( unsigned int geometryType, unsigned int geometryTypeMask )
    : geometryType( geometryType )
    , geometryTypeMask( geometryTypeMask )
{
}


template< typename RenderableCompare >
struct RenderableSort
{
    static void sort( std::vector< Renderable >& renderables, bool skipIfViewDependent )
    {
        if( renderables.size() >= 2 && ( RenderableCompare::isViewDependent || !skipIfViewDependent ) )
        {
            std::sort( renderables.begin(), renderables.end(), RenderableCompare() );
        }
    }
};


template< >
struct RenderableSort< void >
{
    static void sort( std::vector< Renderable >& renderables, bool skipIfViewDependent )
    {
    }
};


template< typename RenderableCompare >
void RenderQueue< RenderableCompare >::build( const Node& root, const math::Matrix4f& viewTransform )
{
    renderables.clear();
    nextRenderableIndex = 0;
    
    /* Collect all geometries.
     */
    root.visitChildren( true, [&]( const Spatial& spatial )
        {
            const Geometry* const geom = dynamic_cast< const Geometry* >( &spatial );
            if( geom != nullptr && ( geom->geometryType & geometryTypeMask ) == geometryType )
            {
                const math::Matrix4f modelViewTransform = viewTransform * geom->worldTransform();
                renderables.push_back( Renderable( *geom, modelViewTransform ) );
            }
        }
    );
    
    /* Order geometries as required. Do not skip anything.
     */
    RenderableSort< RenderableCompare >::sort( renderables, false );
}


template< typename RenderableCompare >
void RenderQueue< RenderableCompare >::rewind()
{
    nextRenderableIndex = 0;
}


template< typename RenderableCompare >
void RenderQueue< RenderableCompare >::updateModelViewTransforms( const math::Matrix4f& viewTransform )
{
    /* Recompute the model-view transforms.
     */
    std::for_each( renderables.begin(), renderables.end(),
        [&viewTransform]( Renderable& r )
        {
            r.setModelViewTransform( viewTransform * r.geometry().worldTransform() );
        }
    );
    
    /* Order geometries as required. Skip if the order is not view-dependent.
     */
    RenderableSort< RenderableCompare >::sort( renderables, true );
}


template< typename RenderableCompare >
bool RenderQueue< RenderableCompare >::isEmpty() const
{
    return nextRenderableIndex >= renderables.size();
}


template< typename RenderableCompare >
const Renderable& RenderQueue< RenderableCompare >::poll()
{
    LIBCARNA_ASSERT( !isEmpty() );
    return renderables[ nextRenderableIndex++ ];
}


template< typename RenderableCompare >
const Renderable& RenderQueue< RenderableCompare >::first() const
{
    LIBCARNA_ASSERT( !isEmpty() );
    return renderables.front();
}


template< typename RenderableCompare >
const Renderable& RenderQueue< RenderableCompare >::last() const
{
    LIBCARNA_ASSERT( !isEmpty() );
    return renderables.back();
}



}  // namespace Carna :: base

}  // namespace Carna

#endif // RENDERQUEUE_H_6014714286
