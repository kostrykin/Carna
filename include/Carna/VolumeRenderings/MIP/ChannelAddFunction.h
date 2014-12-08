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

#ifndef MIP_CHANNELADDFUNCTION_H_6014714286
#define MIP_CHANNELADDFUNCTION_H_6014714286

/** \file   ChannelAddFunction.h
  * \brief  Defines \ref Carna::VolumeRenderings::MIP::ChannelAddFunction.
  */

#include <Carna/VolumeRenderings/MIP/GenericChannelFunction.h>

namespace Carna
{

namespace VolumeRenderings
{

namespace MIP
{



// ----------------------------------------------------------------------------------
// VolumeRenderings :: MIP :: ChannelAddFunction
// ----------------------------------------------------------------------------------

/** \brief  Provides additive blending for MIP channels.
  *
  * Sets \f$ \mathrm{ cf }_k = 1 \f$.
  *
  * \author Leonid Kostrykin
  * \date   26.7.11
  */
class CARNA_EXPORT ChannelAddFunction : public GenericChannelFunction< ChannelAddFunction >
{

public:

    /** \brief Sets <code>glBlendFunc( GL_SRC_ALPHA, GL_ONE )</code>
      */
    virtual void makeCurrent() const override;

}; // VolumeRenderings :: MIP :: ChannelAddFunction



}  // namespace Carna :: VolumeRenderings :: MIP

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // MIP_CHANNELADDFUNCTION_H_6014714286
