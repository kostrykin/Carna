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

#ifndef AXIALPLANE_NULLSLIDERSCREENSPACEBUFFER_H_6014714286
#define AXIALPLANE_NULLSLIDERSCREENSPACEBUFFER_H_6014714286

/** \file   NullSliderScreenSpaceBuffer.h
  * \brief  Defines \ref Carna::MPR::NullSliderScreenSpaceBuffer.
  */

#include <Carna/MPR/SliderScreenSpaceBuffer.h>
#include <Carna/MPR/SliderType.h>

namespace Carna
{

namespace MPR
{



// ----------------------------------------------------------------------------------
// MPR :: NullSliderScreenSpaceBuffer
// ----------------------------------------------------------------------------------

/** \brief  Null-implementation of \ref SliderScreenSpaceBuffer
  *
  * \author Leonid Kostrykin
  * \date   26.9.11 - 30.9.11
  */
class CARNA_EXPORT NullSliderScreenSpaceBuffer : public SliderScreenSpaceBuffer
{

public:

    /** \brief  Does nothing.
      */
    virtual void update( const base::Vector&, const AxialPlaneRenderer& )
    {
    }

    /** \brief  Always returns ThreePanelSlider::null.
      */
    virtual SliderType hitTest( const QPoint& ) const
    {
        return SliderType::null;
    }

}; // MPR :: NullSliderScreenSpaceBuffer



}  // namespace Carna :: MPR

}  // namespace Carna

#endif // AXIALPLANE_NULLSLIDERSCREENSPACEBUFFER_H_6014714286
