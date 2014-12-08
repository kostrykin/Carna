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

#ifndef QTDICOMSERIESOPENINGCONTROLLER_H_6014714286
#define QTDICOMSERIESOPENINGCONTROLLER_H_6014714286

/** \file   QtDicomSeriesOpeningController.h
  * \brief  Defines \ref Carna::base::qt::QtDicomSeriesOpeningController.
  */

#include <QObject>
#include <dicom-interface/dicomSeries.h>
#include <deque>

namespace Carna
{

namespace base
{

namespace qt
{



// ----------------------------------------------------------------------------------
// ImageData
// ----------------------------------------------------------------------------------

struct ImageData
{

    ImageData()
    {
    }

    ImageData( const std::string& patientID
             , const std::string& studyID
             , const std::string& seriesID
             , const std::string& fileName
             , double zPosition )
        : patientID( patientID )
        , studyID( studyID )
        , seriesID( seriesID )
        , fileName( fileName )
        , zPosition( zPosition )
    {
    }


    const std::string patientID;
    const std::string studyID;
    const std::string seriesID;
    const std::string fileName;
    double zPosition;

}; // ImageData



// ----------------------------------------------------------------------------------
// QtDicomSeriesOpeningController
// ----------------------------------------------------------------------------------

/** \brief  Qt based implementation of DicomSeriesOpeningControler
  *
  * Maps method-calling based messaging to Qt's signals- & slots-principle
  *
  * \author Leonid Kostrykin
  * \date   2010 - 2013
  */
class QtDicomSeriesOpeningController : public QObject, public DicomSeriesOpeningController
{

    Q_OBJECT

public:

    /** \brief  Instantiates.
      */
    QtDicomSeriesOpeningController()
        : m_bInvalidState( false )
        , m_bCanceled( false )
        , m_bFailed( false )
    {
    }

    
    /** \brief  Emits onPatientFound.
      */
    virtual void processNewImage( const std::string& patientID
                                , const std::string& studyID
                                , const std::string& seriesID
                                , const std::string& fileName
                                , double zPosition ) override
    {
        emit onNewImage( ImageData( patientID, studyID, seriesID, fileName, zPosition ) );
    }

    /** \brief  Emits onDirOpened.
      *
      * Indicates that \a n files are going to be read.
      */
    virtual void dirOpened( unsigned int n ) override
    {
        failedFiles.clear();
        this->n = n;
        emit onDirOpened( n );
    }

    /** \brief  Emits onDirIterationProgress.
      *
      * Indicates that the \a n.th file is being read.
      */
    virtual void dirIterationProgress( unsigned int n ) override
    {
        this->i = n;
        emit onDirIterationProgress( n );
    }

    /** \brief  Indicates that reading a certain file failed.
      *
      * Prompts the user whether she or he wants to proceed.
      */
    virtual bool failedReadFile( const std::string& file, const std::vector< std::string >& succeeded ) override;

    /** \brief  Displays a hint to the user.
      */
    virtual void hint( const std::string& ) override;


    /** \brief  Returns whether the opening procedure is in a valid state.
      */
    bool isInvalid() const
    {
        return m_bInvalidState;
    }

    
    const std::deque< std::string >& getFailedFiles() const
    {
        return failedFiles;
    }


public slots:

    /** \brief  Asks the user for confirmation and possibly aborts the opening procedure.
      */
    void cancel();


signals:

    /** \brief  Emitted when a new image is processed.
      */
    void onNewImage( const Carna::base::qt::ImageData& );

    /** \brief  Emitted right before scanning each image's meta data begins.
      */
    void onDirOpened( unsigned int nFiles );

    /** \brief  Continuously emitted while scanning images' meta data.
      */
    void onDirIterationProgress( unsigned int nFile );

    /** \brief  Emitted right after an ultimate abort by the user.
      */
    void onCanceled();

    void onFailed( const QString& );


private:

    /** \brief  Holds whether the opening procedure is in a valid state.
      */
    bool m_bInvalidState;
    bool m_bCanceled;   ///< \brief QProgressDialog emits 'canceled' multiple times - once
                        ///         when the 'cancel' button is clicked and once more when
                        ///         the dialog closes. We need to track whether dialog was
                        ///         already canceled so we won't prompt the user more than
                        ///         once.
    bool m_bFailed;     ///< \brief Holds whether reading some image has failed yet.
    unsigned int i;     ///< \brief Incrementely counts which file is being read currently.
    unsigned int n;     ///< \brief Holds count of files to be read.

    std::deque< std::string > failedFiles;

}; // QtDicomSeriesOpeningController



}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna

#endif // QTDICOMSERIESOPENINGCONTROLLER_H_6014714286
