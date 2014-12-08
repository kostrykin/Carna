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

#include <Carna/MPR/PartialSliderPainter.h>

namespace Carna
{

namespace MPR
{



// ----------------------------------------------------------------------------------
// MPR :: PartialSliderPainter
// ----------------------------------------------------------------------------------

PartialSliderPainter::PartialSliderPainter( bool paintX
                                          , bool paintY
                                          , bool paintZ )
    : paintX( paintX )
    , paintY( paintY )
    , paintZ( paintZ )
{
}


void PartialSliderPainter::paint( const base::Vector& position )
{
    if( paintX )
    {
        paintXSlider( position[ 0 ] );
    }

    if( paintY )
    {
        paintYSlider( position[ 1 ] );
    }

    if( paintZ )
    {
        paintZSlider( position[ 2 ] );
    }
}



}  // namespace MPR

}  // namespace Carna
