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
#include <Carna/base/view/glError.h>
#include <Carna/base/view/SceneProvider.h>
#include <Carna/VolumeRenderings/VolumeRendererSinglePassMode.h>
#include <Carna/VolumeRenderings/RayMarching.h>
#include <Carna/VolumeRenderings/VolumeRenderer.h>

namespace Carna
{

namespace VolumeRenderings
{



// ----------------------------------------------------------------------------------
// VolumeRendererSinglePassMode
// ----------------------------------------------------------------------------------

void VolumeRendererSinglePassMode::renderColor( RayMarching& pass )
{
    glPushAttrib( GL_ALL_ATTRIB_BITS );

    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glDepthMask( GL_TRUE );

    REPORT_GL_ERROR;

    {
        base::view::ShaderProgram::Binding shader( pass.renderer.shader( shaderID ) );

        REPORT_GL_ERROR;

        shader.putUniform1i( "intensityTexture", pass.volumeTextureSampler );
        shader.putUniform1i( "frontFacesTexture", pass.frontFacesTextureSampler );
        shader.putUniform1i( "backFacesTexture", pass.backFacesTextureSampler );
        shader.putUniform1i( "sample_count", pass.samples );

        REPORT_GL_ERROR;

        configureShader( pass, shader );

        REPORT_GL_ERROR;

        pass.renderer.paintRectangle();

        REPORT_GL_ERROR;
    }

    REPORT_GL_ERROR;

    if( isPaintingObjects() )
    {
        this->paintObjects3D();
        glColor4f( 1, 1, 1, 1 );
    }

    REPORT_GL_ERROR;

    glPopAttrib();
}


void VolumeRendererSinglePassMode::renderDepth( RayMarching& )
{
}



}  // namespace Carna :: VolumeRenderings

}  // namespace Carna
