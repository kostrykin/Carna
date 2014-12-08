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

#ifndef AXIALPLANE_SLIDERTYPE_H_6014714286
#define AXIALPLANE_SLIDERTYPE_H_6014714286

/** \file   SliderType.h
  * \brief  Defines \ref Carna::MPR::SliderType.
  */

#include <Carna/Carna.h>

namespace Carna
{

namespace MPR
{



// ----------------------------------------------------------------------------------
// MPR :: SliderType
// ----------------------------------------------------------------------------------

/** \brief  Logical description of a plane slider.
  *
  * \author Leonid Kostrykin
  * \date   26.9.11 - 1.3.12
  */
class CARNA_EXPORT SliderType
{

public:

    /** \brief  Lists valid \ref CoordinateSystems "volume space" axis.
      */
    enum Axis
    {
        none = -1,
        x    =  0,
        y    =  1,
        z    =  2
    };

    /** \brief  Lists valid \ref CoordinateSystems "screen space" orientations.
      */
    enum Orientation
    {
        undefined  = -1,
        horizontal =  0,
        vertical   =  1
    };


    /** \brief  Instantiates.
      */
    SliderType( Axis axis, Orientation orientation )
        : axis( axis )
        , orientation( orientation )
    {
    }


    /** \brief  Holds the \ref CoordinateSystems "volume space" axis.
      */
    Axis axis;

    /** \brief  Holds the \ref CoordinateSystems "screen space" orientation.
      */
    Orientation orientation;


    /** \brief  Holds a null object.
      */
    const static SliderType null;


    /** \brief  Relational operator
      */
    bool operator==( const SliderType& sl ) const
    {
        return axis == sl.axis && orientation == sl.orientation;
    }

    /** \brief  Relational operator
      */
    bool operator!=( const SliderType& sl ) const
    {
        return !( ( *this ) == sl );
    }

}; // MPR :: SliderType



}  // namespace Carna :: MPR

}  // namespace Carna

#endif // AXIALPLANE_SLIDERTYPE_H_6014714286
