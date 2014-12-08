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

#include <Carna/MPR/DefaultSliderPainter.h>

namespace Carna
{

namespace MPR
{



// ----------------------------------------------------------------------------------
// MPR :: DefaultSliderPainter
// ----------------------------------------------------------------------------------

DefaultSliderPainter::DefaultSliderPainter()
{
}


void DefaultSliderPainter::paint( const base::Vector& position )
{
    paintXSlider( position[ 0 ] );
    paintYSlider( position[ 1 ] );
    paintZSlider( position[ 2 ] );
}



}  // namespace MPR

}  // namespace Carna
