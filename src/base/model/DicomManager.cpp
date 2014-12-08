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

#include <Carna/base/model/DicomManager.h>
#include <Carna/base/model/Patient.h>
#include <Carna/base/model/Study.h>
#include <Carna/base/model/Series.h>
#include <Carna/base/model/SeriesElement.h>
#include <Carna/base/model/DicomExtractionSettings.h>
#include <Carna/base/qt/QtDicomSeriesOpeningController.h>
#include <Carna/base/CarnaException.h>
#include <dicom-interface/dicomSeries.h>
#include <QDirIterator>
#include <QXmlStreamWriter>
#include <QXmlSimpleReader>
#include <QEventLoop>
#include <sstream>

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// XML Node Definitions
// ----------------------------------------------------------------------------------

const static QString XML_DICOM_INDEX = "DicomIndex";
const static QString XML_PATIENT = "Patient";
const static QString XML_PATIENT_NAME = "name";
const static QString XML_SERIES = "Series";
const static QString XML_SERIES_NAME = "name";
const static QString XML_STUDY = "Study";
const static QString XML_STUDY_NAME = "name";
const static QString XML_DICOM_IMAGE = "DicomImage";
const static QString XML_DICOM_IMAGE_FILE = "file";
const static QString XML_DICOM_IMAGE_ZPOSITION = "zPosition";



// ----------------------------------------------------------------------------------
// QtDicomSeriesFileIterator
// ----------------------------------------------------------------------------------

/** \brief  Qt based implementation of abstract \c DicomSeriesFileIterator class
  *
  * \see    SelectSeries
  * \author Leonid Kostrykin
  * \date   2010-2011
  */
class QtDicomSeriesFileIterator : public DicomSeriesFileIterator
{

public:

    /** \brief  Instantiates.
      */
    QtDicomSeriesFileIterator( const QString& sFolderName )
        : it( sFolderName, QDir::Files, QDirIterator::Subdirectories )
    {
    }


    /** \brief  Returns the next file.
      */
    virtual std::string getNextFile()
    {
        const std::string next = it.next().toStdString();
        return next;
    }

    /** \brief  Returns whether there is a next file.
      */
    virtual bool hasNextFile()
    {
        return it.hasNext();
    }


private:

    QDirIterator it;

}; // QtDicomSeriesFileIterator



// ----------------------------------------------------------------------------------
// DicomManager
// ----------------------------------------------------------------------------------

DicomManager::DicomManager( QObject* parent )
    : QObject( parent )
{
}


DicomManager::~DicomManager()
{
    releasePatients();
}


void DicomManager::releasePatients()
{
    for( auto it = patients.begin(); it != patients.end(); ++it )
    {
        delete it->second;
    }
    patients.clear();
}


void DicomManager::openDirectory( const QString& path )
{
    using namespace qt;

    releasePatients();

    failedFiles.clear();
    loadingController.reset( new qt::QtDicomSeriesOpeningController() );

    QtDicomSeriesFileIterator fileIterator( path );
    DicomSeries ds;

    connect( loadingController.get(), SIGNAL( onDirOpened( unsigned int ) ), this, SLOT( setTotalFilesCount( unsigned int ) ) );
    connect( loadingController.get(), SIGNAL( onDirIterationProgress( unsigned int ) ), this, SLOT( setProgressedFilesCount( unsigned int ) ) );
    connect( loadingController.get(), SIGNAL( onNewImage( const Carna::base::qt::ImageData& ) ), this, SLOT( processNewImage( const Carna::base::qt::ImageData& ) ) );
    connect( loadingController.get(), SIGNAL( onFailed( const QString& ) ), this, SLOT( fail( const QString& ) ) );

    ds.openDicomDir( fileIterator, *loadingController );

    loadingController.reset();
}


void DicomManager::cancel()
{
    if( loadingController.get() )
    {
        loadingController->abort();
    }
}


void DicomManager::setTotalFilesCount( unsigned int totalFilesCount )
{
    this->totalFilesCount = totalFilesCount;
    emit totalFilesCountChanged( totalFilesCount );
}


void DicomManager::setProgressedFilesCount( unsigned int progressedFilesCount )
{
    if( progressedFilesCount == totalFilesCount )
    {
        failedFiles = loadingController->getFailedFiles();
    }

    emit processedFilesCountChanged( progressedFilesCount );

    QEventLoop eventLoop;
    eventLoop.processEvents();

    if( progressedFilesCount == totalFilesCount )
    {
        emit finished();
    }
}


