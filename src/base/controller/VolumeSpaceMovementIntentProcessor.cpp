/*
 *  Copyright (C) 2010 - 2014 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#include <Carna/base/controller/VolumeSpaceMovementIntentProcessor.h>
#include <Carna/base/controller/MovementIntent.h>
#include <Carna/base/model/Position.h>
#include <Carna/base/view/Renderer.h>
#include <Carna/base/view/SceneProvider.h>

namespace Carna
{

namespace base
{

namespace controller
{



// ----------------------------------------------------------------------------------
// VolumeSpaceMovementIntentProcessor
// ----------------------------------------------------------------------------------

VolumeSpaceMovementIntentProcessor::VolumeSpaceMovementIntentProcessor( const QPoint& referencePoint, const view::Renderer& renderer )
    : inverseTransform( computeInverseTransform( renderer ) )
    , renderer( renderer )
    , referencePoint( referencePoint )
{
}


Transformation VolumeSpaceMovementIntentProcessor::computeInverseTransform( const view::Renderer& renderer )
{
    const Transformation volumeToScreen = renderer.projectionTransformation() * renderer.volumeViewTransformation();
    const Transformation screenToVolume = volumeToScreen.inverse();

    return Transformation().scale( 2., 2., 2. )
         * screenToVolume
         * Transformation().scale(  1. / renderer.shortestTargetSide()
                                 , -1. / renderer.shortestTargetSide()
                                 ,  1. );
}


void VolumeSpaceMovementIntentProcessor::processIntent( const MovementIntent& intent )
{
    const Vector origin_volumespace = inverseTransform * Vector( referencePoint.x()
                                                               , referencePoint.y()
                                                               , 0 );

    const Vector destination_volumespace = inverseTransform * Vector( intent.screenspaceDestination.x()
                                                                    , intent.screenspaceDestination.y()
                                                                    , 0 );

    const model::Position position_delta = model::Position::fromVolumeUnits( renderer.provider.scene, destination_volumespace - origin_volumespace );
    processMovement( position_delta );
    referencePoint = intent.screenspaceDestination;
}



}  // namespace Carna :: base :: controller

}  // namespace Carna :: base

}  // namespace Carna
