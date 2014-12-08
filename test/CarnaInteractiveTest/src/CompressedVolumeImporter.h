#pragma once

#include <HUGZSceneFactory.h>
#include <QFileDialog>
#include <QApplication>

class CompressedVolumeImporter
{

    QWidget* const parent;

public:

    CompressedVolumeImporter( QWidget* parent )
        : parent( parent )
    {
    }

    Carna::base::model::Scene* run()
    {
        const QString filename = QFileDialog::getOpenFileName
            ( parent
            , "Select file"
            , ""
            , "HU values GZip file (*.hugz)" );
        Carna::base::model::Scene* scene = nullptr;
        if( !filename.isEmpty() )
        {
            QApplication::setOverrideCursor( Qt::WaitCursor );
            scene = Carna::testing::HUGZSceneFactory::importVolume( filename.toStdString() );
            QApplication::restoreOverrideCursor();
        }
        return scene;
    }

};
