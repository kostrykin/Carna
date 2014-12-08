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

#ifndef DEFAULTVOLUMERENDERERMODEFRAME_H_6014714286
#define DEFAULTVOLUMERENDERERMODEFRAME_H_6014714286

/** \file   DefaultVolumeRendererModeFrame.h
  * \brief  Defines \ref Carna::VolumeRenderings::DefaultVolumeRendererModeFrame.
  */

#include <Carna/Carna.h>

namespace Carna
{

namespace VolumeRenderings
{



// ----------------------------------------------------------------------------------
// DefaultVolumeRendererModeFrame
// ----------------------------------------------------------------------------------

/** \brief  Represents the time span during which a certain \ref VolumeRendererMode is active.
  *
  * This class is used by \ref DefaultVolumeRenderer for the management of GL states between
  * multiple \ref VolumeRendererMode instances.
  */
class DefaultVolumeRendererModeFrame
{

public:

    /** \brief  Saves current GL state.
      *
      * Also invokes \ref VolumeRendererMode::activated afterwards.
      */
    explicit DefaultVolumeRendererModeFrame( VolumeRendererMode& mode );

    /** \brief  Restores previous GL state.
      *
      * Also invokes \ref VolumeRendererMode::deactivated beforehand.
      */
    ~DefaultVolumeRendererModeFrame();


    /** \brief  References the \ref VolumeRendererMode whose activity this instance represents. 
      */
    VolumeRendererMode& mode;

}; // DefaultVolumeRendererModeFrame



}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // DEFAULTVOLUMERENDERERMODEFRAME_H_6014714286
