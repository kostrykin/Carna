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

#include <Carna/base/glew.h>
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

const base::BlendFunction Channel::CHANNEL_FUNCTION_REPLACE( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
const base::BlendFunction Channel::CHANNEL_FUNCTION_ADD    ( GL_SRC_ALPHA, GL_ONE );


Channel::Channel
    ( const base::Span< base::HUV >& huRange
    , const base::math::Vector4f& color
    , const base::BlendFunction& function )

    : huRange( huRange )
    , color( color )
    , function( function )
{
}


Channel::Channel
    ( base::HUV firstHuv, base::HUV lastHuv
    , const base::math::Vector4f& color
    , const base::BlendFunction& function )

    : huRange( firstHuv, lastHuv )
    , color( color )
    , function( function )
{
}



}  // namespace Carna :: VolumeRenderings :: MIP

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna
