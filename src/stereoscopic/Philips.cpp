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
#include <Carna/base/view/ShaderProgram.h>
#include <Carna/base/view/RenderTexture.h>
#include <Carna/base/Transformation.h>
#include <Carna/stereoscopic/Philips.h>
#include <QMessageBox>

namespace Carna
{

namespace stereoscopic
{



// ----------------------------------------------------------------------------------
// Philips :: Details
// ----------------------------------------------------------------------------------

struct Philips :: Details
{

    Details();


    std::unique_ptr< base::view::RenderTexture > colorBuffer;
    std::unique_ptr< base::view::RenderTexture > depthBuffer;

    bool initialized;

}; // Philips :: Details


Philips::Details::Details()
    : initialized( false )
{
}



// ----------------------------------------------------------------------------------
// buildPhilipsHeader
// ----------------------------------------------------------------------------------

static std::vector< signed char > buildPhilipsHeader()
{
    std::vector< signed char > header( 768 * 3 );

#pragma warning( push )
#pragma warning( disable:4309 )

 // Header_ID1

    header[0*6+2] = 255;
    header[1*6+2] = 255;
    header[2*6+2] = 255;
    header[3*6+2] = 255;
    header[4*6+2] = 0;
    header[5*6+2] = 0;
    header[6*6+2] = 0;
    header[7*6+2] = 255;

 // Hdr_Content_type (00000001 = Signage)
    header[48+0*6+2] = 0;
    header[48+1*6+2] = 0;
    header[48+2*6+2] = 0;
    header[48+3*6+2] = 0;
    header[48+4*6+2] = 0;
    header[48+5*6+2] = 0;
    header[48+6*6+2] = 0;
    header[48+7*6+2] = 255;

 // unknown

    header[2*48+0*6+2] = 0;
    header[2*48+1*6+2] = 255;
    header[2*48+2*6+2] = 0;
    header[2*48+3*6+2] = 0;
    header[2*48+4*6+2] = 0;
    header[2*48+5*6+2] = 0;
    header[2*48+6*6+2] = 0;
    header[2*48+7*6+2] = 0;

 // unknown

    header[3*48+0*6+2] = 255;
    header[3*48+1*6+2] = 0;
    header[3*48+2*6+2] = 0;
    header[3*48+3*6+2] = 0;
    header[3*48+4*6+2] = 0;
    header[3*48+5*6+2] = 0;
    header[3*48+6*6+2] = 0;
    header[3*48+7*6+2] = 0;


    header[4*48+0*6+2] = 255;
    header[4*48+1*6+2] = 255;
    header[4*48+2*6+2] = 0;
    header[4*48+3*6+2] = 0;
    header[4*48+4*6+2] = 0;
    header[4*48+5*6+2] = 0;
    header[4*48+6*6+2] = 0;
    header[4*48+7*6+2] = 0;

    header[5*48+0*6+2] = 0;
    header[5*48+1*6+2] = 0;
    header[5*48+2*6+2] = 0;
    header[5*48+3*6+2] = 0;
    header[5*48+4*6+2] = 0;
    header[5*48+5*6+2] = 0;
    header[5*48+6*6+2] = 0;
    header[5*48+7*6+2] = 0;

 // CRC

    header[6*48+0*6+2] = 0;
    header[6*48+1*6+2] = 0;
    header[6*48+2*6+2] = 0;
    header[6*48+3*6+2] = 0;
    header[6*48+4*6+2] = 0;
    header[6*48+5*6+2] = 255;
    header[6*48+6*6+2] = 255;
    header[6*48+7*6+2] = 255;

    header[7*48+0*6+2] = 255;
    header[7*48+1*6+2] = 255;
    header[7*48+2*6+2] = 255;
    header[7*48+3*6+2] = 0;
    header[7*48+4*6+2] = 0;
    header[7*48+5*6+2] = 255;
    header[7*48+6*6+2] = 255;
    header[7*48+7*6+2] = 255;

    header[8*48+0*6+2] = 0;
    header[8*48+1*6+2] = 255;
    header[8*48+2*6+2] = 255;
    header[8*48+3*6+2] = 0;
    header[8*48+4*6+2] = 0;
    header[8*48+5*6+2] = 255;
    header[8*48+6*6+2] = 255;
    header[8*48+7*6+2] = 255;

    header[9*48+0*6+2] = 0;
    header[9*48+1*6+2] = 255;
    header[9*48+2*6+2] = 255;
    header[9*48+3*6+2] = 255;
    header[9*48+4*6+2] = 255;
    header[9*48+5*6+2] = 0;
    header[9*48+6*6+2] = 255;
    header[9*48+7*6+2] = 255;

 // Header ID2

    header[10*48+0*6+2] = 255;
    header[10*48+1*6+2] = 255;
    header[10*48+2*6+2] = 255;
    header[10*48+3*6+2] = 255;
    header[10*48+4*6+2] = 0;
    header[10*48+5*6+2] = 0;
    header[10*48+6*6+2] = 255;
    header[10*48+7*6+2] = 0;

 // Data_types (header)

    header[11*48+0*6+2] = 0;
    header[11*48+1*6+2] = 0;
    header[11*48+2*6+2] = 0;
    header[11*48+3*6+2] = 255;
    header[11*48+4*6+2] = 0;
    header[11*48+5*6+2] = 255;
    header[11*48+6*6+2] = 0;
    header[11*48+7*6+2] = 0;

 // i=12 -> Data_type (00000000 = 2D-plus-Depth format)

    for( int i = 12; i <= 17; i++ )
    {
        header[i*48+0*6+2] = 0;
        header[i*48+1*6+2] = 0;
        header[i*48+2*6+2] = 0;
        header[i*48+3*6+2] = 0;
        header[i*48+4*6+2] = 0;
        header[i*48+5*6+2] = 0;
        header[i*48+6*6+2] = 0;
        header[i*48+7*6+2] = 0;
    }

 // unknown

    header[18*48+0*6+2] = 255;
    header[18*48+1*6+2] = 0;
    header[18*48+2*6+2] = 255;
    header[18*48+3*6+2] = 0;
    header[18*48+4*6+2] = 0;
    header[18*48+5*6+2] = 0;
    header[18*48+6*6+2] = 0;
    header[18*48+7*6+2] = 0;

 // unknown

    for( int i = 19; i <= 27; i++ )
    {
        header[i*48+0*6+2] = 0;
        header[i*48+1*6+2] = 0;
        header[i*48+2*6+2] = 0;
        header[i*48+3*6+2] = 0;
        header[i*48+4*6+2] = 0;
        header[i*48+5*6+2] = 0;
        header[i*48+6*6+2] = 0;
        header[i*48+7*6+2] = 0;
    }

    header[28*48+0*6+2] = 255;
    header[28*48+1*6+2] = 255;
    header[28*48+2*6+2] = 255;
    header[28*48+3*6+2] = 255;
    header[28*48+4*6+2] = 0;
    header[28*48+5*6+2] = 255;
    header[28*48+6*6+2] = 255;
    header[28*48+7*6+2] = 0;

    header[29*48+0*6+2] = 0;
    header[29*48+1*6+2] = 255;
    header[29*48+2*6+2] = 0;
    header[29*48+3*6+2] = 0;
    header[29*48+4*6+2] = 255;
    header[29*48+5*6+2] = 0;
    header[29*48+6*6+2] = 0;
    header[29*48+7*6+2] = 0;

    header[30*48+0*6+2] = 0;
    header[30*48+1*6+2] = 255;
    header[30*48+2*6+2] = 255;
    header[30*48+3*6+2] = 0;
    header[30*48+4*6+2] = 255;
    header[30*48+5*6+2] = 255;
    header[30*48+6*6+2] = 255;
    header[30*48+7*6+2] = 0;

    header[31*48+0*6+2] = 255;
    header[31*48+1*6+2] = 255;
    header[31*48+2*6+2] = 255;
    header[31*48+3*6+2] = 255;
    header[31*48+4*6+2] = 0;
    header[31*48+5*6+2] = 255;
    header[31*48+6*6+2] = 255;
    header[31*48+7*6+2] = 0;

#pragma warning( pop )

    return header;
}



// ----------------------------------------------------------------------------------
// philipsHeader
// ----------------------------------------------------------------------------------

static const std::vector< signed char > philipsHeader = buildPhilipsHeader();



// ----------------------------------------------------------------------------------
// Philips
// ----------------------------------------------------------------------------------

Philips::Philips( base::view::Renderer& renderer )
    : base::view::Renderer::RenderMode( renderer )
    , pimpl( new Details() )
{
}


Philips::~Philips()
{
}


void Philips::postInitialize()
{
    pimpl->initialized = true;

 // setup render textures

    if( !pimpl->colorBuffer.get() )
    {
        pimpl->colorBuffer.reset( new base::view::RenderTexture
            ( renderer.shortestTargetSide()
            , renderer.shortestTargetSide() ) );
    }
    if( !pimpl->depthBuffer.get() )
    {
        pimpl->depthBuffer.reset( new base::view::RenderTexture
            ( renderer.shortestTargetSide()
            , renderer.shortestTargetSide() ) );
    }
}


void Philips::paint()
{
    using namespace base;

    if( !pimpl->initialized )
    {
        return;
    }

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

 // render scene to color buffer and depth to depth buffer

    glMatrixMode( GL_PROJECTION );
    glLoadMatrix( renderer.projectionTransformation() );

    glMatrixMode( GL_MODELVIEW );
    glLoadMatrix( renderer.volumeViewTransformation() );

    renderColorTo( *pimpl->colorBuffer );      //< paint color buffer

    glPushAttrib( GL_ALL_ATTRIB_BITS );

    glEnable( GL_DEPTH_TEST );
    glDepthMask( GL_TRUE );
    glDepthFunc( GL_LEQUAL );

    {
        base::view::ShaderProgram::Binding shaderBinding( renderer.shader( "philips" ) );

        renderDepthTo( *pimpl->depthBuffer );  //< paint depth buffer
    }

 // prepare GL states for final stage

    glDisable( GL_LIGHTING );
    glDisable( GL_DEPTH_TEST );
    glDepthMask( GL_FALSE );
    glActiveTexture( GL_TEXTURE0 );
    glEnable( GL_TEXTURE_2D );
    glDisable( GL_TEXTURE_3D );

 // -------------------------------------------- new ---------------------------------------------------

    glDisable( GL_BLEND );

 // save current viewport

    GLint viewport[ 4 ];
    glGetIntegerv( GL_VIEWPORT, viewport );

 // update viewport

    glViewport( 0, 0, renderer.targetWidth(), renderer.targetHeight() );

    /*
     *  o----o--------o----o----o--------o----o
     *  |    |        |    |    |        |    |
     *  o----o--------o----o----o--------o----o
     *  |    |        |    |
     *  |    |<------>|    |
     *  |    |   w/2       |
     *  |    |             |
     *  |<---|------------>|
     *  |    |   0.5
     *  |<-->|
     *    dx
     *
     *
     *  w  = screen_height / screen_width
     *
     *  dx = ( 0.5 - w/2 ) / 2
     *
     */

    const float w  = static_cast< float >( renderer.targetHeight() ) / renderer.targetWidth();
    const float dx = ( 0.5f - w / 2 ) / 2;

    glColor4f( 1, 1, 1, 1 );

 // render color buffer to screen

    glBindTexture( GL_TEXTURE_2D, pimpl->colorBuffer->id );

    renderer.paintRectangle( dx, 0, dx + w/2, 1 );

 // render depth buffer to screen

    glBindTexture( GL_TEXTURE_2D, pimpl->depthBuffer->id );

    renderer.paintRectangle( 0.5f + dx, 0, 1.0 - dx, 1 );

 // prepare drawing header

    glDisable( GL_DEPTH_TEST );
    glDisable( GL_TEXTURE_2D );
    glDisable( GL_TEXTURE_3D );

    glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );

    glClear( GL_DEPTH_BUFFER_BIT );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0, renderer.targetWidth(), 0, renderer.targetHeight(), 0, 3 );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

 // paint header

    glRasterPos2d( 0, renderer.targetHeight() - 1 );

    glDrawPixels( 768, 1, GL_RGB, GL_UNSIGNED_BYTE, &philipsHeader[ 0 ] );

 // restore viewport

    glViewport( viewport[ 0 ], viewport[ 1 ], viewport[ 2 ], viewport[ 3 ] );

 // restore previous GL states

    glPopAttrib();
}


void Philips::resizeBuffers( unsigned int w, unsigned int h )
{
    base::view::Renderer::RenderMode::resizeBuffers( w, h );

    const unsigned int s = std::min( w, h );

    pimpl->colorBuffer->resize( s, s );
    pimpl->depthBuffer->resize( s, s );
}



}  // namespace Carna :: stereoscopic

}  // namespace Carna
