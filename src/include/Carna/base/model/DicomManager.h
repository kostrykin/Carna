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

#ifndef DICOMMANAGER_H_6014714286
#define DICOMMANAGER_H_6014714286

/** \file   DicomManager.h
  * \brief  Defines \ref Carna::base::model::DicomManager.
  */

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>
#include <map>
#include <deque>
#include <QObject>

namespace Carna
{

namespace base
{

namespace qt
{
    class QtDicomSeriesOpeningController;
    struct ImageData;
}

namespace model
{

class Patient;
class Series;
class DicomExtractionSettings;



// ----------------------------------------------------------------------------------
// DicomManager
// ----------------------------------------------------------------------------------

/** \brief  Manages DICOM series.
  *
  * \author Leonid Kostrykin
  * \date   1.10.12 - 8.5.13
  */
class DicomManager : public QObject
{

    NON_COPYABLE

    Q_OBJECT

public:

    /** \brief  Instantiates.
      */
    DicomManager( QObject* parent = nullptr );

    /** \brief  Releases acquired resources.
      */
    ~DicomManager();


    const std::map< std::string, Patient* >& getPatients() const;

    const std::deque< std::string >& getFailedFiles() const;


public slots:

    void releasePatients();

    void openDirectory( const QString& );

    void openIndex( const QString& );

    void saveIndex( const QString& );

    void cancel();

    void extractSeries( const Carna::base::model::DicomExtractionSettings& );


signals:

    void finished();

    void failed();

    void failed( const QString& );

    void totalFilesCountChanged( unsigned int );

    void processedFilesCountChanged( unsigned int );


private:

    std::map< std::string, Patient* > patients;

    unsigned int totalFilesCount;

    std::unique_ptr< qt::QtDicomSeriesOpeningController > loadingController;

    std::deque< std::string > failedFiles;


private slots:

    void setTotalFilesCount( unsigned int );

    void setProgressedFilesCount( unsigned int );

    void processNewImage( const Carna::base::qt::ImageData& );

    void fail( const QString& );

}; // DicomManager



}  // namespace Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna

#endif // DICOMMANAGER_H_6014714286
