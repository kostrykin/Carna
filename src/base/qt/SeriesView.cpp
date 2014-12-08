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

#include <Carna/base/model/Patient.h>
#include <Carna/base/model/Study.h>
#include <Carna/base/model/Series.h>
#include <Carna/base/qt/SeriesView.h>
#include <Carna/base/qt/ToggleSeriesPreview.h>
#include <Carna/base/qt/FlowLayout.h>
#include <Carna/base/qt/ExpandableGroupBox.h>
#include <QTimer>
#include <QLabel>
#include <QVBoxLayout>
#include <QScrollArea>

namespace Carna
{

namespace base
{

namespace qt
{



// ----------------------------------------------------------------------------------
// SeriesView
// ----------------------------------------------------------------------------------

SeriesView::SeriesView( QWidget* parent )
    : QFrame( parent )
    , rebuildRequired( false )
    , container( new QVBoxLayout() )
    , containerWidget( new QWidget() )
{
    this->setLayout( new QVBoxLayout() );
    this->layout()->setContentsMargins( 0, 0, 0, 0 );
    this->setMinimumWidth( 540 );
    this->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Expanding );

    const static QString BACKGROUND_COLOR = "#808080";

    containerWidget->setLayout( container );
    containerWidget->setStyleSheet( "QWidget{ background-color: " + BACKGROUND_COLOR + "; } QLabel{ color: #FFFFFF; }" );
    
    QScrollArea* const scrollable_container = new QScrollArea();
    scrollable_container->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    scrollable_container->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
    scrollable_container->setWidgetResizable( true );
    scrollable_container->setWidget( containerWidget );
    scrollable_container->setFrameStyle( QFrame::Panel | QFrame::Sunken );
    scrollable_container->setLineWidth( 1 );
    scrollable_container->setStyleSheet( "QScrollArea{ background-color: " + BACKGROUND_COLOR + "; }" );

    this->layout()->addWidget( scrollable_container );
}


SeriesView::~SeriesView()
{
}


void SeriesView::addPatient( const model::Patient& patient )
{
    patients.push_back( &patient );
    scheduleRebuild();
}


void SeriesView::clear()
{
    selectedSeriesPreviews.clear();

 // ----------------------------------------------------------------------------------

    std::set< const model::Series* > selected_series = selectedSeries;
    selectedSeries.clear();

    for( auto series_itr = selected_series.begin(); series_itr != selected_series.end(); ++series_itr )
    {
        emit seriesUnselected( **series_itr );
    }

    emit selectionChanged();

    patients.clear();
    scheduleRebuild();
}


void SeriesView::scheduleRebuild()
{
    if( rebuildRequired == false )
    {
        rebuildRequired = true;
        QTimer::singleShot( 0, this, SLOT( rebuild() ) );
    }
}


void SeriesView::rebuild()
{
    if( !rebuildRequired )
    {
        return;
    }

    QSizePolicy headingSizePolicy( QSizePolicy::Ignored, QSizePolicy::Fixed );
    headingSizePolicy.setHeightForWidth( false );

 // ----------------------------------------------------------------------------------

    delete this->container;
    this->container = new QVBoxLayout();
    qDeleteAll( containerWidget->children() );
    
    for( auto patient_itr = patients.begin(); patient_itr != patients.end(); ++patient_itr )
    {
        const model::Patient& patient = **patient_itr;

     // create and add patient name

        QLabel* const heading = new QLabel( QString::fromStdString( patient.name ) );
        heading->setStyleSheet( "font-size: 14pt; font-weight: bold;" );
        heading->setSizePolicy( headingSizePolicy );
        container->addWidget( heading );

     // process studies

        for( auto study_itr = patient.getStudies().begin(); study_itr != patient.getStudies().end(); ++study_itr )
        {
            const model::Study& study = **study_itr;

         // create expendable group box for the series

            ExpandableGroupBox* const gbStudy = new ExpandableGroupBox( QString::fromStdString( study.name ), true );
            FlowLayout* const series = new FlowLayout();
            gbStudy->child()->setLayout( series );
            series->setContentsMargins( 0, 0, 0, 0 );
            container->addWidget( gbStudy );

         // process series

            for( auto series_itr = study.getSeries().begin(); series_itr != study.getSeries().end(); ++series_itr )
            {
                const model::Series& current_series = **series_itr;

                if( current_series.getElements().size() < model::Series::MIN_ELEMENTS_COUNT )
                {
                    continue;
                }

                ToggleSeriesPreview* const preview = new ToggleSeriesPreview();
                preview->setSeries( current_series );
                series->addWidget( preview );

                connect( preview, SIGNAL( toggled( Carna::base::qt::ToggleSeriesPreview& ) ), this, SLOT( seriesToggled( Carna::base::qt::ToggleSeriesPreview& ) ) );
                connect( preview, SIGNAL( doubleClicked( Carna::base::qt::ToggleSeriesPreview& ) ), this, SLOT( seriesDoubleClicked( Carna::base::qt::ToggleSeriesPreview& ) ) );
            }
        }
    }

    container->addStretch( 1 );

    containerWidget->setLayout( this->container );

 // ----------------------------------------------------------------------------------

    rebuildRequired = false;
}


void SeriesView::seriesToggled( ToggleSeriesPreview& seriesPreview )
{
    if( !seriesPreview.hasSeries() )
    {
        return;
    }

    const model::Series& series = seriesPreview.getSeries();
    if( seriesPreview.isToggled() )
    {
        selectedSeries.insert( &series );
        selectedSeriesPreviews.insert( &seriesPreview );
        emit seriesSelected( series );
    }
    else
    {
        selectedSeries.erase( &series );
        selectedSeriesPreviews.erase( &seriesPreview );
        emit seriesUnselected( series );
    }

    emit selectionChanged();
}


const std::set< const model::Series* >& SeriesView::getSelectedSeries() const
{
    return selectedSeries;
}


void SeriesView::seriesDoubleClicked( ToggleSeriesPreview& seriesPreview )
{
    const auto selectedSeriesPreviews = this->selectedSeriesPreviews;

    for( auto preview_itr = selectedSeriesPreviews.begin(); preview_itr != selectedSeriesPreviews.end(); ++preview_itr )
    {
        ToggleSeriesPreview* const preview = *preview_itr;
        if( preview != &seriesPreview )
        {
            preview->setToggled( false );
        }
    }

    seriesPreview.setToggled( true );

    emit seriesDoubleClicked( seriesPreview.getSeries() );
}



}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna
