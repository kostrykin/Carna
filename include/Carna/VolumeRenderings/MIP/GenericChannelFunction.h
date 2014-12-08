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

#ifndef MIP_GENERICCHANNELFUNCTION_H_6014714286
#define MIP_GENERICCHANNELFUNCTION_H_6014714286

/** \file   GenericChannelFunction.h
  * \brief  Defines \ref Carna::VolumeRenderings::MIP::GenericChannelFunction.
  */

#include <Carna/VolumeRenderings/MIP/ChannelFunction.h>
#include <Carna/VolumeRenderings/MIP/ChannelFunctionFactory.h>
#include <string>

namespace Carna
{

namespace VolumeRenderings
{

namespace MIP
{



// ----------------------------------------------------------------------------------
// VolumeRenderings :: MIP :: GenericChannelFunction
// ----------------------------------------------------------------------------------

/** \brief  Generic abstract implementation of ChannelFunction
  *
  * When deriving from this class, you will have to define
  * GenericChannelFunction::makeCurrent and GenericChannelFunction::Factory::name.
  *
  * \author Leonid Kostrykin
  * \date   26.7.11
  */
template< typename ConcreteFunction >
class CARNA_EXPORT GenericChannelFunction : public ChannelFunction
{

public:

    /** \brief  Returns GenericChannelFunction::Factory::name.
      */
    virtual const std::string getName() const override
    {
        return Factory::name;
    }

    /** \brief  Identifies the implementing class, based on name comparison.
      */
    virtual bool isInstanceOf( const ChannelFunctionFactory& fac ) const override
    {
        return getName() == fac.getName();
    }


    /** \brief  Factory which Instantiates \a ConcreteFunction.
      */
    class CARNA_EXPORT Factory : public ChannelFunctionFactory
    {

    public:

        /** \brief  Holds the name of channel functions created by this factory.
          */
        static const std::string name;


        virtual ChannelFunction* create() override
        {
            return new ConcreteFunction();
        }

        virtual const std::string& getName() const override
        {
            return name;
        }

    }; // GenericChannelFunction< ConcreteFunction > :: Factory

}; // VolumeRenderings :: MIP :: GenericChannelFunction



}  // namespace Carna :: VolumeRenderings :: MIP

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // MIP_GENERICCHANNELFUNCTION_H_6014714286
