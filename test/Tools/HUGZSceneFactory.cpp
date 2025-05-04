/*
 *  Copyright (C) 2010 - 2016 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 * 
 * 
 *  Copyright (C) 2021 - 2025 Leonid Kostrykin
 * 
 */

#include "HUGZSceneFactory.h"

namespace Carna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// stream_read
// ----------------------------------------------------------------------------------

template< typename StreamType, typename ValueType >
void stream_read( StreamType& in, ValueType& out )
{
    in.read( reinterpret_cast< char* >( &out ), sizeof( out ) );
}



// ----------------------------------------------------------------------------------
// HUGZSceneFactory
// ----------------------------------------------------------------------------------

Carna::base::HUVolumeUInt16* HUGZSceneFactory::importVolume( const std::string& filename, Carna::base::math::Vector3f& spacing, bool stretchIntensities )
{
    std::ifstream file( filename, std::ios::in | std::ios::binary );
    LIBCARNA_ASSERT( file.is_open() && !file.fail() );
    boost::iostreams::filtering_istream in;
    in.push( boost::iostreams::gzip_decompressor() );
    in.push( file );

    Carna::base::math::Vector3ui size;
    stream_read( in, size.x() );
    stream_read( in, size.y() );
    stream_read( in, size.z() );

    Carna::base::HUVolumeUInt16* const volume = new Carna::base::HUVolumeUInt16( size );

    stream_read( in, spacing.x() );
    stream_read( in, spacing.y() );
    stream_read( in, spacing.z() );

    HUIO::Reader reader( in );
    for( unsigned int z = 0; z < size.z(); ++z )
    for( unsigned int y = 0; y < size.y(); ++y )
    for( unsigned int x = 0; x < size.x(); ++x )
    {
        const signed short huv = reader.read();
        volume->setVoxel( x, y, z, Carna::base::HUV::abs( huv ) );
    }

    if( stretchIntensities )
    {
        const unsigned short minValue = *std::min_element( volume->buffer().begin(), volume->buffer().end() );
        for( auto& value : volume->buffer() )
        {
            value -= minValue;
        }
        const unsigned short maxValue = *std::max_element( volume->buffer().begin(), volume->buffer().end() );
        for( auto& value : volume->buffer() )
        {
            value = static_cast< unsigned short >( value * static_cast< float >( 0xFFFF ) / maxValue + 0.5f );
        }
        LIBCARNA_ASSERT( *std::min_element( volume->buffer().begin(), volume->buffer().end() ) == 0 );
        LIBCARNA_ASSERT( *std::max_element( volume->buffer().begin(), volume->buffer().end() ) == 0xFFFF );
    }

    return volume;
}



}  // namespace Carna :: testing

}  // namespace Carna
