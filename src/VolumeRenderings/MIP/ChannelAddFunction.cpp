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

#include <Carna/base/view/glew.h>
#include <Carna/VolumeRenderings/MIP/ChannelAddFunction.h>

namespace Carna
{

namespace VolumeRenderings
{

namespace MIP
{



// ----------------------------------------------------------------------------------
// MIP :: ChannelAddFunction
// ----------------------------------------------------------------------------------

const std::string GenericChannelFunction< ChannelAddFunction >::Factory::name = "Additive";

void ChannelAddFunction::makeCurrent() const
{
    glBlendFunc( GL_SRC_ALPHA, GL_ONE );
}



}  // namespace Carna :: VolumeRenderings :: MIP

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna
