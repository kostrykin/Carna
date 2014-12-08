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

#ifndef AXIALPLANE_SLIDERPAINTER_H_6014714286
#define AXIALPLANE_SLIDERPAINTER_H_6014714286

/** \file   SliderPainter.h
  * \brief  Defines \ref Carna::MPR::SliderPainter.
  */

#include <Carna/base/Transformation.h>
#include <Carna/base/noncopyable.h>

namespace Carna
{

namespace MPR
{



// ----------------------------------------------------------------------------------
// MPR :: SliderPainter
// ----------------------------------------------------------------------------------

/** \brief  Abstract base class for plane sliders visualization
  *
  * \author Leonid Kostrykin
  * \date   26.9.11 - 1.3.12
  */
class CARNA_EXPORT SliderPainter : public QObject
{

    Q_OBJECT

    NON_COPYABLE

public:

    /** \brief  Does nothing.
      */
    virtual ~SliderPainter()
    {
    }


    /** \brief  Paints panel sliders at given position.
      */
    virtual void paint( const base::Vector& ) = 0;


signals:

    /** \brief  Emitted when the painter configuration has changed and
      *         a repaint might be required.
      */
    void changed();

}; // MPR :: SliderPainter



}  // namespace Carna :: MPR

}  // namespace Carna

#endif // AXIALPLANE_SLIDERPAINTER_H_6014714286
