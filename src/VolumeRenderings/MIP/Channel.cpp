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

#include <Carna/VolumeRenderings/MIP/Channel.h>
#include <Carna/VolumeRenderings/MIP/ChannelFunction.h>

namespace Carna
{

namespace VolumeRenderings
{

namespace MIP
{



// ----------------------------------------------------------------------------------
// MIP :: Channel
// ----------------------------------------------------------------------------------

Channel::Channel( const base::IntSpan& huv
                , const QColor& minColor
                , const QColor& maxColor
                , float opacity
                , ChannelFunction* function )
    : huv( huv )
    , minColor( minColor )
    , maxColor( maxColor )
    , opacity( opacity )
    , function( function )
{
}

    
Channel::~Channel()
{
}


const ChannelFunction& Channel::getFunction() const
{
    return *function;
}


void Channel::setFunction( ChannelFunction* f )
{
    function.reset( f );
    emit changed();
}



}  // namespace Carna :: VolumeRenderings :: MIP

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna
