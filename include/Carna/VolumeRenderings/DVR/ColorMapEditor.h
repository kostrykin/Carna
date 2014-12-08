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

#ifndef DVR_COLORMAPEDITOR_H_6014714286
#define DVR_COLORMAPEDITOR_H_6014714286

/** \file   ColorMapEditor.h
  * \brief  Defines \ref Carna::VolumeRenderings::DVR::ColorMapEditor.
  */

#include <Carna/base/qt/MultiSpanSlider.h>
#include <Carna/base/qt/RotatingColor.h>
#include <deque>

namespace Carna
{

namespace VolumeRenderings
{

namespace DVR
{



// ----------------------------------------------------------------------------------
// VolumeRenderings :: DVR :: ColorMapEditor
// ----------------------------------------------------------------------------------

/** \brief  User interface for the configuration of DVR's color map
  *
  * \author Leonid Kostrykin
  * \date   16.8.11 - 29.8.11
  */
class CARNA_EXPORT ColorMapEditor : public base::qt::MultiSpanSlider
{

    Q_OBJECT

public:

    /** \brief  Holds width of box drawn around ColorMapTrackerEditor instances. 
      */
    static const int trackerEditorBoxWidth;

    /** \brief  Holds height of box drawn around ColorMapTrackerEditor instances. 
      */
    static const int trackerEditorBoxHeight;


    /** \brief  Instantiates.
      */
    explicit ColorMapEditor( QWidget* parent = 0 );

    /** \brief  Does nothing.
      */
    virtual ~ColorMapEditor();

    
    /** \brief  References the currently selected span or \c nullptr if no span is selected.
      */
    base::IntSpan* getSelectedSpan()
    {
        return selectedSpan;
    }

    /** \brief  References the currently selected span or \c nullptr if no span is selected.
      */
    const base::IntSpan* getSelectedSpan() const
    {
        return selectedSpan;
    }


    /** \brief  References the color assigned to the beginning tracker of given span.
      */
    QColor& getFirstColor( int span_index )
    {
        return colors[ 2 * span_index ];
    }

    /** \brief  References the color assigned to the ending tracker of given span.
      */
    QColor& getLastColor( int span_index )
    {
        return colors[ 2 * span_index + 1 ];
    }

    /** \brief  References the color assigned to the beginning tracker of given span.
      */
    const QColor& getFirstColor( int span_index ) const
    {
        return const_cast< ColorMapEditor* >( this )->getFirstColor( span_index );
    }

    /** \brief  References the color assigned to the ending tracker of given span.
      */
    const QColor& getLastColor( int span_index ) const
    {
        return const_cast< ColorMapEditor* >( this )->getLastColor( span_index );
    }


    /** \brief Emits signals indicating this widget's state has changed.
      *
      * Also posts a repaint event.
      */
    void pretendDataChanged()
    {
        emit spansChanged( *this );
        emit spansChanged();
        this->update();
    }


protected:

    virtual void paintEvent( QPaintEvent* );

    /** \brief  Updates the layouting of the ColorMapTrackerEditor instances.
      *
      * This is done by the invocation of ColorMapEditor::layoutSpanConfigWidgets.
      */
    virtual void resizeEvent( QResizeEvent* );

    /** \brief  Updates span selection.
      */
    virtual void processTrackerMousePress( base::IntSpan&, int index, base::qt::MultiSpanSliderTracker::Role );

    /** \brief  Updates color map.
      */
    virtual void processNewSpan( base::IntSpan&, int index );

    /** \brief  Updates color map and span selection.
      */
    virtual void processRemovedSpan( base::IntSpan&, int index );

    /** \brief  Updates ColorMapTrackerEditor instances for currently selected span.
      */
    virtual void processModifiedSpan( base::IntSpan&, int index );


private:    // TODO: move to PIMPL

    /** \brief  References the currently selected span.
      */
    base::IntSpan* selectedSpan;

    /** \brief  Holds the color map.
      */
    std::deque< QColor > colors;

    /** \brief  Holds the color which will be assigned to the span created next.
      */
    base::qt::RotatingColor nextColor;

    /** \brief  References the editor for the currently selected beginning tracker
      *         or is \c 0 when no span is selected.
      */
    ColorMapTrackerEditor* beginConfigWidget;

    /** \brief  References the editor for the currently selected ending tracker
      *         or is \c 0 when no span is selected.
      */
    ColorMapTrackerEditor* endConfigWidget;


private slots:

    /** \brief  Updates the layouting of the ColorMapTrackerEditor instances.
      */
    void layoutSpanConfigWidgets();

}; // VolumeRenderings :: DVR :: ColorMapEditor



}  // namespace Carna :: VolumeRenderings :: DVR

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // DVR_COLORMAPEDITOR_H_6014714286
