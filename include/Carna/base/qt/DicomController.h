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

#ifndef DICOMCONTROLLER_H_6014714286
#define DICOMCONTROLLER_H_6014714286

/** \file   DicomController.h
  * \brief  Defines \ref Carna::base::qt::DicomController.
  */

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>
#include <QWidget>

class QDoubleSpinBox;
class QPushButton;
class QThread;
class QLabel;

namespace Carna
{

namespace base
{

namespace model
{
    class DicomManager;
    class DicomExtractionSettings;
}

namespace qt
{

class SeriesView;



// ----------------------------------------------------------------------------------
// DicomController
// ----------------------------------------------------------------------------------

/** \brief  Visual component for managing DICOM series and selecting for loading.
  *
  * \image html DicomController01.png
  *
  * Usage example:
  *
  * \code
  * #include <QObject>
  * #include <Carna/base/qt/DicomController.h>
  * #include <Carna/base/model/SeriesLoadingRequest.h>
  * #include <Carna/base/model/Scene.h>
  * #include <Carna/base/model/SceneFactory.h>
  *
  * using Carna::base::qt::DicomController;
  * using Carna::base::model::SeriesLoadingRequest;
  * using Carna::base::model::Scene;
  * using Carna::base::model::SceneFactory;
  *
  * class Loader : public QObject
  * {
  *
  *     Q_OBJECT
  *
  * public:
  *
  *     Loader( QWidget* dialogParent = nullptr )
  *         : modelFactory( new SceneFactory( dialogParent ) )
  *     {
  *         DicomController* controller = new DicomController();
  *         controller->show();
  *         
  *         connect( controller
  *                , SIGNAL( const Carna::base::model::SeriesLoadingRequest& )
  *                , this
  *                , SLOT( load( const Carna::base::model::SeriesLoadingRequest& ) ) );
  *     }
  *
  * signals:
  *
  *     void modelLoaded( const Scene* );
  *
  * private slots:
  *
  *     void load( const SeriesLoadingRequest& request )
  *     {
  *         const Scene* model = CarmodelFactory.createFromRequest( request );
  *         if( model != nullptr )
  *         {
  *             emit modelLoaded( model );
  *         }
  *     }
  *
  * private:
  *
  *     std::unique_ptr< SceneFactory > modelFactory;
  *
  * } // Loader
  * \endcode
  *
  * You might also want put the \c DicomController into a \c QDialog.
  *
  * \author Leonid Kostrykin
  * \date   5.10.12 - 8.10.12
  */
class CARNA_EXPORT DicomController : public QWidget
{

    NON_COPYABLE

    Q_OBJECT

public:

    /** \brief  Instantiates.
      */
    DicomController();

    /** \brief  Releases acquired resources.
      */
    ~DicomController();


signals:

    /** \cond   0
      */
    void openDirectory( const QString& );
    void openIndex( const QString& );
    void saveIndex( const QString& );
    void extractSeries( const Carna::base::model::DicomExtractionSettings& );
    /** \endcond
      */
    
    /** \brief  The user has intended to load some DICOM series.
      */
    void seriesLoadingRequested( const Carna::base::model::SeriesLoadingRequest& );


private:

    SeriesView* const seriesView;

    QLabel* const laSpacingZ;
    QDoubleSpinBox* const sbSpacingZ;

    QPushButton* const buSaveIndex;
    QPushButton* const buExtract;
    QPushButton* const buLoad;

    QThread* const managerThread;
    model::DicomManager* const manager;


private slots:

    void openDirectory();

    void openIndex();

    void saveIndex();

    void seriesLoaded();

    void seriesLoadingFailed( const QString& );

    void seriesLoadingAborted();

    void closeSeries();

    void indexSavingFailed( const QString& );

    void extractionFailed( const QString& );

    void selectionChanged();

    void seriesSelected( const Carna::base::model::Series& );

    void seriesDoubleClicked( const Carna::base::model::Series& );

    void extractSeries();

    void loadSeries();

}; // DicomController



}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna

#endif // DICOMCONTROLLER_H_6014714286
