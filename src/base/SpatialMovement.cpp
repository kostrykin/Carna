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

#include <LibCarna/base/SpatialMovement.h>
#include <LibCarna/base/Spatial.h>
#include <LibCarna/base/LibCarnaException.h>
#include <LibCarna/base/math/Ray.h>
#include <LibCarna/base/math/RayPlaneHitTest.h>

namespace LibCarna
{

namespace base
{



// ----------------------------------------------------------------------------------
// SpatialMovement :: Details
// ----------------------------------------------------------------------------------

struct SpatialMovement::Details
{

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    Details( Spatial& movedSpatial, unsigned int frameX, unsigned int frameY, bool perspectival );
    const bool perspectival;

    Spatial* const movedSpatial;

    const math::Matrix4f movedSpatialParentInverseWorldTransform;

    math::Vector3f movementReferencePoint;

    math::Vector3f computeMovementPlaneHitLocation( SpatialMovement& self, unsigned int frameX, unsigned int frameY ) const;

    unsigned int frameXprev;
    unsigned int frameYprev;

    static Spatial* findMovableSpatial( Spatial& candidate );

    static math::Matrix4f computeInverseWorldTransform( Spatial* movedSpatial );

}; // SpatialMovement :: Details


SpatialMovement::Details::Details( Spatial& movedSpatialRef, unsigned int frameX, unsigned int frameY, bool perspectival )
    : perspectival( perspectival )
    , movedSpatial( findMovableSpatial( movedSpatialRef ) )
    , movedSpatialParentInverseWorldTransform( computeInverseWorldTransform( movedSpatial ) )
    , frameXprev( frameX )
    , frameYprev( frameY )
{
}


math::Vector3f SpatialMovement::Details::computeMovementPlaneHitLocation
    ( SpatialMovement& self
    , unsigned int frameX
    , unsigned int frameY ) const
{
    /* Construct plane parallel to projection plane that 'movedSpatial' lies in.
     */
    const math::Vector3f  spatialLocationInWorld = math::vector3< float, 4 >( movedSpatial->worldTransform().col( 3 ) );
    const math::Vector3f camViewDirectionInWorld = math::vector3< float, 4 >( self.cam.worldTransform().col( 2 ) );

    const math::Vector3f movementPlaneNormal       = camViewDirectionInWorld;
    const          float movementPlaneOriginOffset = movementPlaneNormal.dot( spatialLocationInWorld );

    /* Compute ray from eye at given location.
     */
    math::Ray3f ray;
    ray.fromEye( frameX, frameY, self.viewport, self.inverseProjection, self.cam.worldTransform() );

    /* Compute hit location of ray and movement plane.
     */
    math::RayPlaneHitTest< math::Vector3f > hitTest;
    hitTest.compute( ray, movementPlaneNormal, movementPlaneOriginOffset );
    LIBCARNA_ASSERT( hitTest.hitExists() );
    return hitTest.hitLocation();
}


Spatial* SpatialMovement::Details::findMovableSpatial( Spatial& candidate )
{
    if( candidate.isMovable() )
    {
        return &candidate;
    }
    else
    if( candidate.hasParent() )
    {
        return findMovableSpatial( candidate.parent() );
    }
    else
    {
        return nullptr;
    }
}


math::Matrix4f SpatialMovement::Details::computeInverseWorldTransform( Spatial* movedSpatial )
{
    if( movedSpatial == nullptr || !movedSpatial->hasParent() )
    {
        return math::identity4f();
    }
    else
    {
        return movedSpatial->parent().worldTransform().inverse();
    }
}



// ----------------------------------------------------------------------------------
// SpatialMovement
// ----------------------------------------------------------------------------------

SpatialMovement::SpatialMovement
    ( Spatial& movedSpatial
    , unsigned int frameX, unsigned int frameY
    , const Viewport& vp
    , const Camera& cam )

    : pimpl( new Details( movedSpatial, frameX, frameY, !cam.isOrthogonalProjectionHintSet() ) )
    , viewport( vp )
    , cam( cam )
    , inverseProjection( cam.projection().inverse() )
{
    if( pimpl->perspectival )
    {
        /* Compute initial hit location.
         */
        pimpl->movementReferencePoint = pimpl->computeMovementPlaneHitLocation( *this, frameX, frameY );
    }
}


SpatialMovement::~SpatialMovement()
{
}


bool SpatialMovement::update( unsigned int frameX, unsigned int frameY )
{
    if( hasMovedSpatial() && frameX >= 0 && frameY >= 0 && ( frameX != pimpl->frameXprev || frameY != pimpl->frameYprev ) )
    {
        math::Vector3f displacement;
        if( pimpl->perspectival )
        {
            const math::Vector3f movementPlaneHitLocation = pimpl->computeMovementPlaneHitLocation( *this, frameX, frameY );
            displacement = movementPlaneHitLocation - pimpl->movementReferencePoint;
            pimpl->movementReferencePoint = movementPlaneHitLocation;
        }
        else
        {
            const int frameDeltaX = static_cast< int >( frameX ) - static_cast< int >( pimpl->frameXprev );
            const int frameDeltaY = static_cast< int >( frameY ) - static_cast< int >( pimpl->frameYprev );
            
            /* Transform directional vector from frame to clipping coordinates.
             */
            base::math::Vector4f clippingDelta;
            clippingDelta.x() = (  2 * frameDeltaX ) / static_cast< float >( viewport. width() );
            clippingDelta.y() = ( -2 * frameDeltaY ) / static_cast< float >( viewport.height() );
            clippingDelta.z() = clippingDelta.w() = 0;
            
            /* Transform clipping coordinates to world space.
             */
            const base::math::Matrix4f& inverseView = cam.worldTransform();
            displacement = base::math::vector3< float, 4 >( inverseView * inverseProjection * clippingDelta );
        }

        /* Move the object.
         */
        const math::Matrix4f translation = pimpl->movedSpatialParentInverseWorldTransform * math::translation4f( displacement );
        pimpl->movedSpatial->localTransform = translation * pimpl->movedSpatial->localTransform;
        
        /* Update 'previous' frame coordinates.
         */
        pimpl->frameXprev = frameX;
        pimpl->frameYprev = frameY;

        /* Invalidate the subtree.
         */
        Node* subtree = dynamic_cast< Node* >( pimpl->movedSpatial );
        if( subtree == nullptr && pimpl->movedSpatial->hasParent() )
        {
            subtree = &pimpl->movedSpatial->parent();
        }
        if( subtree != nullptr )
        {
            subtree->invalidate();
        }
        
        /* Denote that something has been moved.
         */
        return true;
    }
    else
    {
        /* Denote that nothing was moved.
         */
        return false;
    }
}


bool SpatialMovement::hasMovedSpatial() const
{
    return pimpl->movedSpatial != nullptr;
}


Spatial& SpatialMovement::movedSpatial() const
{
    LIBCARNA_ASSERT( hasMovedSpatial() );
    return *pimpl->movedSpatial;
}



}  // namespace LibCarna :: base

}  // namespace LibCarna
