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

#ifndef SERIESVIEW_H_6014714286
#define SERIESVIEW_H_6014714286

/** \file   SeriesView.h
  * \brief  Defines \ref Carna::base::qt::SeriesView.
  */

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>
#include <QFrame>
#include <deque>
#include <set>

class QVBoxLayout;

namespace Carna
{

namespace base
{

namespace model
{
    class Patient;
    class Series;
}

namespace qt
{

class ToggleSeriesPreview;



// ----------------------------------------------------------------------------------
// SeriesView
// ----------------------------------------------------------------------------------

class SeriesView : public QFrame
{

    NON_COPYABLE

    Q_OBJECT

public:

    /** \brief  Instantiates.
      */
    explicit SeriesView( QWidget* parent = nullptr );

    /** \brief  Releases acquired resources.
      */
    virtual ~SeriesView();


    const std::set< const model::Series* >& getSelectedSeries() const;


public slots:

    void addPatient( const Carna::base::model::Patient& );

    void clear();


signals:

    void selectionChanged();

    void seriesSelected( const Carna::base::model::Series& );

    void seriesUnselected( const Carna::base::model::Series& );

    void seriesDoubleClicked( const Carna::base::model::Series& );


private:

    QVBoxLayout* container;

    QWidget* const containerWidget;

    bool rebuildRequired;

    std::deque< const model::Patient* > patients;

    std::set< const model::Series* > selectedSeries;

    std::set< ToggleSeriesPreview* > selectedSeriesPreviews;


private slots:

    void rebuild();

    void scheduleRebuild();

    void seriesToggled( Carna::base::qt::ToggleSeriesPreview& );

    void seriesDoubleClicked( Carna::base::qt::ToggleSeriesPreview& );

}; // SeriesView



}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna

#endif // SERIESVIEW_H_6014714286
