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

#ifndef MIP_MAXIMUMINTENSITYPROJECTION_DETAILS_H_6014714286
#define MIP_MAXIMUMINTENSITYPROJECTION_DETAILS_H_6014714286

/** \file   MaximumIntensityProjectionDetails.h
  * \brief  Defines \ref Carna::VolumeRenderings::MIP::MaximumIntensityProjection::Details.
  */

#include <Carna/VolumeRenderings/MIP/MaximumIntensityProjection.h>
#include <deque>

namespace Carna
{

namespace VolumeRenderings
{

namespace MIP
{



// ----------------------------------------------------------------------------------
// MIP :: MaximumIntensityProjection :: Details
// ----------------------------------------------------------------------------------

class MaximumIntensityProjection :: Details
{

public:

    // ------------------------------------------------------------------------------
    // MIP :: MaximumIntensityProjection :: Details :: ChannelSlot
    // ------------------------------------------------------------------------------

    class ChannelSlot
    {

        NON_COPYABLE

    public:

        ChannelSlot( MaximumIntensityProjection&, Channel* );

        explicit ChannelSlot( ChannelSlot&& );

        ~ChannelSlot();

        ChannelSlot& operator=( ChannelSlot&& );


        std::unique_ptr< Channel > channel;

        std::unique_ptr< base::view::RenderTexture > texture;


        void swap( ChannelSlot& );

    }; // ChannelSlot

    // ------------------------------------------------------------------------------
    // Constructors & Destructors
    // ------------------------------------------------------------------------------

    Details();

    ~Details();

    // ------------------------------------------------------------------------------
    // Helper Functions
    // ------------------------------------------------------------------------------

    typedef std::deque< ChannelSlot > ChannelSlots;

    ChannelSlots::iterator findSlotByChannel( const Channel& );

    ChannelSlots::const_iterator findSlotByChannel( const Channel& ) const;

    // ------------------------------------------------------------------------------
    // Payload
    // ------------------------------------------------------------------------------

    std::deque< ChannelSlot > channelSlots;

    std::unique_ptr< base::view::FramebufferObject > fbo;

}; // MIP :: MaximumIntensityProjection :: Details



}  // namespace Carna :: VolumeRenderings :: MIP

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // MIP_MAXIMUMINTENSITYPROJECTION_DETAILS_H_6014714286
