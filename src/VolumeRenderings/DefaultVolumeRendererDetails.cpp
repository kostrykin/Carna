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

#include <Carna/VolumeRenderings/DefaultVolumeRendererDetails.h>
#include <Carna/base/view/FramebufferObject.h>
#include <Carna/base/Math.h>

namespace Carna
{

namespace VolumeRenderings
{



// ----------------------------------------------------------------------------------
// DefaultVolumeRenderer :: Details
// ----------------------------------------------------------------------------------

DefaultVolumeRenderer::Details::Details( DefaultVolumeRenderer& self )
    : self( self )
    , rotationCenter( 0.5, 0.5, 0.5 )
    , sampleCount( std::sqrt( static_cast< double >
        ( base::Math::sq( self.provider.scene.volume().size.x )
        + base::Math::sq( self.provider.scene.volume().size.y )
        + base::Math::sq( self.provider.scene.volume().size.z ) ) ) )
    , colorPostProcessing( DefaultVolumeRenderer::noColorOperation )
    , mask( false )
    , flippedHorizontal( false )
    , flippedVertical( false )
    , rotation2D( 0 )
    , xMin( 0 )
    , yMin( 0 )
    , zMin( 0 )
    , xMax( 1 )
    , yMax( 1 )
    , zMax( 1 )
    , lazyModeInitializationScheduled( false )
{
}


DefaultVolumeRenderer::Details::~Details()
{
}



}  // namespace Carna :: VolumeRenderings

}  // namespace Carna
