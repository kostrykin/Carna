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

#include <Carna/base/controller/Object3DMovementIntentProcessor.h>
#include <Carna/base/model/Object3D.h>

namespace Carna
{

namespace base
{

namespace controller
{



// ----------------------------------------------------------------------------------
// Object3DMovementIntentProcessor
// ----------------------------------------------------------------------------------

Object3DMovementIntentProcessor::Object3DMovementIntentProcessor
    ( model::Object3D& object
    , const QPoint& referencePoint
    , const view::Renderer& renderer )

    : VolumeSpaceMovementIntentProcessor( referencePoint, renderer )
    , object( object )
{
}


void Object3DMovementIntentProcessor::processMovement( const model::Position& delta )
{
    object.move( delta );
}



}  // namespace Carna :: base :: controller

}  // namespace Carna :: base

}  // namespace Carna
