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

#ifndef AXIALPLANE_PARTIALSLIDERPAINTER_H_6014714286
#define AXIALPLANE_PARTIALSLIDERPAINTER_H_6014714286

/** \file   PartialSliderPainter.h
  * \brief  Defines \ref Carna::MPR::PartialSliderPainter.
  */

#include <Carna/MPR/AbstractSliderPainter.h>

namespace Carna
{

namespace MPR
{



// ----------------------------------------------------------------------------------
// MPR :: PartialSliderPainter
// ----------------------------------------------------------------------------------
    
/** \brief  Implementation of \ref AbstractSliderPainter,
  *         that paints only certain sliders, based on their orientation.
  *
  * \author Leonid Kostrykin
  * \date   26.9.11 - 21.8.12
  */
class CARNA_EXPORT PartialSliderPainter : public AbstractSliderPainter
{

    Q_OBJECT

public:

    /** \brief  Instantiates.
      */
    PartialSliderPainter( bool paintX, bool paintY, bool paintZ );


    virtual void paint( const base::Vector& ) override;


    /** \brief  Tells whether the slider of panels moving along the x-axis is painted.
      */
    bool doesPaintX() const
    {
        return paintX;
    }

    /** \brief  Tells whether the slider of panels moving along the y-axis is painted.
      */
    bool doesPaintY() const
    {
        return paintY;
    }

    /** \brief  Tells whether the slider of panels moving along the z-axis is painted.
      */
    bool doesPaintZ() const
    {
        return paintZ;
    }


public slots:

    /** \brief  Sets whether the slider of panels moving along the x-axis is painted.
      */
    void turnPaintX( bool on )
    {
        if( paintX != on )
        {
            paintX = on;
            emit changed();
        }
    }

    /** \brief  Sets whether the slider of panels moving along the y-axis is painted.
      */
    void turnPaintY( bool on )
    {
        if( paintY != on )
        {
            paintY = on;
            emit changed();
        }
    }

    /** \brief  Sets whether the slider of panels moving along the z-axis is painted.
      */
    void turnPaintZ( bool on )
    {
        if( paintZ != on )
        {
            paintZ = on;
            emit changed();
        }
    }


private:

    bool paintX;

    bool paintY;

    bool paintZ;

}; // MPR :: PartialSliderPainter



}  // namespace Carna :: MPR

}  // namespace Carna

#endif // AXIALPLANE_PARTIALSLIDERPAINTER_H_6014714286
