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

#ifndef MIP_CHANNELFUNCTION_H_6014714286
#define MIP_CHANNELFUNCTION_H_6014714286

/** \file   ChannelFunction.h
  * \brief  Defines \ref Carna::VolumeRenderings::MIP::ChannelFunction.
  */

#include <Carna/Carna.h>
#include <vector>

namespace Carna
{

namespace VolumeRenderings
{

namespace MIP
{



// ----------------------------------------------------------------------------------
// VolumeRenderings :: MIP :: ChannelFunction
// ----------------------------------------------------------------------------------

/** \brief  Interfaces channel overlay behavior definitions.
  *
  * \author Leonid Kostrykin
  * \date   26.7.11
  */
class CARNA_EXPORT ChannelFunction
{

public:

    /** \brief  Does nothing.
      */
    virtual ~ChannelFunction()
    {
    }

    /** \brief  Configures current GL state appropriately.
      */
    virtual void makeCurrent() const = 0;

    /** \brief  Retuns name of this channel function.
      */
    virtual const std::string getName() const = 0;

    /** \brief  Identifies the implementing class.
      */
    virtual bool isInstanceOf( const ChannelFunctionFactory& ) const = 0;

    
    /** \brief  Holds all known channel function factories.
      */
    static const std::vector< ChannelFunctionFactory* > factories;

    /** \brief  Gets a certain channel function factory by it's name from
      *         ChannelFunction::factories.
      *
      * \pre    There is a factory listed on \ref factories named as told.
      */
    static ChannelFunctionFactory& getFactory( const std::string& );

}; // VolumeRenderings :: MIP :: ChannelFunction



}  // namespace Carna :: VolumeRenderings :: MIP

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // MIP_CHANNELFUNCTION_H_6014714286
