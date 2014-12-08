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

#ifndef OBJECTMOVEMENTINTENTPROCESSOR_H_6014714286
#define OBJECTMOVEMENTINTENTPROCESSOR_H_6014714286

/** \file   Object3DMovementIntentProcessor.h
  * \brief  Defines \ref Carna::base::controller::Object3DMovementIntentProcessor.
  */

#include <Carna/base/controller/VolumeSpaceMovementIntentProcessor.h>

namespace Carna
{

namespace base
{

namespace controller
{



// ----------------------------------------------------------------------------------
// Object3DMovementIntentProcessor
// ----------------------------------------------------------------------------------

/** \brief	Processes the user's intent to move Object3D instances.
  *
  * \author Leonid Kostrykin
  * \date   5.3.12 - 8.3.12
  */
class CARNA_EXPORT Object3DMovementIntentProcessor : public VolumeSpaceMovementIntentProcessor
{

public:

    /** \brief	References the processed object.
      */
    model::Object3D& object;


    /** \brief	Instantiates.
      */
    Object3DMovementIntentProcessor
        ( model::Object3D& object
        , const QPoint& referencePoint
        , const view::Renderer& renderer );


    /** \brief	Moves the referenced object by the given position delta.
      */
    virtual void processMovement( const model::Position& ) override;

}; // Object3DMovementIntentProcessor



}  // namespace Carna :: base :: controller

}  // namespace Carna :: base

}  // namespace Carna

#endif // OBJECTMOVEMENTINTENTPROCESSOR_H_6014714286
