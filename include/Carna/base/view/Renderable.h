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
#include <Carna/base/math.h>

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

    std::unique_ptr< math::Matrix4f > myModelViewTransform;

public:

    Renderable( const Geometry& geometry, const math::Matrix4f& modelViewTransform );

    Renderable( const Renderable& other );

    const Geometry& geometry() const;

    const math::Matrix4f& modelViewTransform() const;

    Renderable& operator=( const Renderable& );

    const static int ORDER_BACK_TO_FRONT = +1;

    const static int ORDER_FRONT_TO_BACK = -1;

    /** \brief
      * Establishes partial order for renderables w.r.t. to their depth in eye space.
      */
    template< int order >
    struct DepthOrder
    {
        bool operator()( const Renderable& l, const Renderable& r ) const;
    };

    /** \brief
      * Establishes partial order for renderables s.t.
      * geometries with same \ref VideoResourceControl instances are grouped together.
      */
    template< unsigned int role >
    struct VideoResourcesOrder
    {
        bool operator()( const Renderable& l, const Renderable& r ) const;
    };

}; // Renderable


template< int order >
bool Renderable::DepthOrder< order >::operator()( const Renderable& l, const Renderable& r ) const
{
    return order * ( math::translationDistanceSq( l.modelViewTransform() ) - math::translationDistanceSq( r.modelViewTransform() ) ) > 0;
}


template< unsigned int role >
bool Renderable::VideoResourcesOrder< role >::operator()( const Renderable& l, const Renderable& r ) const
{
    const VideoResourceControl& vrcL = l.geometry().aggregate( role ).videoResources();
    const VideoResourceControl& vrcR = r.geometry().aggregate( role ).videoResources();
    if( vrcL.isSameResource( vrcR ) )
    {
        return false;
    }
    else
    {
        return &vrcL < &vrcR;
    }
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // RENDERABLE_H_6014714286
