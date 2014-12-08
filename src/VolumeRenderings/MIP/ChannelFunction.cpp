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

#include <Carna/VolumeRenderings/MIP/ChannelFunction.h>
#include <Carna/VolumeRenderings/MIP/ChannelReplaceFunction.h>
#include <Carna/VolumeRenderings/MIP/ChannelAddFunction.h>
#include <Carna/base/CarnaException.h>

namespace Carna
{

namespace VolumeRenderings
{

namespace MIP
{



// ----------------------------------------------------------------------------------
// MIP :: FactoriesContainer
// ----------------------------------------------------------------------------------

static class FactoriesContainer
{

public:

    std::vector< ChannelFunctionFactory* > factories;

    FactoriesContainer()
    {
        factories.push_back( new ChannelReplaceFunction::Factory() );
        factories.push_back( new     ChannelAddFunction::Factory() );
    }

    ~FactoriesContainer()
    {
        for( auto it = factories.begin(); it != factories.end(); ++it )
        {
            delete *it;
        }
    }

}
factoriesContainer;



// ----------------------------------------------------------------------------------
// MIP :: ChannelFunction
// ----------------------------------------------------------------------------------

const std::vector< ChannelFunctionFactory* > ChannelFunction::factories = factoriesContainer.factories;


ChannelFunctionFactory& ChannelFunction::getFactory( const std::string& name )
{
    for( std::vector< ChannelFunctionFactory* >::const_iterator it  = factories.begin();
                                                                it != factories.end();
                                                              ++it )
    {
        ChannelFunctionFactory& factory = **it;
        if( factory.getName() == name )
        {
            return factory;
        }
    }

    CARNA_FAIL( "no such channel factory '" << name << "'" );
}



}  // namespace Carna :: VolumeRenderings :: MIP

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna
