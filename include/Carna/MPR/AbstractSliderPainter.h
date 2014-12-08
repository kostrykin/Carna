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

#ifndef AXIALPLANE_ABSTRACTSLIDERPAINTER_H_6014714286
#define AXIALPLANE_ABSTRACTSLIDERPAINTER_H_6014714286

/** \file   AbstractSliderPainter.h
  * \brief  Defines \ref Carna::MPR::AbstractSliderPainter.
  */

#include <Carna/MPR/SliderPainter.h>

namespace Carna
{

namespace MPR
{



// ----------------------------------------------------------------------------------
// MPR :: AbstractSliderPainter
// ----------------------------------------------------------------------------------

/** \brief  Abstract implementation of \ref SliderPainter, which offers methods for
  *         painting sliders as simple lines with configurable width.
  *
  * \author Leonid Kostrykin
  * \date   21.8.12
  */
class CARNA_EXPORT AbstractSliderPainter : public SliderPainter
{

    Q_OBJECT

public:

    /** \brief  Instantiates with a default line width of \c 1 pixel.
      */
    explicit AbstractSliderPainter()
        : width( 1.f )
    {
    }


    /** \brief  Configures the line width.
      *
      * \param  width   must be given in pixels.
      */
    void setWidth( float width )
    {
        this->width = width;

        emit changed();
    }

    /** \brief  Returns the configured line width in pixels.
      */
    float getWidth() const
    {
        return width;
    }


protected:

    /** \brief  Paints slider for panels moving along the x-axis.
      */
    void paintXSlider( float );

    /** \brief  Paints slider for panels moving along the y-axis.
      */
    void paintYSlider( float );

    /** \brief  Paints slider for panels moving along the z-axis.
      */
    void paintZSlider( float );


private:
    
    /** \brief  Holds the configured line width in pixels.
      */
    float width;

}; // MPR :: AbstractSliderPainter



}  // namespace Carna :: MPR

}  // namespace Carna

#endif // AXIALPLANE_ABSTRACTSLIDERPAINTER_H_6014714286