void DicomManager::processNewImage( const qt::ImageData& imageData )
{
    if( patients.find( imageData.patientID ) == patients.end() )
    {
        patients[ imageData.patientID ] = new Patient( imageData.patientID );
    }

    Patient& updated_patient = *patients[ imageData.patientID ];

 // ----------------------------------------------------------------------------------

    Study* updated_study = nullptr;
    const auto& studies = updated_patient.getStudies();
    for( auto study_itr = studies.begin(); study_itr != studies.end(); ++study_itr )
    {
        if( ( **study_itr ).name == imageData.studyID )
        {
            updated_study = *study_itr;
            break;
        }
    }

    if( updated_study == nullptr )
    {
        updated_study = new Study( imageData.studyID );
        updated_patient.put( updated_study );
    }

 // ----------------------------------------------------------------------------------

    Series* updated_series = nullptr;
    const auto& series = updated_study->getSeries();
    for( auto series_itr = series.begin(); series_itr != series.end(); ++series_itr )
    {
        if( ( **series_itr ).name == imageData.seriesID )
        {
            updated_series = *series_itr;
            break;
        }
    }

    if( updated_series == nullptr )
    {
        updated_series = new Series( imageData.seriesID );
        updated_study->put( updated_series );
    }

 // ----------------------------------------------------------------------------------

    updated_series->put( new SeriesElement( imageData.fileName, imageData.zPosition ) );
}


const std::map< std::string, Patient* >& DicomManager::getPatients() const
{
    return patients;
}


const std::deque< std::string >& DicomManager::getFailedFiles() const
{
    return failedFiles;
}


void DicomManager::saveIndex( const QString& fileName )
{
    QFile file( fileName );
    if( !file.open( QIODevice::WriteOnly | QIODevice::Truncate ) )
    {
        this->fail( "Failed opening '" + fileName + "' for writing." );
        return;
    }

    QXmlStreamWriter xml( &file );
    xml.setAutoFormatting( true );
    xml.writeStartDocument();

    xml.writeStartElement( XML_DICOM_INDEX );

    for( auto patient_itr = patients.begin(); patient_itr != patients.end(); ++patient_itr )
    {
        const Patient& patient = *( patient_itr->second );
        xml.writeStartElement( XML_PATIENT );
        xml.writeAttribute( XML_PATIENT_NAME, QString::fromStdString( patient.name ) );

        for( auto study_itr = patient.getStudies().begin(); study_itr != patient.getStudies().end(); ++study_itr )
        {
            const Study& study = **study_itr;
            xml.writeStartElement( XML_STUDY );
            xml.writeAttribute( XML_STUDY_NAME, QString::fromStdString( study.name ) );

            for( auto series_itr = study.getSeries().begin(); series_itr != study.getSeries().end(); ++series_itr )
            {
                const Series& series = **series_itr;
                xml.writeStartElement( XML_SERIES );
                xml.writeAttribute( XML_SERIES_NAME, QString::fromStdString( series.name ) );

                for( auto file_itr = series.getElements().begin(); file_itr != series.getElements().end(); ++file_itr )
                {
                    const SeriesElement& element = **file_itr;
                    xml.writeStartElement( XML_DICOM_IMAGE );
                    xml.writeAttribute( XML_DICOM_IMAGE_FILE, QString::fromStdString( element.fileName ) );
                    xml.writeAttribute( XML_DICOM_IMAGE_ZPOSITION, QString::number( element.zPosition ) );
                    xml.writeEndElement();
                }

                xml.writeEndElement();
            }

            xml.writeEndElement();
        }

        xml.writeEndElement();
    }

    xml.writeEndElement();
    xml.writeEndDocument();

    file.close();

    if( file.exists() )
    {
        emit finished();
    }
    else
    {
        this->fail( "Failed writing '" + fileName + "' for unknown reason." );
    }
}


