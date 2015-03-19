/*
 *  Copyright (C) 2010 - 2015 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#include <Carna/base/RenderStage.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// RenderStage
// ----------------------------------------------------------------------------------

RenderStage::RenderStage()
    : viewTransformFixed( true )
    , enabled( true )
{
}


RenderStage::~RenderStage()
{
}


void RenderStage::setViewTransformFixed( bool viewTransformFixed )
{
    this->viewTransformFixed = viewTransformFixed;
}


bool RenderStage::isViewTransformFixed() const
{
    return viewTransformFixed;
}


void RenderStage::prepareFrame( Node& root )
{
    viewTransformFixed = true;
}


bool RenderStage::isEnabled() const
{
    return enabled;
}


void RenderStage::setEnabled( bool enabled )
{
    this->enabled = enabled;
}



}  // namespace Carna :: base

}  // namespace Carna
