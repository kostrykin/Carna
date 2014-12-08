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

#ifndef AXIALPLANE_INTERSECTIONMOVEMENTINTENTPROCESSOR_H_6014714286
#define AXIALPLANE_INTERSECTIONMOVEMENTINTENTPROCESSOR_H_6014714286

/** \file   IntersectionMovementIntentProcessor.h
  * \brief  Defines \ref Carna::MPR::IntersectionMovementIntentProcessor.
  */

#include <Carna/base/controller/VolumeSpaceMovementIntentProcessor.h>
#include <Carna/MPR/SliderType.h>

namespace Carna
{

namespace MPR
{



// ----------------------------------------------------------------------------------
// MPR :: IntersectionMovementIntentProcessor
// ----------------------------------------------------------------------------------

/** \brief  Maps \ref base::controller::MovementIntent objects to calls to
  *         \ref AxialPlaneRenderer::setIntersection.
  *
  * \author Leonid Kostrykin
  * \date   5.3.12 - 18.11.12
  */
class CARNA_EXPORT IntersectionMovementIntentProcessor : public base::controller::VolumeSpaceMovementIntentProcessor
{

public:

    /** \brief  Defines the type of intersection slider, this intent processor controls.
      */
    const SliderType sliderType;

    /** \brief  References the axial plane renderer, this intent processor controls.
      */
    AxialPlaneRenderer& renderer;


    /** \brief  Instantiates.
      */
    IntersectionMovementIntentProcessor
        ( const SliderType& sliderType
        , QPoint referencePoint
        , AxialPlaneRenderer& renderer );


    virtual void processMovement( const base::model::Position& delta ) override;

}; // MPR :: IntersectionMovementIntentProcessor



}  // namespace Carna :: MPR

}  // namespace Carna

#endif // AXIALPLANE_INTERSECTIONMOVEMENTINTENTPROCESSOR_H_6014714286
