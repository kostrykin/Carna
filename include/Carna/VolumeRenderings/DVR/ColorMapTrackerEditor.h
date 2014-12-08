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

#ifndef DVR_COLORMAPTRACKEREDITOR_H_6014714286
#define DVR_COLORMAPTRACKEREDITOR_H_6014714286

/** \file   ColorMapTrackerEditor.h
  * \brief  Defines \ref Carna::VolumeRenderings::DVR::ColorMapTrackerEditor.
  */

#include <Carna/Carna.h>
#include <QWidget>

class QSpinBox;
class QColor;

namespace Carna
{

namespace VolumeRenderings
{

namespace DVR
{



// ----------------------------------------------------------------------------------
// VolumeRenderings :: DVR :: ColorMapTrackerEditor
// ----------------------------------------------------------------------------------

/** \brief  User interface for ColorMapTracker configuration
  *
  * \author Leonid Kostrykin
  * \date   16.8.11 - 29.8.11
  */
class CARNA_EXPORT ColorMapTrackerEditor : public QWidget
{

    Q_OBJECT

public:

    /** \brief  Instantiates.
      */
    ColorMapTrackerEditor( ColorMapEditor& widget
                         , unsigned int color_index
                         , int current_huv
                         , QWidget* parent = 0 );


private:

    /** \brief  References the widget which the edited trackers belong to.
      */
    ColorMapEditor& widget;

    /** \brief  Identifies the edited tracker by the index of it's assigned color.
      */
    const unsigned int colorIndex;

    /** \brief  Holds whether the current settings made by user are valid.
      */
    bool validState;

    /** \brief  References HUV editing UI element.
      */
    QSpinBox* const sbHuv;


public slots:

    /** \brief  Sets current HUV setting for the editor.
      *
      * The associated tracker will be updated immediately if the HUV setting is valid.
      * In this case, also ColorMapTrackerEditor::validState will be set to \c true.
      * Otherwise it will be set to \c false.
      */
    void setCurrentHuv( int huv );


protected:

    /** \brief  Paints the widget.
      *
      * If ColorMapTrackerEditor::validState is set to \c false, a red background will
      * be painted.
      */
    virtual void paintEvent( QPaintEvent* );


private slots:

    /** \brief  Updates internal state.
      */
    void updateColor( const QColor& );

    /** \brief  Updates internal state.
      */
    void updateHuv( int new_huv );

}; // VolumeRenderings :: DVR :: ColorMapTrackerEditor



}  // namespace Carna :: VolumeRenderings :: DVR

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // DVR_COLORMAPTRACKEREDITOR_H_6014714286
