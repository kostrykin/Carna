#pragma once

#include <HUIO.h>
#include <Carna/Carna.h>
#include <Carna/base/model/Scene.h>
#include <Carna/base/model/UInt16Volume.h>
#include <QFileDialog>
#include <QApplication>
#include <QDebug>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/device/file.hpp>

template< typename StreamType, typename ValueType, typename ValueRef >
void stream_write( StreamType& out, ValueRef data )
{
    out.write( reinterpret_cast< const char* >( &data ), sizeof( data ) );
}

template< typename StreamType, typename ValueType >
void stream_write( StreamType& out, const ValueType& data )
{
    stream_write< StreamType, ValueType, const ValueType& >( out, data );
}

class CompressedVolumeExporter
{

public:

    int exec( Carna::base::model::Scene& scene )
    {
        const QString filename = QFileDialog::getSaveFileName( nullptr, "Select file" );
        if( !filename.isEmpty() )
        {
            QApplication::setOverrideCursor( Qt::WaitCursor );
            exportVolume( scene, filename.toStdString() );
            QApplication::restoreOverrideCursor();
        }
        VolumeVisualizationDemo preview;
        return preview.exec( scene );
    }


private:

    void exportVolume( const Carna::base::model::Scene& scene, const std::string& filename )
    {
        const Carna::base::model::Volume& volume = scene.volume();

        boost::iostreams::filtering_ostream out;
        out.push( boost::iostreams::gzip_compressor() );
        out.push( boost::iostreams::file_sink( filename, std::ios::out | std::ios::binary ) );

        stream_write( out, volume.size.x );
        stream_write( out, volume.size.y );
        stream_write( out, volume.size.z );
        qDebug( "Written volume size: %d, %d, %d", volume.size.x, volume.size.y, volume.size.z );

        stream_write( out, scene.spacingX() );
        stream_write( out, scene.spacingY() );
        stream_write( out, scene.spacingZ() );
        qDebug( "Written volume spacings: %f, %f, %f", scene.spacingX(), scene.spacingY(), scene.spacingZ() );

        Carna::testing::HUIO::Writer writer( out );
        for( unsigned int z = 0; z < volume.size.z; ++z )
        for( unsigned int y = 0; y < volume.size.y; ++y )
        for( unsigned int x = 0; x < volume.size.x; ++x )
        {
            writer.write( volume( x, y, z ) );
        }
    }

};
