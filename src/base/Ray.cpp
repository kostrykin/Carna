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

#include <Carna/base/Ray.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Ray3f
// ----------------------------------------------------------------------------------

void Ray3f::fromEye
    ( unsigned int frameX, unsigned int frameY
    , const Viewport& vp
    , const math::Matrix4f& inverseProjection
    , const math::Matrix4f& inverseView )
{
    using namespace math;

    /* Construct ND-coordinates from frame-space coordinates.
     */
    const float ndX =    ( static_cast< float >( static_cast< signed int >( frameX - vp.marginLeft() ) ) / vp.width () ) * 2 - 1  ;
    const float ndY = -( ( static_cast< float >( static_cast< signed int >( frameY - vp.marginTop () ) ) / vp.height() ) * 2 - 1 );

    /* Compute the clipping coordinates of the corresponding point on the near clipping plane.
     * We can choose the 'w' component here arbitrarily, because the directional vector we are going to compute has no intrinsic depth.
     */
    const float clippingW = 1;
    const Vector4f clippingCoordinates( ndX, ndY, -clippingW, clippingW );

    /* Transform to eye space and fix the 'z' and 'w' components so we get a directional vector,
     * pointing from the eye space origin, namely the eye, to the point on the near clipping plane.
     */
    Vector4f eyeDirection = inverseProjection * clippingCoordinates;
    eyeDirection.z() = -1;
    eyeDirection.w() =  0;

    /* Transform the ray direction and it's origin to world space.
     */
    origin = vector3f( inverseView * Vector4f( 0, 0, 0, 1 ) );
    direction = ( vector3f( inverseView * eyeDirection ) ).normalized();
}


void Ray3f::fromEye
    ( unsigned int frameX, unsigned int frameY
    , const Viewport& vp
    , const Camera& cam )
{
    fromEye( frameX, frameY, vp, cam.projection().inverse(), cam.worldTransform() );
}



}  // namespace Carna :: base

}  // namespace Carna
