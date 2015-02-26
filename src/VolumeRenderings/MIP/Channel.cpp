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
    , const base::math::Vector4f& color
    , const base::view::BlendFunction& function )

    : huRange( huRange )
    , color( color )
    , function( function )
{
}


Channel::Channel
    ( HUV firstHuv, HUV lastHuv
    , const base::math::Vector4f& color
    , const base::view::BlendFunction& function )

    : huRange( firstHuv, lastHuv )
    , color( color )
    , function( function )
{
}



}  // namespace Carna :: VolumeRenderings :: MIP

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna
