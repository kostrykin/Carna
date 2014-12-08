#pragma once

#include <HUIO.h>
#include <Carna/Carna.h>
#include <Carna/base/model/Scene.h>
#include <Carna/base/model/UInt16Volume.h>
#include <fstream>
#include <QDebug>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>

namespace Carna
{

namespace testing
{



/** \brief      Reads value of type \a ValueType from stream of type \a StreamType.
  * \relates    HUGZSceneFactory
  */
template< typename StreamType, typename ValueType >
void stream_read( StreamType& in, ValueType& out )
{
    in.read( reinterpret_cast< char* >( &out ), sizeof( out ) );
}

/** \brief  Creates \ref Carna::base::model::Scene object from HUGZ-file.
  *
  * \section HUGZFileFormat HUGZ File Format
  *
  * The HUGZ file is GZIP compression of the following data:
  * -# Bytes 1 to 4 are an unsigned integer that describes the volume width.
  * -# Bytes 5 to 8 are an unsigned integer that describes the volume height.
  * -# Bytes 9 to 12 are an unsigned integer that describes the volume depth.
  * -# Bytes 13 to 16 are an IEEE 754 single precision floating point number that describes the x-spacing.
  * -# Bytes 17 to 20 are an IEEE 754 single precision floating point number that describes the y-spacing.
  * -# Bytes 21 to 24 are an IEEE 754 single precision floating point number that describes the z-spacing.
  * -# Each voxel is represented as a \c signed \c short.
  *
  * \todo   Use \c int16_t instead of \c signed \c short.
  */
struct HUGZSceneFactory
{
    /** \brief  Reads HUGZ file and returns created \ref Carna::base::model::Scene object.
      *
      * The HUGZ file format is described \ref HUGZFileFormat here.
      */
    static Carna::base::model::Scene* importVolume( const std::string& filename )
    {
        std::ifstream file( filename, std::ios::in | std::ios::binary );
        boost::iostreams::filtering_istream in;
        in.push( boost::iostreams::gzip_decompressor() );
        in.push( file );

        Carna::base::Vector3ui size;
        stream_read( in, size.x );
        stream_read( in, size.y );
        stream_read( in, size.z );
        qDebug( "Read volume size: %d, %d, %d", size.x, size.y, size.z );

        Carna::base::model::UInt16Volume* const volume = new Carna::base::model::UInt16Volume( size );

        TRTK::Coordinate< float > spacing( 0, 0, 0 );
        stream_read( in, spacing.x() );
        stream_read( in, spacing.y() );
        stream_read( in, spacing.z() );
        qDebug( "Read volume spacing: %f, %f, %f", spacing.x(), spacing.y(), spacing.z() );

        HUIO::Reader reader( in );
        for( unsigned int z = 0; z < size.z; ++z )
        for( unsigned int y = 0; y < size.y; ++y )
        for( unsigned int x = 0; x < size.x; ++x )
        {
            const signed short huv = reader.read();
            volume->setVoxel( x, y, z, huv );
        }

        Carna::base::model::Scene* const scene = new Carna::base::model::Scene
            ( new Carna::base::Composition< Carna::base::model::Volume >( volume )
            , spacing.x()
            , spacing.y()
            , spacing.z() );

        return scene;
    }
};



}  // namespace testing

}  // namespace Carna
