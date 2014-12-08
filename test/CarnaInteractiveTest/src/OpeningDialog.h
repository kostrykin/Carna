#pragma once

#include <Carna/Carna.h>
#include <Carna/base/model/SceneFactory.h>
#include <Carna/base/model/Scene.h>
#include <Carna/base/qt/DicomController.h>
#include <QMainWindow>
#include <QDialog>
#include <QVBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <CompressedVolumeImporter.h>



// ----------------------------------------------------------------------------------
// OpeningDialog
// ----------------------------------------------------------------------------------

class OpeningDialog : public QDialog
{

    Q_OBJECT

private:

    Carna::base::model::SceneFactory sceneFactory;
    std::unique_ptr< Carna::base::model::Scene > scene;

public:

    OpeningDialog( unsigned int max_allowed_side_length )
        : sceneFactory( this )
    {
        if( max_allowed_side_length > 0 )
        {
            sceneFactory.setMaximumAllowedSideLength( max_allowed_side_length );
        }

        QMainWindow* const mainWindow = new QMainWindow();
        QVBoxLayout* const master = new QVBoxLayout( this );
        Carna::base::qt::DicomController* const sceneFactoryUI = new Carna::base::qt::DicomController();

        QObject::connect
            ( sceneFactoryUI, SIGNAL( seriesLoadingRequested( const Carna::base::model::SeriesLoadingRequest& ) )
            , this, SLOT( createFromRequest( const Carna::base::model::SeriesLoadingRequest& ) ) );

        mainWindow->setCentralWidget( sceneFactoryUI );
        mainWindow->setParent( this );
        master->addWidget( mainWindow );
        master->setContentsMargins( 0, 0, 0, 0 );

        QMenu* const fileMenu = mainWindow->menuBar()->addMenu( "&File" );
        fileMenu->addAction( "&Import compressed volume", this, SLOT( importCompressedVolume() ) );
        fileMenu->addSeparator();
        fileMenu->addAction( "E&xit", this, SLOT( reject() ) );

        this->resize( this->width(), 500 );
    }

    Carna::base::model::Scene* takeScene()
    {
        Carna::base::model::Scene* scene_ptr = scene.get();
        scene.release();
        return scene_ptr;
    }

public slots:

    void createFromRequest( const Carna::base::model::SeriesLoadingRequest& request )
    {
        scene.reset( sceneFactory.createFromRequest( request ) );
        this->accept();
    }

    void importCompressedVolume()
    {
        CompressedVolumeImporter importer( this );
        scene.reset( importer.run() );
        if( scene.get() != nullptr )
        {
            this->accept();
        }
    }

}; // OpeningDialog
