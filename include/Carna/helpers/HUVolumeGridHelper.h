/*
 *  Copyright (C) 2010 - 2015 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#ifndef HUVOLUMEGRIDHELPER_H_6014714286
#define HUVOLUMEGRIDHELPER_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/math.h>
#include <Carna/base/HUVolumeGrid.h>
#include <Carna/base/HUVolumeSegment.h>

/** \file   HUVolumeGridHelper.h
  * \brief  Defines \ref Carna::helpers::HUVolumeGridHelper.
  */

namespace Carna
{

namespace helpers
{



// ----------------------------------------------------------------------------------
// HUVolumeGridHelper
// ----------------------------------------------------------------------------------

template< typename HUVolumeSegmentVolume >
class HUVolumeGridHelper
{

    NON_COPYABLE

    std::unique_ptr< base::HUVolumeGrid > myGrid;

public:

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    const static std::size_t DEFAULT_MAX_SEGMENT_BYTESIZE = 2 * 300 * 300 * 300;

    HUVolumeGridHelper( const base::math::Vector3ui& resolution, std::size_t maxSegmentBytesize = DEFAULT_MAX_SEGMENT_BYTESIZE );

    const base::math::Vector3ui resolution;

    const std::size_t maxSegmentBytesize;

    const base::math::Vector3ui maxSegmentSize;

    void loadData( const std::function< base::HUV( unsigned int, unsigned int, unsigned int ) >& );

    base::HUVolumeGrid& grid() const;

    struct Spacing
    {
        explicit Spacing( const base::math::Vector3f& millimters );

        const base::math::Vector3f& millimters;
    };

    struct Dimensions
    {
        explicit Dimensions( const base::math::Vector3f& millimters );

        const base::math::Vector3f& millimters;
    };

    base::Spatial* createSpatial( const base::GLContext& glc, const Spacing& spacing ) const;

    base::Spatial* createSpatial( const base::GLContext& glc, const Dimensions& dimensions ) const;

}; // HUVolumeGridHelper


template< typename HUVolumeSegmentVolume >
HUVolumeGridHelper< HUVolumeSegmentVolume >::HUVolumeGridHelper( const base::math::Vector3ui& resolution, std::size_t maxSegmentBytesize )
    : resolution( resolution )
    , maxSegmentBytesize( maxSegmentBytesize )
    , maxSegmentSize( [&resolution, maxSegmentBytesize]()->base::math::Vector3ui
            {
                const float maxSideLength = std::pow< float >( maxSegmentBytesize / 2.f, 1.3f / 3 );
                return base::math::round_ui( maxSideLength );
            }()
        )
{
    const base::math::Vector3ui segmentCounts
        ( resolution.x() / maxSegmentSize.x() + ( resolution.x() % maxSegmentSize.x() > 0 ? 1 : 0 )
        , resolution.y() / maxSegmentSize.y() + ( resolution.y() % maxSegmentSize.y() > 0 ? 1 : 0 )
        , resolution.z() / maxSegmentSize.z() + ( resolution.z() % maxSegmentSize.z() > 0 ? 1 : 0 ) );
    myGrid.reset( new HUVolumeGrid< HUVolumeSegmentVolume >( maxSegmentSize, segmentCounts ) )
}


template< typename HUVolumeSegmentVolume >
void HUVolumeGridHelper< HUVolumeSegmentVolume >::loadData( const std::function< base::HUV( unsigned int, unsigned int, unsigned int ) >& )
{
}


template< typename HUVolumeSegmentVolume >
base::HUVolumeGrid& HUVolumeGridHelper< HUVolumeSegmentVolume >::grid() const
{
    return *myGrid;
}


template< typename HUVolumeSegmentVolume >
base::Spatial* HUVolumeGridHelper< HUVolumeSegmentVolume >::createSpatial( const base::GLContext& glc, const Spacing& spacing ) const
{
    glc.makeActive();
}


template< typename HUVolumeSegmentVolume >
base::Spatial* HUVolumeGridHelper< HUVolumeSegmentVolume >::createSpatial( const base::GLContext& glc, const Dimensions& dimensions ) const
{
    glc.makeActive();
}


template< typename HUVolumeSegmentVolume >
HUVolumeGridHelper< HUVolumeSegmentVolume >::Spacing::Spacing( const base::math::Vector3f& millimters )
    : millimters( millimters )
{
}


template< typename HUVolumeSegmentVolume >
HUVolumeGridHelper< HUVolumeSegmentVolume >::Dimensions::Dimensions( const base::math::Vector3f& millimters )
    : millimters( millimters )
{
}



}  // namespace Carna :: helpers

}  // namespace Carna

#endif // HUVOLUMEGRIDHELPER_H_6014714286
