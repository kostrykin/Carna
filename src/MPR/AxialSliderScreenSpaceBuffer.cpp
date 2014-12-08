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

#include <Carna/MPR/AxialSliderScreenSpaceBuffer.h>
#include <Carna/MPR/AxialPlaneRenderer.h>
#include <Carna/base/view/SceneProvider.h>

namespace Carna
{

namespace MPR
{



// ----------------------------------------------------------------------------------
// MPR :: AxialSliderScreenSpaceBuffer
// ----------------------------------------------------------------------------------

void AxialSliderScreenSpaceBuffer::update( const base::Vector& position3
                                         , const AxialPlaneRenderer& renderer )
{
    using base::Vector;

    const QSize size( renderer.targetWidth(), renderer.targetHeight() );

    const int dx = ( size.width () - renderer.shortestTargetSide() ) / 2;
    const int dy = ( size.height() - renderer.shortestTargetSide() ) / 2;

    const Vector pos_volspace( position3[ 0 ], position3[ 1 ], position3[ 2 ], 1 );
    const Vector pos_viewspace = renderer.projectionTransformation() * renderer.volumeViewTransformation() * pos_volspace;
    const Vector pos_scrspace = Vector( pos_viewspace[ 0 ], pos_viewspace[ 1 ] ) / 2
                              + Vector( 0.5, 0.5 );
    const Vector pos_winspace = ( Vector( 0, 1 ) + pos_scrspace * Vector( 1, -1 ) )
                              * renderer.shortestTargetSide()
                              + Vector( dx, dy );

    this->x = static_cast< int >( pos_winspace[ 0 ] + 0.5 );
    this->y = static_cast< int >( pos_winspace[ 1 ] + 0.5 );
}


SliderType AxialSliderScreenSpaceBuffer::hitTest( const QPoint& p ) const
{
    if( unsigned( std::abs( p.x() - x ) ) <= hitTestTolerance )
    {
        return SliderType( horizontal, SliderType::horizontal );
    }
    else
    if( unsigned( std::abs( p.y() - y ) ) <= hitTestTolerance )
    {
        return SliderType( vertical, SliderType::vertical );
    }
    else
    {
        return SliderType::null;
    }
}



}  // namespace MPR

}  // namespace Carna
