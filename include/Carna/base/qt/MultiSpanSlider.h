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

#ifndef MULTISPANSLIDER_H_6014714286
#define MULTISPANSLIDER_H_6014714286

/** \file   MultiSpanSlider.h
  * \brief  Defines \ref Carna::base::qt::MultiSpanSlider.
  */

#include <Carna/Carna.h>
#include <Carna/base/IntSpan.h>
#include <Carna/base/qt/NullIntSpanPainter.h>
#include <Carna/base/qt/MultiSpanSliderTracker.h>
#include <Carna/base/qt/MultiSpanSliderModelViewMapping.h>
#include <QWidget>
#include <vector>
#include <map>

namespace Carna
{

namespace base
{

namespace qt
{



// ----------------------------------------------------------------------------------
// MultiSpanSlider
// ----------------------------------------------------------------------------------

/** \brief  Supplies a widget, which provides an intuitive way of creating, editing
  *         and removing non-overlapping intervals within some given boundaries
  *         through it's user interface.
  *
  * \ingroup    Widgets
  *
  * Example:
  *
  * \image  html    MultiSpanSlider01.png
  *
  * \par    General notes
  *
  * This widget can operate either vertically or horizontally. By default it
  * operates vertically.
  *
  * \todo   Implement horizontal orientation support.
  *
  * In the following documentation \f$ \overline I \f$ is used to describe the set of
  * all legal intervals, \f$ I \f$ is used to identify the \em ascendingly \em sorted
  * set of currently existing intervals.
  *
  * \par    How the view is painted - and how to customize it
  *
  * The widget draws the axis which the trackers are moving along. It also draws the
  * ticks. Drawing trackers is delegated to themselves by calling
  * \ref MultiSpanSliderTracker::paint. This means that by supplying any custom
  * \ref MultiSpanSliderTracker implementation you can easily change the appearance of the
  * trackers. The supplied IntSpanPainter implementation is responsible for visualizing
  * the intervals themselves. By default, a null-implementation is used, which simply
  * does not visualize the spans.
  *
  * \author Leonid Kostrykin
  * \date   12.8.11 - 29.8.11
  */
class CARNA_EXPORT MultiSpanSlider : public QWidget
{

    Q_OBJECT

public:

    /** \brief  Instantiates.
      *
      * \param  orientation defines whether the trackers are moving horizontally or
      *                     vertically.
      * \param  tracker     is the prototype of the trackers to use.
      * \param  spanPainter defines the visualization of spans.
      * \param  parent      is delegated to the \c QWidget constructor.
      */
    explicit MultiSpanSlider
        ( Qt::Orientation orientation = Qt::Vertical
        , MultiSpanSliderTracker* tracker = new MultiSpanSliderTracker()
        , IntSpanPainter* spanPainter = new NullIntSpanPainter()
        , QWidget* parent = nullptr );


    /** \brief  Returns \f$ \min \{ \min J, J \in \overline I \} \f$.
      */
    int getFirst() const
    {
        return first;
    }

    /** \brief  Returns \f$ \max \{ \max J, J \in \overline I \} \f$.
      */
    int getLast() const
    {
        return last;
    }

    /** \brief  Returns the number of values spent between two ticks.
      *
      * If set to \c 0, no ticks will be drawn at all.
      * Behavior is undefined for negative values.
      */
    int getTickInterval() const
    {
        return tickInterval;
    }

    /** \brief  Returns the number of values spent between two labeled ticks.
      *
      * If set to \c 0, no labeled ticks will be drawn at all.
      * Behavior is undefined for negative values.
      */
    int getCaptionInterval() const
    {
        return captionInterval;
    }

    /** \brief  Returns \f$ \left| I \right| \f$.
      */
    unsigned int getSpanCount() const
    {
        return spans.size();
    }

    /** \brief  References a certain element within \f$ I \f$.
      */
    const IntSpan& getSpan( unsigned int index ) const
    {
        return spans[ index ];
    }

    /** \brief  Tells whether an invocation to \ref updateSpan would satisfy all pre-conditions.
      *
      * These are in particular:
      *
      * - <code>index <= 0 || getSpan( index - 1 ).last < span.first</code>
      * - <code>index + 1 >= getSpanCount() || getSpan( index + 1 ).first > span.last</code>
      * - <code>span.first < span.last</code>
      * - <code>span.first >= getFirst() && span.last <= getLast()</code>
      */
    bool canTakeUpdatedSpan( unsigned int index, const IntSpan& span ) const;

    /** \brief  Manipulates a certain element within \f$ I \f$.
      *
      * \pre    <code>index <= 0 || getSpan( index - 1 ).last < span.first</code>
      * \pre    <code>index + 1 >= getSpanCount() || getSpan( index + 1 ).first > span.last</code>
      * \pre    <code>span.first < span.last</code>
      * \pre    <code>span.first >= getFirst() && span.last <= getLast()</code>
      *
      * \see    \ref canTakeUpdatedSpan
      */
    void updateSpan( unsigned int index, const IntSpan& span );


    /** \brief  Sets a new span visualization definition.
      */
    void setSpanPainter( IntSpanPainter* );


