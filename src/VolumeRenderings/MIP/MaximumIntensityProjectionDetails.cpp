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

#include <Carna/VolumeRenderings/MIP/MaximumIntensityProjectionDetails.h>
#include <Carna/VolumeRenderings/MIP/Channel.h>
#include <Carna/VolumeRenderings/VolumeRenderer.h>
#include <Carna/base/view/FramebufferObject.h>
#include <Carna/base/view/RenderTexture.h>
#include <Carna/base/CarnaException.h>
#include <Carna/base/Visualization.h>
#include <Carna/base/VisualizationEnvironment.h>

namespace Carna
{

namespace VolumeRenderings
{

namespace MIP
{



// ----------------------------------------------------------------------------------
// MIP :: MaximumIntensityProjection :: Details
// ----------------------------------------------------------------------------------

MaximumIntensityProjection::Details::Details()
{
}


MaximumIntensityProjection::Details::~Details()
{
}


#define FIND_SLOT_BY_CHANNEL_IMPL \
    for( auto it = channelSlots.begin(); it != channelSlots.end(); ++it ) \
    { \
        if( it->channel.get() == &channel ) \
        { \
            return it; \
        } \
    } \
    return channelSlots.end();


MaximumIntensityProjection::Details::ChannelSlots::const_iterator
MaximumIntensityProjection::Details::findSlotByChannel( const Channel& channel ) const
{
    FIND_SLOT_BY_CHANNEL_IMPL
}


MaximumIntensityProjection::Details::ChannelSlots::iterator
    MaximumIntensityProjection::Details::findSlotByChannel( const Channel& channel )
{
    FIND_SLOT_BY_CHANNEL_IMPL
}



// ----------------------------------------------------------------------------------
// MIP :: MaximumIntensityProjection :: Details :: ChannelSlot
// ----------------------------------------------------------------------------------

MaximumIntensityProjection::Details::ChannelSlot::ChannelSlot
    ( MaximumIntensityProjection& mode
    , Channel* channel )

    : channel( channel )
{
    mode.renderer().module().environment().activateContext();

    texture.reset( new base::view::RenderTexture
        ( mode.renderer().shortestTargetSide()
        , mode.renderer().shortestTargetSide() ) );
}


MaximumIntensityProjection::Details::ChannelSlot::ChannelSlot( ChannelSlot&& other )
    : channel( other.channel.release() )
    , texture( other.texture.release() )
{
}


MaximumIntensityProjection::Details::ChannelSlot&
MaximumIntensityProjection::Details::ChannelSlot::operator=
    ( MaximumIntensityProjection::Details::ChannelSlot&& other )
{
    swap( other );
    return *this;
}


MaximumIntensityProjection::Details::ChannelSlot::~ChannelSlot()
{
}


void MaximumIntensityProjection::Details::ChannelSlot::swap( ChannelSlot& with )
{
    channel.swap( with.channel );
    texture.swap( with.texture );
}



}  // namespace Carna :: VolumeRenderings :: MIP

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna
