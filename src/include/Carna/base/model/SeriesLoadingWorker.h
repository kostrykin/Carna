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

#ifndef SERIESLOADINGWORKER_H_6014714286
#define SERIESLOADINGWORKER_H_6014714286

/** \file   SeriesLoadingWorker.h
  * \brief  Defines Carna::base::model::SeriesLoadingWorker.
  */

#include <QThread>
#include <vector>

class DicomImage;   // from dicom-interface

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// SeriesLoadingWorker
// ----------------------------------------------------------------------------------

/** \class  SeriesLoadingWorker
  *
  * \brief  Thread which loads DICOM images.
  */
class SeriesLoadingWorker : public QObject
{

    Q_OBJECT

public:

    /** \brief  Defines ordered set of DICOM images.
      */
    typedef std::vector< DicomImage* > DicomImageVector;


    /** \brief  Instantiates.
      *
      * Loads DICOM images \f$\text{firstIndex},\text{firstIndex}+1,\dots,\text{firstIndex}+\text{numFiles}\f$ from file list \a filenames.
      */
    SeriesLoadingWorker( const QStringList& filenames
                       , unsigned int firstIndex
                       , unsigned int numFiles );


    /** \brief  References the ordered set of loaded DICOM images.
      */
    inline const DicomImageVector& getResult() const
    {
        return images;
    }

    /** \brief  Returns whether current job has been aborted.
      */
    inline bool isCanceled() const
    {
        return canceled;
    }

    unsigned int getWorkAmount() const
    {
        return numFiles;
    }


public slots:

    /** \brief  Performs the current job.
      *
      * Loads the specified DICOM images.
      */
    void run();


    /** \brief  Aborts the current job.
      */
    inline void cancel()
    {
        canceled = true;
    }


signals:

    /** \brief  Emitted when job finishes loading single DICOM image.
      */
    void progressed( int amount );

    /** \brief	Emitted when job has finished.
      */
    void finished();


private:

    /** \brief  List of files, the DICOM images are to be loaded from
      */
    const QStringList& filenames;

    /** \brief  Index of first file within SeriesLoadingWorker::filenames to load from.
      */
    const unsigned int firstIndex;

    /** \brief  Offset within SeriesLoadingWorker::filenames marking last file to load.
      */
    const unsigned int numFiles;

    /** \brief  Holds the loaded DICOM images.
      */
    DicomImageVector images;


    /** \brief  Holds whether the current job has been aborted.
      */
    bool canceled;

}; // SeriesLoadingWorker



}  // namespace Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna

#endif // SERIESLOADINGWORKER_H_6014714286
