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

#include <Carna/MPR/DefaultAxialPlaneRendererDetails.h>
#include <Carna/MPR/NullSliderPainter.h>
#include <Carna/MPR/NullSliderScreenSpaceBuffer.h>

namespace Carna
{
namespace MPR
{



// ----------------------------------------------------------------------------------
// MPR :: DefaultAxialPlaneRenderer :: Details
// ----------------------------------------------------------------------------------

DefaultAxialPlaneRenderer::Details::Details()
    : textureID( 0 )
    , sliderPainter( new NullSliderPainter() )
    , sliderScreenSpaceBuffer( new NullSliderScreenSpaceBuffer() )
    , intersection( 0.5, 0.5, 0.5 )
    , huv0( 3071 )
    , huv1( 3071 )
    , huvMaskOpacity( 0.8f )
{
}


DefaultAxialPlaneRenderer::Details::~Details()
{
}



}  // namespace MPR

}  // namespace Carna
