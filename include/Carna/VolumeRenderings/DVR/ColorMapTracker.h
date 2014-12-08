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

#ifndef DVR_COLORMAPTRACKER_H_6014714286
#define DVR_COLORMAPTRACKER_H_6014714286

/** \file   ColorMapTracker.h
  * \brief  Defines \ref Carna::VolumeRenderings::DVR::ColorMapTracker.
  */

#include <Carna/base/qt/MultiSpanSliderTracker.h>

namespace Carna
{

namespace VolumeRenderings
{

namespace DVR
{



// ----------------------------------------------------------------------------------
// VolumeRenderings :: DVR :: ColorMapTracker
// ----------------------------------------------------------------------------------

/** \brief  Specialization of MultiSpanSliderTracker which also displays the color
  *         assigned to the tracker within a ColorMapEditor.
  *
  * The painter needs to be associated with a ColorMapEditor by the invocation
  * ColorMapSpanPainter::setWidget of to work properly.
  *
  * \author Leonid Kostrykin
  * \date   16.8.11 - 29.8.11
  */
class CARNA_EXPORT ColorMapTracker : public base::qt::MultiSpanSliderTracker
{

public:

    virtual void paint( const base::IntSpan&
                      , unsigned int span_index
                      , Qt::Orientation
                      , QPainter&
                      , Role );

    virtual bool testHit( const QPoint&, Qt::Orientation, Role );


    /** \brief  Associates the painter with some ColorMapEditor.
      */
    void setWidget( ColorMapEditor& widget )
    {
        this->widget = &widget;
    }


private:

    /** \brief  References the widget which this tracker is associated with.
      */
    ColorMapEditor* widget;

}; // VolumeRenderings :: DVR :: ColorMapTracker



}  // namespace Carna :: VolumeRenderings :: DVR

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // DVR_COLORMAPTRACKER_H_6014714286
