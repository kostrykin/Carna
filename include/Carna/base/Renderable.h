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
#include <Carna/base/GeometryFeature.h>
#include <Carna/base/math.h>

/** \file   Renderable.h
  * \brief  Defines \ref Carna::base::Renderable.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Renderable
// ----------------------------------------------------------------------------------

/** \brief
  * Represents a \ref Geometry object that has been queued into a \ref RenderQueue.
  * The object's \ref ViewSpace "model-view transform" has been computed in
  * particular.
  *
  * Instances of this class are copyable and assignable.
  *
  * \date   22.2.2015
  * \author Leonid Kostrykin
  */
class CARNA_LIB Renderable
{

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    /** \brief
      * Associates \a geometry with \a modelViewTransform.
      */
    Renderable( const Geometry& geometry, const math::Matrix4f& modelViewTransform );

    /** \brief
      * Copies from \a other.
      */
    Renderable( const Renderable& other );

    /** \brief
      * Deletes
      */
    ~Renderable();

    /** \brief
      * References the \ref SceneGraph_SpecialSpatialClasses "geometry node".
      */
    const Geometry& geometry() const;

    /** \brief
      * References the \ref ViewSpace "model-view transform".
      */
    const math::Matrix4f& modelViewTransform() const;

    /** \brief
      * Alters the \ref ViewSpace "model-view transform".
      */
    void setModelViewTransform( const math::Matrix4f& );

    /** \brief
      * Returns the inverse \ref modelViewTransform.
      */
    const math::Matrix4f& viewModelTransform() const;

    /** \brief
      * Returns the squared distance of \ref geometry to the eye. If it has an
      * \ref Geometry::hasBoundingVolume "associated boundary volume", than the
      * distance is computed w.r.t. to
      * \ref BoundingVolume::computeClosemostPoint "its closemost point". Otherwise
      * it is computed w.r.t. its center.
      */
    float eyeDistance2() const;

    /** \brief
      * Assigns from \a other.
      */
    Renderable& operator=( const Renderable& other );

    /** \brief
      * Defines partial order for renderables w.r.t. to their depth in eye space.
      *
      * \param order
      *     establishes order back-to-front for positive \a order and front-to-back
      *     for negative \a order.
      */
    template< int order >
    struct DepthOrder
    {
        const static bool isViewDependent = true;
        bool operator()( const Renderable& l, const Renderable& r ) const;
    };

    /** \brief
      * Defines partial order for renderables w.r.t. to their depth in eye space s.t.
      * distant renderables come before closer renderables.
      */
    typedef DepthOrder< +1 > BackToFront;
    
    /** \brief
      * Defines partial order for renderables w.r.t. to their depth in eye space s.t.
      * closer renderables come before distant renderables.
      */
    typedef DepthOrder< -1 > FrontToBack;

    /** \brief
      * Establishes partial order for renderables s.t. geometries with such
      * \ref GeometryFeature instances, that do
      * \ref GeometryFeature::controlsSameVideoResource "control the same video resource",
      * are grouped together.
      */
    template< unsigned int role >
    struct VideoResourcesOrder
    {
        const static bool isViewDependent = false;
        bool operator()( const Renderable& l, const Renderable& r ) const;
    };

}; // Renderable


template< int order >
bool Renderable::DepthOrder< order >::operator()( const Renderable& l, const Renderable& r ) const
{
    return order * ( l.eyeDistance2() - r.eyeDistance2() ) > 0;
}


template< unsigned int role >
bool Renderable::VideoResourcesOrder< role >::operator()( const Renderable& l, const Renderable& r ) const
{
    const GeometryFeature& gfl = l.geometry().feature( role );
    const GeometryFeature& gfr = r.geometry().feature( role );
    if( gfl.controlsSameVideoResource( gfr ) )
    {
        return false;
    }
    else
    {
        return &gfl < &gfr;
    }
}



}  // namespace Carna :: base

}  // namespace Carna

#endif // RENDERABLE_H_6014714286
