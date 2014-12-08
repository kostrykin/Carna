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

#ifndef AXIALPLANE_SLIDERSCREENSPACEBUFFER_H_6014714286
#define AXIALPLANE_SLIDERSCREENSPACEBUFFER_H_6014714286

/** \file   SliderScreenSpaceBuffer.h
  * \brief  Defines \ref Carna::MPR::SliderScreenSpaceBuffer.
  */

#include <Carna/base/Transformation.h>
#include <Carna/base/noncopyable.h>

class QPoint;

namespace Carna
{

namespace MPR
{



// ----------------------------------------------------------------------------------
// MPR :: SliderScreenSpaceBuffer
// ----------------------------------------------------------------------------------

/** \brief  Describes position of panel sliders in \ref CoordinateSystems "screen space".
  *
  * \author Leonid Kostrykin
  * \date   26.9.11 - 19.11.12
  */
class CARNA_EXPORT SliderScreenSpaceBuffer
{

    NON_COPYABLE

public:

    /** \brief  Does nothing.
      */
    virtual ~SliderScreenSpaceBuffer()
    {
    }


    /** \brief  Tells the recent slice sliders position.
      */
    virtual void update( const base::Vector&, const AxialPlaneRenderer& ) = 0;

    /** \brief  Returns the slider which is hit by some given screen space point.
      *
      * \returns    ThreePanelSlider::null if no slider is hit.
      */
    virtual SliderType hitTest( const QPoint& ) const = 0;

}; // MPR :: SliderScreenSpaceBuffer



}  // namespace Carna :: MPR

}  // namespace Carna

#endif // AXIALPLANE_SLIDERSCREENSPACEBUFFER_H_6014714286
