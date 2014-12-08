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

#ifndef VOLUMESPACEMOVEMENTINTENTPROCESSOR_H_6014714286
#define VOLUMESPACEMOVEMENTINTENTPROCESSOR_H_6014714286

/** \file   VolumeSpaceMovementIntentProcessor.h
  * \brief  Defines \ref Carna::base::controller::VolumeSpaceMovementIntentProcessor.
  */

#include <Carna/base/controller/MovementIntentProcessor.h>
#include <Carna/base/noncopyable.h>
#include <Carna/base/Transformation.h>
#include <QPoint>

namespace Carna
{

namespace base
{

namespace controller
{



// ----------------------------------------------------------------------------------
// VolumeSpaceMovementIntentProcessor
// ----------------------------------------------------------------------------------

/** \brief  Implementations of this abstract class process \ref MovementIntent objects
  *         as moving something within volume space.
  *
  * \author Leonid Kostrykin
  * \date   5.3.12 - 6.3.13
  */
class CARNA_EXPORT VolumeSpaceMovementIntentProcessor : public MovementIntentProcessor
{

    NON_COPYABLE

public:

    /** \brief	Instantiates.
      */
    VolumeSpaceMovementIntentProcessor
        ( const QPoint& referencePoint
        , const view::Renderer& renderer );

    /** \brief	Does nothing.
      */
    virtual ~VolumeSpaceMovementIntentProcessor()
    {
    }


    /** \brief	Holds the transformation from view space to volume space.
      */
    const Transformation inverseTransform;

    /** \brief	References the associated view.
      */
    const view::Renderer& renderer;


    virtual void processIntent( const MovementIntent& ) override;

    /** \brief	Processes the movement within volume space.
      */
    virtual void processMovement( const model::Position& ) = 0;


private:

    static Transformation computeInverseTransform( const view::Renderer& renderer );

    QPoint referencePoint;

}; // VolumeSpaceMovementIntentProcessor



}  // namespace Carna :: base :: controller

}  // namespace Carna :: base

}  // namespace Carna

#endif // VOLUMESPACEMOVEMENTINTENTPROCESSOR_H_6014714286
