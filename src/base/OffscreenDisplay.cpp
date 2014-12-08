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

#include <Carna/base/OffscreenDisplay.h>
#include <Carna/base/OffscreenVisualizationEnvironment.h>
#include <Carna/base/view/Renderer.h>
#include <Carna/base/view/FramebufferObject.h>
#include <Carna/base/view/RenderTexture.h>
#include <Carna/base/Visualization.h>
#include <Carna/base/CarnaException.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// OffscreenDisplay :: Details
// ----------------------------------------------------------------------------------

class OffscreenDisplay::Details
{

    OffscreenDisplay& self;

public:

    Details
        ( OffscreenDisplay& self
        , OffscreenVisualizationEnvironment& environment
        , unsigned int width
        , unsigned int height
        , Visualization* module );


    const std::unique_ptr< Visualization > module;

    bool invalidated;

    bool framebufferGrabbed;

    unsigned int width, height;

    std::unique_ptr< view::FramebufferObject > frameBuffer;

    std::unique_ptr< view::RenderTexture > colorBuffer;

    QImage grabbedFrameBuffer;


    bool repaint();

    void updateGrabbedFrameBuffer();

    void grabFramebuffer();

}; // OffscreenDispaly :: Details


OffscreenDisplay::Details::Details
    ( OffscreenDisplay& self
    , OffscreenVisualizationEnvironment& environment
    , unsigned int width
    , unsigned int height
    , Visualization* module )

    : self( self )
    , module( module )
    , invalidated( true )
    , framebufferGrabbed( false )
    , width( width )
    , height( height )
{
    environment.activateContext();
    CARNA_GLEW_INIT;

    width = std::max( 1u, width );
    height = std::max( 1u, height );

    updateGrabbedFrameBuffer();

    environment.activateContext();

    colorBuffer.reset( new view::RenderTexture( width, height ) );
    frameBuffer.reset( new view::FramebufferObject( *colorBuffer ) );

    CARNA_ASSERT( colorBuffer->id != 0 );
    CARNA_ASSERT( frameBuffer->id != 0 );
}


bool OffscreenDisplay::Details::repaint()
{
    if( invalidated )
    {
        self.paint();
        return true;
    }
    else
    {
        return false;
    }
}


void OffscreenDisplay::Details::updateGrabbedFrameBuffer()
{
    grabbedFrameBuffer = QImage( width, height, QImage::Format_RGB888 );
}


void OffscreenDisplay::Details::grabFramebuffer()
{
    framebufferGrabbed = true;

 // TODO: move this functionality to FramebufferObject::MinimalBinding class

    self.module().environment().activateContext();

    view::FramebufferObject::MinimalBinding binding( *frameBuffer );

    glReadBuffer( GL_COLOR_ATTACHMENT0_EXT );
    glReadPixels( 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, grabbedFrameBuffer.bits() );

    grabbedFrameBuffer = grabbedFrameBuffer.mirrored();
}



// ----------------------------------------------------------------------------------
// OffscreenDisplay
// ----------------------------------------------------------------------------------

OffscreenDisplay::OffscreenDisplay
    ( OffscreenVisualizationEnvironment& environment
    , unsigned int width, unsigned int height
    , Visualization* module
    , view::SceneProvider& provider )

    : environment( environment )
    , pimpl( new Details( *this, environment, width, height, module ) ) // activates the environment context
{
    CARNA_ASSERT( !module->isInitialized() );

 // ----------------------------------------------------------------------------------

    pimpl->module->initialize( provider, environment );
    pimpl->module->renderer().setTargetSize( pimpl->width, pimpl->height );

 // ----------------------------------------------------------------------------------

    CARNA_ASSERT( module->isInitialized() );
}


OffscreenDisplay::~OffscreenDisplay()
{
    environment.activateContext();
}


Visualization& OffscreenDisplay::module()
{
    return *pimpl->module;
}


const Visualization& OffscreenDisplay::module() const
{
    return *pimpl->module;
}


void OffscreenDisplay::resize( unsigned int width, unsigned int height )
{
    CARNA_ASSERT( module().isInitialized() );

    width = std::max( 1u, width );
    height = std::max( 1u, height );

    if( pimpl->width != width || pimpl->height != height )
    {
        pimpl->width = width;
        pimpl->height = height;

        pimpl->updateGrabbedFrameBuffer();

        pimpl->frameBuffer->resize( pimpl->width, pimpl->height );
        pimpl->colorBuffer->resize( pimpl->width, pimpl->height );

        module().renderer().setTargetSize( pimpl->width, pimpl->height );
        this->invalidate();

        CARNA_ASSERT( isInvalidated() );
    }

    CARNA_ASSERT( this->width() == width );
    CARNA_ASSERT( this->height() == height );
}


unsigned int OffscreenDisplay::width() const
{
    return pimpl->width;
}


unsigned int OffscreenDisplay::height() const
{
    return pimpl->height;
}

void OffscreenDisplay::invalidate()
{
    pimpl->invalidated = true;
}


bool OffscreenDisplay::isInvalidated() const
{
    return pimpl->invalidated;
}


void OffscreenDisplay::paint()
{
    CARNA_ASSERT( module().isInitialized() );

    module().environment().activateContext();

    view::FramebufferObject::Binding binding( *pimpl->frameBuffer );

    module().renderer().paint();

    pimpl->invalidated = false;
    pimpl->framebufferGrabbed = false;

    CARNA_ASSERT( !isInvalidated() );
}


QImage& OffscreenDisplay::framebuffer()
{
    if( pimpl->repaint() || !pimpl->framebufferGrabbed )
    {
        pimpl->grabFramebuffer();
    }

    return pimpl->grabbedFrameBuffer;
}


unsigned int OffscreenDisplay::textureID() const
{
    CARNA_ASSERT( pimpl->colorBuffer->id > 0 );

    return pimpl->colorBuffer->id;
}



}  // namespace Carna :: base

}  // namespace Carna
