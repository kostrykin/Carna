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

#ifndef MPR_DEFAULTAXIALPLANERENDERER_DETAILS_H_6014714286
#define MPR_DEFAULTAXIALPLANERENDERER_DETAILS_H_6014714286

/** \file   DefaultAxialPlaneRendererDetails.h
  * \brief  Defines \ref Carna::MPR::DefaultAxialPlaneRenderer::Details.
  */

#include <Carna/MPR/DefaultAxialPlaneRenderer.h>
#include <deque>

namespace Carna
{

namespace MPR
{



// ----------------------------------------------------------------------------------
// MPR :: DefaultAxialPlaneRenderer :: Details
// ----------------------------------------------------------------------------------

/** \brief  PIMPL-implementation of \ref AxialPlane::DefaultAxialPlaneRenderer "DefaultAxialPlaneRenderer".
  *
  * \author Leonid Kostrykin
  * \date   1.10.12 - 8.3.13
  */
class CARNA_EXPORT DefaultAxialPlaneRenderer :: Details
{

    NON_COPYABLE

public:

    /** \brief  Instantiates.
      */
    Details();

    /** \brief  Releases acquired resources.
      */
    ~Details();


    /** \brief  Holds the volume GL texture object ID.
      */
    unsigned int textureID;

    /** \brief  Holds the slice slider painter.
      */
    std::unique_ptr< SliderPainter > sliderPainter;

    /** \brief  Holds the slice slider screen space positioning.
      */
    std::unique_ptr< SliderScreenSpaceBuffer > sliderScreenSpaceBuffer;

    /** \brief  Holds the current intersection.
      */
    base::Vector intersection;

    /** \brief  Holds the mask's minimal HUV.
      */
    int huv0;

    /** \brief  Holds the mask's maximal HUV.
      */
    int huv1;

    /** \brief	Holds the HUV mask opacity.
      */
    float huvMaskOpacity;

}; // MPR :: DefaultAxialPlaneRenderer :: Details



}  // namespace MPR

}  // namespace Carna

#endif // MPR_DEFAULTAXIALPLANERENDERER_DETAILS_H_6014714286