void DicomManager::openIndex( const QString& fileName )
{
    releasePatients();

    QFile file( fileName );
    if( !file.exists() )
    {
        std::stringstream ss;
        ss << "Failed opening '" << fileName.toStdString() << "' for reading.";
        this->fail( QString::fromStdString( ss.str() ) );
        return;
    }

    QXmlSimpleReader xmlReader;
    QXmlInputSource xmlSource( &file );

    class SaxParser : public QXmlDefaultHandler
    {

    public:

        SaxParser( std::map< std::string, Patient* >& patients )
            : patients( patients )
            , currentScope( INDICES )
            , currentPatient( nullptr )
            , currentStudy( nullptr )
            , currentSeries( nullptr )
        {
        }

        virtual bool startElement( const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& attributes )
        {
            switch( currentScope )
            {

                case INDICES:
                {
                    if( localName == XML_DICOM_INDEX )
                    {
                        currentScope = PATIENTS;
                        break;
                    }
                    else
                    {
                        return false;
                    }
                }

                case PATIENTS:
                {
                    if( localName == XML_PATIENT )
                    {
                        currentPatient = new Patient( attributes.value( XML_PATIENT_NAME ).toStdString() );
                        patients[ currentPatient->name ] = currentPatient;

                        currentScope = STUDIES;
                        break;
                    }
                    else
                    {
                        return false;
                    }
                }

                case STUDIES:
                {
                    if( localName == XML_STUDY )
                    {
                        CARNA_ASSERT( currentPatient != nullptr );

                        currentStudy = new Study( attributes.value( XML_STUDY_NAME ).toStdString() );
                        currentPatient->put( currentStudy );

                        currentScope = SERIES;
                        break;
                    }
                    else
                    {
                        return false;
                    }
                }

                case SERIES:
                {
                    if( localName == XML_SERIES )
                    {
                        CARNA_ASSERT( currentPatient != nullptr );
                        CARNA_ASSERT( currentStudy != nullptr );

                        currentSeries = new Series( attributes.value( XML_SERIES_NAME ).toStdString() );
                        currentStudy->put( currentSeries );

                        currentScope = ELEMENTS;
                        break;
                    }
                    else
                    {
                        return false;
                    }
                }

                case ELEMENTS:
                {
                    if( localName == XML_DICOM_IMAGE )
                    {
                        CARNA_ASSERT( currentPatient != nullptr );
                        CARNA_ASSERT( currentStudy != nullptr );
                        CARNA_ASSERT( currentSeries != nullptr );

                        bool ok = false;
                        const std::string fileName = attributes.value( XML_DICOM_IMAGE_FILE ).toStdString();
                        const double zPosition = attributes.value( XML_DICOM_IMAGE_ZPOSITION ).toDouble( &ok );

                        if( !ok )
                        {
                            return false;
                        }

                        SeriesElement* const element = new SeriesElement( fileName, zPosition );
                        currentSeries->put( element );

                        currentScope = ELEMENT;
                        break;
                    }
                    else
                    {
                        return false;
                    }
                }

            }
            return true;
        }

        virtual bool endElement( const QString& namespaceURI, const QString& localName, const QString& qName )
        {
            switch( currentScope )
            {

                case PATIENTS:
                {
                    if( localName == XML_DICOM_INDEX )
                    {
                        currentScope = INDICES;
                        break;
                    }
                    else
                    {
                        return false;
                    }
                }

                case STUDIES:
                {
                    if( localName == XML_PATIENT )
                    {
                        currentScope = PATIENTS;
                        break;
                    }
                    else
                    {
                        return false;
                    }
                }

                case SERIES:
                {
                    if( localName == XML_STUDY )
                    {
                        currentScope = STUDIES;
                        break;
                    }
                    else
                    {
                        return false;
                    }
                }

                case ELEMENTS:
                {
                    if( localName == XML_SERIES )
                    {
                        currentScope = SERIES;
                        break;
                    }
                    else
                    {
                        return false;
                    }
                }

                case ELEMENT:
                {
                    if( localName == XML_DICOM_IMAGE )
                    {
                        currentScope = ELEMENTS;
                        break;
                    }
                    else
                    {
                        return false;
                    }
                }

            }
            return true;
        }

    private:

        std::map< std::string, Patient* >& patients;

        enum Scope
        {
            INDICES, PATIENTS, STUDIES, SERIES, ELEMENTS, ELEMENT
        }
        currentScope;

        Patient* currentPatient;
        Study* currentStudy;
        Series* currentSeries;

    }; // SaxParser

    SaxParser parser( patients );
    xmlReader.setContentHandler( &parser );
    xmlReader.setErrorHandler( &parser );

    if( !xmlReader.parse( xmlSource ) )
    {
        std::stringstream ss;
        ss << "Failed reading '" << fileName.toStdString() << "' due to unexpected format.";
        this->fail( QString::fromStdString( ss.str() ) );
        return;
    }

    emit finished();
}


void DicomManager::extractSeries( const DicomExtractionSettings& settings )
{
    unsigned long file_index = 0;
    QDir dst_dir( settings.destination );

 // compute files count

    std::deque< const SeriesElement* > elements;
    unsigned int files_count = 0;
    for( auto series_itr = settings.series.begin(); series_itr != settings.series.end(); ++series_itr )
    {
        const Series* const series = *series_itr;
        files_count += series->getElements().size();
        for( auto element_itr = series->getElements().begin(); element_itr != series->getElements().end(); ++element_itr )
        {
            const SeriesElement* const element = *element_itr;
            elements.push_back( element );
        }
    }

    emit totalFilesCountChanged( files_count );

 // copy files

    unsigned int processed_files = 0;
    for( auto element_itr = elements.begin(); element_itr != elements.end(); ++element_itr )
    {
        while( dst_dir.exists( QString::number( file_index ) ) )
        {
            ++file_index;
        }

        QString dst_filename = dst_dir.filePath( QString::number( file_index ) );
        
        const SeriesElement& element = **element_itr;
        QFile src( QString::fromStdString( element.fileName ) );
        if( !src.copy( dst_filename ) )
        {
            std::stringstream ss;
            ss << "Failed copying '" << element.fileName << "' to '" << dst_filename.toStdString() << "'";
            fail( QString::fromStdString( ss.str() ) );
            return;
        }

        emit processedFilesCountChanged( ++processed_files );
    }

 // finish

    emit finished();
}


void DicomManager::fail( const QString& message )
{
    releasePatients();

    emit failed( message );
    emit failed();
}



}  // namespace Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna
