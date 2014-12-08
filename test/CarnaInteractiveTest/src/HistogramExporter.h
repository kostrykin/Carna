#pragma once

#include <Carna/Carna.h>
#include <Carna/base/model/Scene.h>
#include <Carna/base/model/Histogram.h>
#include <QFileDialog>
#include <QApplication>
#include <QTextStream>

class HistogramExporter
{

public:

    int exec( Carna::base::model::Scene& scene )
    {
        const QString filename = QFileDialog::getSaveFileName( nullptr, "Select file" );
        if( !filename.isEmpty() )
        {
            QApplication::setOverrideCursor( Qt::WaitCursor );

            Carna::base::model::VolumeHistogram<> histogram( scene.volume(), scene.volume().size );
            histogram.updateCounter();

            QFile file( filename );
            file.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate );
            QTextStream out( &file );
            for( signed short huv = -1024; huv <= 3071; ++huv )
            {
                out << "frequency( huv = " << huv << " ) = " << histogram.counter[ huv ] << "\n";
            }
            file.close();

            QApplication::restoreOverrideCursor();
        }

        return 0;
    }

};
