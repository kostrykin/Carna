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
#include <Carna/presets/MIPChannel.h>

namespace Carna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// MIP :: MIPChannel
// ----------------------------------------------------------------------------------

const base::BlendFunction MIPChannel::CHANNEL_FUNCTION_REPLACE( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
const base::BlendFunction MIPChannel::CHANNEL_FUNCTION_ADD    ( GL_SRC_ALPHA, GL_ONE );


MIPChannel::MIPChannel
    ( const base::Span< base::HUV >& huRange
    , const base::math::Vector4f& color
    , const base::BlendFunction& function )

    : huRange( huRange )
    , color( color )
    , myFunction( &function )
{
}


MIPChannel::MIPChannel
    ( base::HUV firstHuv, base::HUV lastHuv
    , const base::math::Vector4f& color
    , const base::BlendFunction& function )

    : huRange( firstHuv, lastHuv )
    , color( color )
    , myFunction( &function )
{
}


void MIPChannel::setFunction( const base::BlendFunction& channelFunction )
{
    myFunction = &channelFunction;
}


const base::BlendFunction& MIPChannel::function() const
{
    return *myFunction;
}



}  // namespace Carna :: presets

}  // namespace Carna
