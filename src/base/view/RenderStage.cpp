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

#include <Carna/base/view/RenderStage.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// RenderStage
// ----------------------------------------------------------------------------------

RenderStage::RenderStage()
    : viewTransformFixed( true )
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



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
