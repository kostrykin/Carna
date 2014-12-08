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
#include <Carna/base/view/SceneProvider.h>
#include <Carna/base/view/RenderTexture.h>
#include <Carna/base/view/Monoscopic.h>
#include <Carna/base/Transformation.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// Monoscopic :: Details
// ----------------------------------------------------------------------------------

struct Monoscopic :: Details
{

    std::unique_ptr< RenderTexture > colorBuffer;

}; // Monoscopic :: Details



// ----------------------------------------------------------------------------------
// Monoscopic
// ----------------------------------------------------------------------------------

Monoscopic::Monoscopic( Renderer& renderer )
    : Renderer::RenderMode( renderer )
    , pimpl( new Details() )
{
}


Monoscopic::~Monoscopic()
{
}


void Monoscopic::postInitialize()
{
    if( !pimpl->colorBuffer.get() )
    {
        pimpl->colorBuffer.reset
            ( new RenderTexture
                ( renderer.shortestTargetSide()
                , renderer.shortestTargetSide() ) );
    }
}


void Monoscopic::paint()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

 // render scene to color buffer

    glMatrixMode( GL_PROJECTION );
    glLoadMatrix( renderer.projectionTransformation() );

    glMatrixMode( GL_MODELVIEW );
    glLoadMatrix( renderer.volumeViewTransformation() );

    renderColorTo( *pimpl->colorBuffer );

 // render color buffer to screen

    glPushAttrib( GL_ALL_ATTRIB_BITS );

    glDisable( GL_LIGHTING );
    glDisable( GL_DEPTH_TEST );
    glDepthMask( GL_FALSE );
    glActiveTexture( GL_TEXTURE0 );
    glEnable( GL_TEXTURE_2D );
    glDisable( GL_TEXTURE_3D );
    glBindTexture( GL_TEXTURE_2D, pimpl->colorBuffer->id );

    renderer.setSquareViewport();

    glColor4f( 1, 1, 1, 1 );

    glDisable( GL_BLEND );
    renderer.paintRectangle();

    glPopAttrib();
}


void Monoscopic::resizeBuffers( unsigned int w, unsigned int h )
{
    Renderer::RenderMode::resizeBuffers( w, h );

    const unsigned int s = std::min( w, h );

    pimpl->colorBuffer->resize( s, s );
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