    /** \brief  References the supplied tracker prototype.
      */
    MultiSpanSliderTracker& getTracker()
    {
        return *tracker;
    }

    /** \brief  References the supplied tracker prototype.
      */
    const MultiSpanSliderTracker& getTracker() const
    {
        return *tracker;
    }


    /** \brief  References the supplied span visualization definition.
      */
    IntSpanPainter& getSpanPainter()
    {
        return *spanPainter;
    }

    /** \brief  References the supplied span visualization definition.
      */
    const IntSpanPainter& getSpanPainter() const
    {
        return *spanPainter;
    }


public slots:

    /** \brief  Sets \f$ \min \{ \min J, J \in \overline I \} \f$.
      *
      * \pre    no existing span becomes out of bounds by the requested boundary alternation
      */
    void setFirst( int );

    /** \brief  Sets \f$ \max \{ \max J, J \in \overline I \} \f$.
      *
      * \pre    no existing span becomes out of bounds by the requested boundary alternation
      */
    void setLast( int );

    /** \brief  Sets the number of values spent between two ticks.
      *
      * If set to \c 0, no ticks will be drawn at all.
      * Behavior is undefined for negative values.
      */
    void setTickInterval( int );

    /** \brief  Sets the number of values spent between two labeled ticks.
      *
      * If set to \c 0, no labeled ticks will be drawn at all.
      * Behavior is undefined for negative values.
      */
    void setCaptionInterval( int );

    /** \brief  Adds given span to \f$ I \f$.
      *
      * \pre    <code>span.first < span.last</code>
      *
      * \throws std::runtime_error  thrown when the new span would either overlap with
      *                             another span or exceed the previously configured
      *                             limits.
      */
    unsigned int addSpan( const Carna::base::IntSpan& span );

    /** \brief  Makes \f$ I = \emptyset \f$.
      */
    void removeAllSpans();


protected:

    /** \brief  Paints the widget.
      */
    virtual void paintEvent( QPaintEvent* );

    /** \brief  Performs tracker interaction.
      */
    virtual void mousePressEvent( QMouseEvent* ev );

    /** \brief  Performs tracker interaction.
      */
    virtual void mouseMoveEvent( QMouseEvent* ev );

    /** \brief  Performs tracker interaction.
      */
    virtual void mouseReleaseEvent( QMouseEvent* ev );


    /** \brief  Does nothing.
      *
      * Override this in order to implement some custom tracker interaction behavior.
      */
    virtual void processTrackerMousePress( IntSpan&, int index, MultiSpanSliderTracker::Role )
    {
    }

    /** \brief  Does nothing.
      */
    virtual void processNewSpan( IntSpan&, int index )
    {
    }

    /** \brief  Does nothing.
      */
    virtual void processRemovedSpan( IntSpan&, int index )
    {
    }

    /** \brief  Does nothing.
      */
    virtual void processModifiedSpan( IntSpan&, int index )
    {
    }


private:

    /** \brief  Holds whether the trackers are moving horizontally or vertically.
      */
    Qt::Orientation orientation;

    /** \brief  Holds a tracker prototype.
      */
    std::unique_ptr< MultiSpanSliderTracker > tracker;

    /** \brief  Defines the visualization of spans.
      */
    std::unique_ptr< IntSpanPainter > spanPainter;


    /** \brief  Holds \f$ \min \{ \min J, J \in \overline I \} \f$.
      */
    int first;

    /** \brief  Holds \f$ \max \{ \max J, J \in \overline I \} \f$.
      */
    int last;

    /** \brief  Holds the number of values spent between two ticks.
      */
    unsigned int tickInterval;

    /** \brief  Holds the number of values spent between two labeled ticks.
      */
    unsigned int captionInterval;


    /** \brief  Type definition of \f$ I \f$
      */
    typedef std::vector< IntSpan > SpanSet;

    /** \brief  Instantiation of \f$ I \f$
      */
    SpanSet spans;


    /** \brief  Maps numerical values from model space to pixels in view space and
      *         vise versa.
      */
    MultiSpanSliderModelViewMapping mvm;


    /** \brief  Holds the index of the span, the user is currently interacting with,
      *         or \c -1, when no interaction is happening.
      */
    int grabbedIndex;

    /** \brief  When \c grabbedIndex is not \c -1, \c grabbedMode determines which
      *         ending of the span referenced by \c grabbedIndex is being modified.
      */
    MultiSpanSliderTracker::Role grabbedMode;

    /** \brief  Holds the pixel offset between the currently grabbed tracker's
      *         real position and the result of mapping current mouse position to
      *         model space.
      */
    int grabOffset;

    /** \brief  Holds the previously captured significant mouse coordinate.
      */
    int prevMouseY;

    /** \brief  Holds whether a new span will be spawn next time the mouse is moved
      *         with primary button pressed down while no span grabbed.
      */
    bool spawnNewSpan;


signals:

    /** \brief  Emitted when changes have been made to the model.
      */
    void spansChanged( Carna::base::qt::MultiSpanSlider& );

    /** \brief  Emitted when changes have been made to the model.
      */
    void spansChanged();

}; // MultiSpanSlider



}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna

#endif // MULTISPANSLIDER_H_6014714286
