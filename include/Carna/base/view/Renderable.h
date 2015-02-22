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

#ifndef RENDERABLE_H_6014714286
#define RENDERABLE_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/Matrix4f.h>

/** \file   Renderable.h
  * \brief  Defines \ref Carna::base::view::Renderable.
  */

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// Renderable
// ----------------------------------------------------------------------------------

/** \brief  Represents a \ref Geometry object that has been queued into a \ref RenderQueue.
  *
  * Instances of this class are copyable and assignable.
  *
  * \date   22.2.2015
  * \author Leonid Kostrykin
  */
class CARNA_LIB Renderable
{

    const Geometry* myGeometry;

public:

    Matrix4f modelViewTransform;

    Renderable( const Geometry& geometry, const Matrix4f& modelViewTransform );

    const Geometry& geometry() const;

    const static int ORDER_BACK_TO_FRONT = +1;

    const static int ORDER_FRONT_TO_BACK = -1;

    template< int order >
    struct DepthOrder
    {
        bool operator()( const Renderable* l, const Renderable* r ) const;
    };

}; // Renderable


template< int order >
bool Renderable::DepthOrder< order >::operator()( const Renderable* l, const Renderable* r ) const
{
    return order * ( translationDistanceSq( l->modelViewTransform ) - translationDistanceSq( r->modelViewTransform ) );
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // RENDERABLE_H_6014714286
