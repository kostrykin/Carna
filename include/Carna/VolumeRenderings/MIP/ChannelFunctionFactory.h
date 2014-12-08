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

#ifndef MIP_CHANNELFUNCTIONFACTORY_H_6014714286
#define MIP_CHANNELFUNCTIONFACTORY_H_6014714286

/** \file   MaximumIntensityProjection.h
  * \brief  Defines \ref Carna::VolumeRenderings::MIP::MaximumIntensityProjection.
  */

#include <Carna/Carna.h>

namespace Carna
{

namespace VolumeRenderings
{

namespace MIP
{



// ----------------------------------------------------------------------------------
// VolumeRenderings :: MIP :: ChannelFunctionFactory
// ----------------------------------------------------------------------------------

/** \brief  Instantiates MipViewChannelFunction implementations.
  *
  * \author Leonid Kostrykin
  * \date   28.7.11
  */
class CARNA_EXPORT ChannelFunctionFactory
{

public:

    /** \brief  Does nothing.
      */
    virtual ~ChannelFunctionFactory()
    {
    }

    /** \brief  Instantiates some ChannelFunction.
      */
    virtual ChannelFunction* create() = 0;

    /** \brief  Returns name of channel functions created by this factory.
      */
    virtual const std::string& getName() const = 0;

}; // VolumeRenderings :: MIP :: ChannelFunctionFactory



}  // namespace Carna :: VolumeRenderings :: MIP

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // MIP_CHANNELFUNCTIONFACTORY_H_6014714286
