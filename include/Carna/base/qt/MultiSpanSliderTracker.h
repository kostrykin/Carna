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

#ifndef MULTISPANSLIDERTRACKER_H_6014714286
#define MULTISPANSLIDERTRACKER_H_6014714286

/** \file   MultiSpanSliderTracker.h
  * \brief  Defines \ref Carna::base::qt::MultiSpanSliderTracker.
  */

#include <Carna/Carna.h>
#include <Carna/base/IntSpan.h>
#include <Qt>

class QPoint;
class QPainter;

namespace Carna
{

namespace base
{

namespace qt
{



// ----------------------------------------------------------------------------------
// MultiSpanSliderTracker
// ----------------------------------------------------------------------------------

/** \brief  Defines a MultiSpanSlider tracker.
  *
  * \author Leonid Kostrykin
  * \date   12.8.11 - 29.8.11
  */
class CARNA_EXPORT MultiSpanSliderTracker
{

public:

    /** \brief  Does nothing.
      */
    virtual ~MultiSpanSliderTracker();


    /** \brief  Each tracker marks either the beginning or the ending of an interval.
      */
    enum Role
    {
        begin, end
    };


    /** \brief  Paints the tracker with given parameters.
      *
      * \param  span        references the span, this tracker belongs to.
      * \param  span_index  tells what number \a span is in the context of the
      *                     MultiSpanSlider it is associated to.
      * \param  orientation defines whether the tracker is moving horizontally or
      *                     vertically.
      * \param  painter     is the \c QPainter to use for drawing.
      * \param  mode        tells whether the tracker to be drawn marks the beginning
      *                     or the ending of the references span.
      */
    virtual void paint( const IntSpan& span
                      , unsigned int span_index
                      , Qt::Orientation orientation
                      , QPainter& painter
                      , Role mode );

    /** \brief  Tests, whether the tracker is hit by given point.
      *
      * The test does not have to be accurate.
      *
      * \param  point       is the point which is to be tested.
      * \param  orientation defines whether the tracker is moving horizontally or
      *                     vertically.
      * \param  mode        tells whether the tracker being tested marks the beginning
      *                     or the ending of it's associated span.
      */
    virtual bool testHit( const QPoint& point
                        , Qt::Orientation orientation
                        , Role mode );


    /** \brief  Holds the width of the tracker as painted by
      *         MultiSpanSliderTracker::paint and assumed by
      *         MultiSpanSliderTracker::testHit.
      */
    static const int trackerWidth;

    /** \brief  Holds the height of the tracker as painted by
      *         MultiSpanSliderTracker::paint and assumed by
      *         MultiSpanSliderTracker::testHit.
      */
    static const int trackerHeight;

}; // MultiSpanSliderTracker



}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna

#endif // MULTISPANSLIDERTRACKER_H_6014714286
