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

#ifndef MIP_CHANNELREPLACEFUNCTION_H_6014714286
#define MIP_CHANNELREPLACEFUNCTION_H_6014714286

/** \file   ChannelReplaceFunction.h
  * \brief  Defines \ref Carna::VolumeRenderings::MIP::ChannelReplaceFunction.
  */

#include <Carna/VolumeRenderings/MIP/GenericChannelFunction.h>

namespace Carna
{

namespace VolumeRenderings
{

namespace MIP
{



// ----------------------------------------------------------------------------------
// VolumeRenderings :: MIP :: ChannelReplaceFunction
// ----------------------------------------------------------------------------------

/** \brief  Makes the higher prioritized channel replace the lower one.
  *
  * Sets \f$ \mathrm{ cf }_k = 1 - f_k( \max a(x) )_{ \text{alpha} } \f$.
  *
  * \author Leonid Kostrykin
  * \date   26.7.11
  */
class CARNA_EXPORT ChannelReplaceFunction : public GenericChannelFunction< ChannelReplaceFunction >
{

public:

    /** \brief Sets <code>glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA )</code>
      */
    virtual void makeCurrent() const override;

}; // VolumeRenderings :: MIP :: ChannelReplaceFunction



}  // namespace Carna :: VolumeRenderings :: MIP

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // MIP_CHANNELREPLACEFUNCTION_H_6014714286
