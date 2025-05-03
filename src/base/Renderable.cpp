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

#include <LibCarna/base/Renderable.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Renderable :: Details
// ----------------------------------------------------------------------------------

struct Renderable::Details
{
    Details( const Geometry& geometry, const math::Matrix4f& modelViewTransform );
    explicit Details( const Details& );

    const Geometry* geometry;

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    math::Matrix4f modelViewTransform;
    math::Matrix4f viewModelTransform;
    bool isViewModelTransformDirty;

    float eyeDistance2;
    bool isEyeDistance2Dirty;
};


Renderable::Details::Details( const Geometry& geometry, const math::Matrix4f& modelViewTransform )
    : geometry( &geometry )
    , modelViewTransform( modelViewTransform )
    , isViewModelTransformDirty( true )
    , isEyeDistance2Dirty( true )
{
}


Renderable::Details::Details( const Details& other )
    : geometry( other.geometry )
    , modelViewTransform( other.modelViewTransform )
    , viewModelTransform( other.viewModelTransform )
    , isViewModelTransformDirty( other.isViewModelTransformDirty )
    , eyeDistance2( other.eyeDistance2 )
    , isEyeDistance2Dirty( other.isEyeDistance2Dirty )
{
}



// ----------------------------------------------------------------------------------
// Renderable
// ----------------------------------------------------------------------------------

Renderable::Renderable( const Geometry& geometry, const math::Matrix4f& modelViewTransform )
    : pimpl( new Details( geometry, modelViewTransform ) )
{
}


Renderable::Renderable( const Renderable& other )
    : pimpl( new Details( *other.pimpl ) )
{
}


Renderable::~Renderable()
{
}


const Geometry& Renderable::geometry() const
{
    return *pimpl->geometry;
}


const math::Matrix4f& Renderable::modelViewTransform() const
{
    return pimpl->modelViewTransform;
}


void Renderable::setModelViewTransform( const math::Matrix4f& modelViewTransform )
{
    pimpl->modelViewTransform = modelViewTransform;
    pimpl->isViewModelTransformDirty = true;
    pimpl->isEyeDistance2Dirty = true;
}


const math::Matrix4f& Renderable::viewModelTransform() const
{
    if( pimpl->isViewModelTransformDirty )
    {
        pimpl->isViewModelTransformDirty = false;
        pimpl->viewModelTransform = pimpl->modelViewTransform.inverse();
    }
    return pimpl->viewModelTransform;
}


float Renderable::eyeDistance2() const
{
    if( pimpl->isEyeDistance2Dirty )
    {
        pimpl->isEyeDistance2Dirty = false;

        /* Recompute the eye-distance.
         */
        if( pimpl->geometry->hasBoundingVolume() )
        {
            /* Transform eye location into the geometry's model space.
             */
            const math::Matrix4f& viewModel = viewModelTransform();
            const math::Vector4f eyeLocationInModelSpace = viewModel * math::Vector4f( 0, 0, 0, 1 );

            /* Compute the geometry-representative point that is close-most to the eye.
             */
            math::Vector3f closemost;
            pimpl->geometry->boundingVolume().computeClosemostPoint( closemost, math::vector3( eyeLocationInModelSpace ) );

            /* Transform close-most point to eye space.
             */
            const math::Matrix4f& modelView = modelViewTransform();
            closemost = math::vector3< float, 4 >( modelView * math::vector4( closemost, 1 ) );

            /* Compute the squared distance.
             */
            pimpl->eyeDistance2 = closemost.squaredNorm();
        }
        else
        {
            /* Compute the distance w.r.t. the center of the geometry node.
             */
            pimpl->eyeDistance2 = math::translationDistance2( pimpl->modelViewTransform );
        }
    }
    return pimpl->eyeDistance2;
}


Renderable& Renderable::operator=( const Renderable& r )
{
    pimpl->geometry = r.pimpl->geometry;
    pimpl->modelViewTransform = r.pimpl->modelViewTransform;
    pimpl->isViewModelTransformDirty = r.pimpl->isViewModelTransformDirty;
    pimpl->isEyeDistance2Dirty = r.pimpl->isEyeDistance2Dirty;
    if( !pimpl->isViewModelTransformDirty )
    {
        pimpl->viewModelTransform = r.pimpl->viewModelTransform;
    }
    if( !pimpl->isEyeDistance2Dirty )
    {
        pimpl->eyeDistance2 = r.pimpl->eyeDistance2;
    }
    return *this;
}



}  // namespace Carna :: base

}  // namespace Carna
