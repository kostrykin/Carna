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
#include <Carna/base/view/Renderer.h>
#include <Carna/base/view/SceneProvider.h>
#include <Carna/base/view/ShaderProgram.h>
#include <Carna/base/view/RenderTexture.h>
#include <Carna/base/Transformation.h>
#include <Carna/stereoscopic/Zalman.h>
#include <QMessageBox>
#include <QDebug>

namespace Carna
{

namespace stereoscopic
{



// ----------------------------------------------------------------------------------
// Zalman :: Details
// ----------------------------------------------------------------------------------

struct Zalman :: Details
{

    Details();


    std::unique_ptr< base::view::RenderTexture > colorBuffer[ 2 ];

    bool initialized;

}; // Zalman :: Details


Zalman::Details::Details()
    : initialized( false )
{
}



// ----------------------------------------------------------------------------------
// Zalman
// ----------------------------------------------------------------------------------

Zalman::Zalman( base::view::Renderer& renderer )
    : base::view::Renderer::RenderMode( renderer )
    , pimpl( new Details() )
{
}


Zalman::~Zalman()
{
}


void Zalman::postInitialize()
{
    pimpl->initialized = true;

    for( unsigned int i = 0; i < 2; ++i )
    {
        if( !pimpl->colorBuffer[ i ].get() )
        {
            pimpl->colorBuffer[ i ].reset(
                new base::view::RenderTexture
                    ( renderer.shortestTargetSide()
                    , renderer.shortestTargetSide() ) );
        }
    }
}


void Zalman::paint()
{
    using namespace base;

    if( !pimpl->initialized )
    {
        return;
    }

 // render scene to color buffer

    const float d = 0.025f;

    glMatrixMode( GL_PROJECTION );
    glLoadMatrix( renderer.projectionTransformation() );
    glPushMatrix();

    glTranslatef( -d, 0, 0 );

    glMatrixMode( GL_MODELVIEW );
    glLoadMatrix( renderer.volumeViewTransformation() );
    glPushMatrix();

    renderColorTo( *pimpl->colorBuffer[ 0 ] );

    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();

    glMatrixMode( GL_PROJECTION );
    glPopMatrix();

    glTranslatef( +d, 0, 0 );

    glMatrixMode( GL_MODELVIEW );

    renderColorTo( *pimpl->colorBuffer[ 1 ] );

 // render color buffer to screen

    glPushAttrib( GL_ALL_ATTRIB_BITS );

    glDisable( GL_LIGHTING );
    glDisable( GL_DEPTH_TEST );
    glDepthMask( GL_FALSE );
    glActiveTexture( GL_TEXTURE0 );
    glEnable( GL_TEXTURE_2D );
    glDisable( GL_TEXTURE_3D );
    glDisable( GL_BLEND );

    renderer.setSquareViewport();

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    {
        base::view::ShaderProgram::Binding shaderBinding( renderer.shader( "zalman" ) );
        shaderBinding.putUniform1i( "sceneTexture", 0 );

        for( unsigned int i = 0; i < 2; ++i )
        {
            shaderBinding.putUniform1i( "mod2result", i );
            glBindTexture( GL_TEXTURE_2D, pimpl->colorBuffer[ i ]->id );
            renderer.paintRectangle();
        }
    }

    glPopAttrib();
}


void Zalman::resizeBuffers( unsigned int w, unsigned int h )
{
    base::view::Renderer::RenderMode::resizeBuffers( w, h );

    const unsigned int s = std::min( w, h );

    for( unsigned int i = 0; i < 2; ++i )
    {
        pimpl->colorBuffer[ i ]->resize( s, s );
    }
}



}  // namespace Carna :: stereoscopic

}  // namespace Carna
