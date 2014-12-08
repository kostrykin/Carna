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
#include <Carna/VolumeRenderings/DefaultVolumeRendererModeFrame.h>
#include <Carna/VolumeRenderings/VolumeRendererMode.h>

namespace Carna
{

namespace VolumeRenderings
{



// ----------------------------------------------------------------------------------
// DefaultVolumeRendererModeFrame
// ----------------------------------------------------------------------------------

DefaultVolumeRendererModeFrame::DefaultVolumeRendererModeFrame( VolumeRendererMode& mode )
    : mode( mode )
{
    glPushAttrib( GL_ALL_ATTRIB_BITS );
    mode.start();
}


DefaultVolumeRendererModeFrame::~DefaultVolumeRendererModeFrame()
{
    mode.stop();
    glPopAttrib();
}



}  // namespace Carna :: VolumeRenderings

}  // namespace Carna
