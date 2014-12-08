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

#ifndef AXIALPLANE_AXIALSLIDERSCREENSPACEBUFFER_H_6014714286
#define AXIALPLANE_AXIALSLIDERSCREENSPACEBUFFER_H_6014714286

/** \file   AxialSliderScreenSpaceBuffer.h
  * \brief  Defines \ref Carna::MPR::AxialSliderScreenSpaceBuffer.
  */

#include <Carna/MPR/SliderScreenSpaceBuffer.h>
#include <Carna/MPR/SliderType.h>

namespace Carna
{

namespace MPR
{



// ----------------------------------------------------------------------------------
// MPR :: AxialSliderScreenSpaceBuffer
// ----------------------------------------------------------------------------------

/** \brief  \ref SliderScreenSpaceBuffer implementation, suitable for plane sliders
  *         that are aligned along the \ref CoordinateSystems "screen space" axis.
  *
  * \author Leonid Kostrykin
  * \date   26.9.11 - 1.3.12
  */
class CARNA_EXPORT AxialSliderScreenSpaceBuffer : public SliderScreenSpaceBuffer
{

public:

    /** \brief  Instantiates.
      *
      * \param  horizontal          tells which volume space axis is aligned along
      *                             the x-axis in screen space.
      * \param  vertical            tells which volume space axis is aligned along
      *                             the y-axis in screen space.
      * \param  hitTestTolerance    specifies the \f$\theta\f$ in \f$ \| p - q \|
      *                             \leq \theta \f$ which is the condition evaluated
      *                             by AxialSliderScreenSpaceBuffer::hitTest
      *                             with \f$ p \f$ being the given point, \f$ q \f$
      *                             being the closest point to \f$ p \f$ on the
      *                             evaluated axis, in pixels.
      *
      * \see    \ref CoordinateSystems
      */
    AxialSliderScreenSpaceBuffer( SliderType::Axis horizontal
                                , SliderType::Axis vertical
                                , unsigned int hitTestTolerance = 2 )
        : horizontal( horizontal )
        , vertical( vertical )
        , hitTestTolerance( hitTestTolerance )
    {
    }


    virtual void update
        ( const base::Vector& position
        , const AxialPlaneRenderer& renderer ) override;

    virtual SliderType hitTest( const QPoint& ) const override;


private:

    /** \brief  Holds the \ref CoordinateSystems "volume space" axis, which is aligned
      *         along the x-axis in screen space.
      */
    const SliderType::Axis horizontal;

    /** \brief  Holds the \ref CoordinateSystems "volume space" axis, which is aligned
      *         along the y-axis in screen space.
      */
    const SliderType::Axis vertical;


    /** \brief  Holds the recent known position the horizontal slice slider in pixels.
      */
    int x;

    /** \brief  Holds the recent known position the vertical slice slider in pixels.
      */
    int y;


    const unsigned int hitTestTolerance;

}; // MPR :: AxialSliderScreenSpaceBuffer



}  // namespace Carna :: MPR

}  // namespace Carna

#endif // AXIALPLANE_AXIALSLIDERSCREENSPACEBUFFER_H_6014714286
