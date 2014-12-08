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

#ifndef DVR_COLORMAPSPANPAINTER_H_6014714286
#define DVR_COLORMAPSPANPAINTER_H_6014714286

/** \file   ColorMapSpanPainter.h
  * \brief  Defines \ref Carna::VolumeRenderings::DVR::ColorMapSpanPainter.
  */

#include <Carna/base/qt/IntSpanPainter.h>

namespace Carna
{

namespace VolumeRenderings
{

namespace DVR
{



// ----------------------------------------------------------------------------------
// VolumeRenderings :: DVR :: ColorMapSpanPainter
// ----------------------------------------------------------------------------------

/** \brief  Visualizes spans as color gradients.
  *
  * The painter needs to be associated with a ColorMapEditor by the invocation
  * ColorMapSpanPainter::setWidget of to work properly.
  *
  * \author Leonid Kostrykin
  * \date   16.8.11 - 29.8.11
  */
class CARNA_EXPORT ColorMapSpanPainter : public base::qt::IntSpanPainter
{

public:

    virtual void paint( QPainter&, const base::IntSpan&, int span_index );


    /** \brief  Associates the painter with some ColorMapEditor.
      */
    void setWidget( ColorMapEditor& widget )
    {
        this->widget = &widget;
    }


private:

    /** \brief  References the widget which this painter is associated with.
      */
    ColorMapEditor* widget;

}; // VolumeRenderings :: DVR :: ColorMapSpanPainter



}  // namespace Carna :: VolumeRenderings :: DVR

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // DVR_COLORMAPSPANPAINTER_H_6014714286
