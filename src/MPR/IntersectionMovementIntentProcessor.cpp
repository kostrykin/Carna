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

#include <Carna/MPR/IntersectionMovementIntentProcessor.h>
#include <Carna/MPR/AxialPlaneRenderer.h>
#include <Carna/base/model/Position.h>

namespace Carna
{

namespace MPR
{



// ----------------------------------------------------------------------------------
// MPR :: IntersectionMovementIntentProcessor
// ----------------------------------------------------------------------------------

IntersectionMovementIntentProcessor::IntersectionMovementIntentProcessor( const SliderType& sliderType, QPoint referencePoint, AxialPlaneRenderer& renderer )
    : VolumeSpaceMovementIntentProcessor( referencePoint, renderer )
    , sliderType( sliderType )
    , renderer( renderer )
{
}

void IntersectionMovementIntentProcessor::processMovement( const base::model::Position& delta )
{
    base::Vector insection = renderer.intersection();

    insection[ sliderType.axis ] += delta.toVolumeUnits()[ sliderType.axis ];

    renderer.setIntersection( insection );
}



}  // namespace MPR

}  // namespace Carna
