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

#include <Carna/base/view/glew.h>
#include <Carna/VolumeRenderings/MIP/Channel.h>

namespace Carna
{

namespace VolumeRenderings
{

namespace MIP
{



// ----------------------------------------------------------------------------------
// MIP :: Channel
// ----------------------------------------------------------------------------------

Channel::Channel
    ( const base::Span< HUV >& huRange
    , const base::math::Vector3f& color
    , float opacity
    , const base::view::BlendFunction& function )

    : huRange( huRange )
    , color( color )
    , opacity( opacity )
    , function( function )
{
}


Channel::Channel
    ( HUV firstHuv, HUV lastHuv
    , const base::math::Vector3f& color
    , float opacity
    , const base::view::BlendFunction& function )

    : huRange( firstHuv, lastHuv )
    , color( color )
    , opacity( opacity )
    , function( function )
{
}



}  // namespace Carna :: VolumeRenderings :: MIP

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna
