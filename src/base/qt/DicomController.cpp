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

#include <Carna/base/qt/DicomController.h>
#include <Carna/base/qt/SeriesView.h>
#include <Carna/base/qt/CarnaProgressDialog.h>
#include <Carna/base/model/DicomManager.h>
#include <Carna/base/model/DicomExtractionSettings.h>
#include <Carna/base/model/Series.h>
#include <Carna/base/model/SeriesLoadingRequest.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QThread>
#include <QMessageBox>
#include <climits>
#include <sstream>

namespace Carna
{

namespace base
{

namespace qt
{



// ----------------------------------------------------------------------------------
// DicomController
// ----------------------------------------------------------------------------------

DicomController::DicomController()
    : seriesView( new SeriesView() )
    , laSpacingZ( new QLabel( "Z-Spacing:" ) )
    , sbSpacingZ( new QDoubleSpinBox() )
    , buSaveIndex( new QPushButton( "Save Index..." ) )
    , buExtract( new QPushButton( "Extract Series..." ) )
    , buLoad( new QPushButton( "Load Series" ) )
    , managerThread( new QThread() )
    , manager( new model::DicomManager() )
{
    qRegisterMetaType< Carna::base::model::DicomExtractionSettings >( "Carna::base::model::DicomExtractionSettings" );
    qRegisterMetaType< Carna::base::model::SeriesLoadingRequest >( "Carna::base::model::SeriesLoadingRequest" );

    manager->moveToThread( managerThread );
    managerThread->start();
    connect( this, SIGNAL( openDirectory( const QString& ) ), manager, SLOT( openDirectory( const QString& ) ) );
    connect( this, SIGNAL( openIndex( const QString& ) ), manager, SLOT( openIndex( const QString& ) ) );
    connect( this, SIGNAL( saveIndex( const QString& ) ), manager, SLOT( saveIndex( const QString& ) ) );
    connect( this, SIGNAL( extractSeries( const Carna::base::model::DicomExtractionSettings& ) ),
        manager, SLOT( extractSeries( const Carna::base::model::DicomExtractionSettings& ) ) );

    connect( managerThread, SIGNAL( finished() ), managerThread, SLOT( deleteLater() ) );
    connect( managerThread, SIGNAL( finished() ), manager, SLOT( deleteLater() ) );

 // ----------------------------------------------------------------------------------

    QVBoxLayout* const master = new QVBoxLayout();
    QHBoxLayout* const controls = new QHBoxLayout();

 // ----------------------------------------------------------------------------------

    QPushButton* const buOpenDirectory = new QPushButton( "Scan Directory..." );
    QPushButton* const buOpenIndex = new QPushButton( "Open Index..." );

    sbSpacingZ->setRange( 0.01, std::numeric_limits< double >::max() );
    sbSpacingZ->setSingleStep( 0.1 );
    sbSpacingZ->setDecimals( 3 );
    sbSpacingZ->setSuffix( " mm" );

    laSpacingZ->setBuddy( sbSpacingZ );
    laSpacingZ->setAlignment( Qt::AlignRight | Qt::AlignVCenter );

    controls->addWidget( buOpenDirectory );
    controls->addWidget( buOpenIndex );
    controls->addWidget( buSaveIndex );
    controls->addWidget( buExtract );
    controls->addWidget( laSpacingZ );
    controls->addWidget( sbSpacingZ );
    controls->addWidget( buLoad );

    master->addLayout( controls );
    master->addWidget( seriesView );

    connect( buOpenDirectory, SIGNAL( clicked() ), this, SLOT( openDirectory() ) );
    connect( buOpenIndex, SIGNAL( clicked() ), this, SLOT( openIndex() ) );
    connect( buSaveIndex, SIGNAL( clicked() ), this, SLOT( saveIndex() ) );
    connect( seriesView, SIGNAL( selectionChanged() ), this, SLOT( selectionChanged() ) );
    connect( seriesView, SIGNAL( seriesSelected( const Carna::base::model::Series& ) ), this, SLOT( seriesSelected( const Carna::base::model::Series& ) ) );
    connect( seriesView, SIGNAL( seriesDoubleClicked( const Carna::base::model::Series& ) ), this, SLOT( seriesDoubleClicked( const Carna::base::model::Series& ) ) );
    connect( buExtract, SIGNAL( clicked() ), this, SLOT( extractSeries() ) );
    connect( buLoad, SIGNAL( clicked() ), this, SLOT( loadSeries() ) );

    buSaveIndex->setEnabled( false );
    buExtract->setEnabled( false );
    selectionChanged();

 // ----------------------------------------------------------------------------------

    this->setLayout( master );
}


DicomController::~DicomController()
{
    managerThread->quit();
}


void DicomController::openDirectory()
{
    closeSeries();

 // ----------------------------------------------------------------------------------

    const QString dirName
        = QFileDialog::getExistingDirectory( this
                                           , "Scan Directory"
                                           , ""
                                           , QFileDialog::ShowDirsOnly
                                           | QFileDialog::DontResolveSymlinks
                                           | QFileDialog::HideNameFilterDetails );

    if( dirName.isEmpty() )
    {
        return;
    }

    CarnaProgressDialog loadingController( dirName, "Cancel", 0, 0, this );
    loadingController.setWindowTitle( "Scan Directory" );
    loadingController.setWindowModality( Qt::WindowModal );
    loadingController.setModal( true );

    connect( &loadingController, SIGNAL( canceled() ), manager, SLOT( cancel() ) );
    connect( &loadingController, SIGNAL( canceled() ), this, SLOT( seriesLoadingAborted() ) );
    connect( manager, SIGNAL( finished() ), this, SLOT( seriesLoaded() ) );
    connect( manager, SIGNAL( finished() ), &loadingController, SLOT( accept() ) );
    connect( manager, SIGNAL( failed( const QString& ) ), this, SLOT( seriesLoadingFailed( const QString& ) ) );
    connect( manager, SIGNAL( totalFilesCountChanged( unsigned int ) ), &loadingController, SLOT( setMaximum( unsigned int ) ) );
    connect( manager, SIGNAL( processedFilesCountChanged( unsigned int ) ), &loadingController, SLOT( setValue( unsigned int ) ) );
    connect( manager, SIGNAL( failed() ), &loadingController, SLOT( reject() ) );

    emit openDirectory( dirName );

    loadingController.exec();

    disconnect( manager, SIGNAL( finished() ), this, SLOT( seriesLoaded() ) );
    disconnect( manager, SIGNAL( failed( const QString& ) ), this, SLOT( seriesLoadingFailed( const QString& ) ) );

    if( !manager->getFailedFiles().empty() )
    {
        std::stringstream failedFiles;
        for( auto it = manager->getFailedFiles().begin(); it != manager->getFailedFiles().end(); ++it )
        {
            failedFiles << ( *it ) << std::endl;
        }

        QMessageBox msgBox;
        msgBox.setIcon( QMessageBox::Warning );
        msgBox.setText( QString::number( manager->getFailedFiles().size() ) + " file(s) could not be processed. Refer to the details section for a full list." );
        msgBox.setDetailedText( QString::fromStdString( failedFiles.str() ) );
        msgBox.setStandardButtons( QMessageBox::Ok );
        msgBox.setDefaultButton( QMessageBox::Ok );
        msgBox.setEscapeButton( QMessageBox::Ok );
        msgBox.exec();
    }
}


void DicomController::openIndex()
{
    closeSeries();

 // ----------------------------------------------------------------------------------

    const QString fileName
        = QFileDialog::getOpenFileName( this
                                      , "Open Index"
                                      , ""
                                      , "Index Files (*.idx);;XML Files (*.xml);;All files (*.*)"
                                      , 0
                                      , QFileDialog::ReadOnly
                                      | QFileDialog::HideNameFilterDetails );

    if( fileName.isEmpty() )
    {
        return;
    }

    QPushButton* const cancelButton = new QPushButton( "Cancel" );
    cancelButton->setEnabled( false );

    CarnaProgressDialog loadingController( fileName, "Cancel", 0, 0, this );
    loadingController.setCancelButton( cancelButton );
    loadingController.setWindowTitle( "Open Index" );
    loadingController.setWindowModality( Qt::WindowModal );
    loadingController.setModal( true );

    connect( manager, SIGNAL( finished() ), this, SLOT( seriesLoaded() ) );
    connect( manager, SIGNAL( finished() ), &loadingController, SLOT( accept() ) );
    connect( manager, SIGNAL( failed( const QString& ) ), this, SLOT( seriesLoadingFailed( const QString& ) ) );
    connect( manager, SIGNAL( failed() ), &loadingController, SLOT( reject() ) );

    emit openIndex( fileName );

    loadingController.exec();

    disconnect( manager, SIGNAL( finished() ), this, SLOT( seriesLoaded() ) );
    disconnect( manager, SIGNAL( failed( const QString& ) ), this, SLOT( seriesLoadingFailed( const QString& ) ) );
}


void DicomController::saveIndex()
{
    const QString fileName
        = QFileDialog::getSaveFileName( this
                                      , "Save Index"
                                      , ""
                                      , "Index Files (*.idx);;XML Files (*.xml);;All files (*.*)"
                                      , 0
                                      , QFileDialog::DontResolveSymlinks
                                      | QFileDialog::HideNameFilterDetails );

    if( fileName.isEmpty() )
    {
        return;
    }

    QPushButton* const cancelButton = new QPushButton( "Cancel" );
    cancelButton->setEnabled( false );

    CarnaProgressDialog savingController( fileName, "Cancel", 0, 0, this );
    savingController.setCancelButton( cancelButton );
    savingController.setWindowTitle( "Save Index" );
    savingController.setWindowModality( Qt::WindowModal );
    savingController.setModal( true );

    connect( manager, SIGNAL( finished() ), &savingController, SLOT( accept() ) );
    connect( manager, SIGNAL( failed() ), &savingController, SLOT( reject() ) );
    connect( manager, SIGNAL( failed( const QString& ) ), this, SLOT( indexSavingFailed( const QString& ) ) );

    emit saveIndex( fileName );

    savingController.exec();

    disconnect( manager, SIGNAL( failed( const QString& ) ), this, SLOT( indexSavingFailed( const QString& ) ) );
}


void DicomController::closeSeries()
{
    seriesView->clear();

    buSaveIndex->setEnabled( false );
}


void DicomController::seriesLoaded()
{
    seriesView->clear();

    for( auto patient_it = manager->getPatients().begin(); patient_it != manager->getPatients().end(); ++patient_it )
    {
        const model::Patient& patient = *( patient_it->second );
        seriesView->addPatient( patient );
    }

    buSaveIndex->setEnabled( true );
}


void DicomController::seriesLoadingFailed( const QString& message )
{
    QMessageBox::critical( this, "Loading DICOM Series", "DICOM Series loading failed: " + message );
}


void DicomController::indexSavingFailed( const QString& message )
{
    QMessageBox::critical( this, "Writing Series Index", "Saving DICOM Series index failed: " + message );
}


void DicomController::extractionFailed( const QString& message )
{
    QMessageBox::critical( this, "Writing Series Index", "Saving DICOM Series index failed: " + message );
}


void DicomController::seriesLoadingAborted()
{
}


void DicomController::selectionChanged()
{
    const unsigned int selected_series_count = seriesView->getSelectedSeries().size();

    laSpacingZ->setEnabled( selected_series_count == 1 );
    sbSpacingZ->setEnabled( selected_series_count == 1 );
    buLoad->setEnabled( selected_series_count == 1 );

    buExtract->setEnabled( selected_series_count > 0 );
}


void DicomController::seriesSelected( const model::Series& series )
{
    sbSpacingZ->setValue( series.getSpacingZ() );
}


void DicomController::extractSeries()
{
    const auto& selected_series = seriesView->getSelectedSeries();
    if( selected_series.empty() )
    {
        return;
    }

    QString dirName = QFileDialog::getExistingDirectory( this
                                                       , "Extract Series"
                                                       , ""
                                                       , QFileDialog::ShowDirsOnly
                                                       | QFileDialog::DontResolveSymlinks );

    if( dirName.isEmpty() )
    {
        return;
    }

    QPushButton* const cancelButton = new QPushButton( "Cancel" );
    cancelButton->setEnabled( false );

    CarnaProgressDialog extractingController( dirName, "Cancel", 0, 0, this );
    extractingController.setCancelButton( cancelButton );
    extractingController.setWindowTitle( "Extract Series" );
    extractingController.setWindowModality( Qt::WindowModal );
    extractingController.setModal( true );

    connect( manager, SIGNAL( finished() ), &extractingController, SLOT( accept() ) );
    connect( manager, SIGNAL( failed() ), &extractingController, SLOT( reject() ) );
    connect( manager, SIGNAL( failed( const QString& ) ), this, SLOT( extractionFailed( const QString& ) ) );
    connect( manager, SIGNAL( totalFilesCountChanged( unsigned int ) ), &extractingController, SLOT( setMaximum( unsigned int ) ) );
    connect( manager, SIGNAL( processedFilesCountChanged( unsigned int ) ), &extractingController, SLOT( setValue( unsigned int ) ) );

    model::DicomExtractionSettings settings( dirName, selected_series );

    emit extractSeries( settings );

    extractingController.exec();

    disconnect( manager, SIGNAL( failed( const QString& ) ), this, SLOT( extractionFailed( const QString& ) ) );
}


void DicomController::loadSeries()
{
    const auto& selected_series = seriesView->getSelectedSeries();
    if( selected_series.size() != 1 )
    {
        return;
    }

    const double zSpacing = sbSpacingZ->value();

    emit seriesLoadingRequested( model::SeriesLoadingRequest( **selected_series.begin(), zSpacing ) );
}


void DicomController::seriesDoubleClicked( const model::Series& series )
{
    emit seriesLoadingRequested( model::SeriesLoadingRequest( series, series.getSpacingZ() ) );
}



}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna
