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

#include <GL/glew.h>
#include "MyVisualization.h"
#include <Carna/base/view/SceneProvider.h>

namespace Carna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// MyRenderer
// ----------------------------------------------------------------------------------

MyRenderer::MyRenderer
    ( Carna::base::Visualization& module
    , Carna::base::view::SceneProvider& provider
    , QObject* parent )

    : Carna::base::view::Renderer
        ( module
        , provider
        , parent )
{

 // this is fairly optional

    glDisable( GL_DEPTH_TEST );
    glBlendFunc( GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR );

    glBindTexture( GL_TEXTURE_3D, volumeTextureID() );

}


void MyRenderer::fetchSceneMatrix
    ( Carna::base::Transformation& sceneMatrix
    , const Carna::base::Transformation& cameraMatrix
    , const Carna::base::Transformation& projectionMatrix ) const
{
    using Carna::base::Transformation;

 // scene space

    Carna::base::view::Renderer::fetchSceneMatrix( sceneMatrix, cameraMatrix, projectionMatrix );

 // volume space

    sceneMatrix = sceneMatrix * Transformation().translate( -0.5, -0.5, -0.5 );
}


void MyRenderer::renderColor()
{
    glClear( GL_COLOR_BUFFER_BIT );

    glEnable( GL_BLEND );
    glEnable( GL_TEXTURE_3D );

    for( float z = 0.f; z <= 1.f; z += 0.1f )
    {
        glBegin( GL_TRIANGLE_FAN );

        glTexCoord3f( 0, 0, z );
        glVertex3f( 0, 0, z );

        glTexCoord3f( 0, 1, z );
        glVertex3f( 0, 1, z );

        glTexCoord3f( 1, 1, z );
        glVertex3f( 1, 1, z );

        glTexCoord3f( 1, 0, z );
        glVertex3f( 1, 0, z );

        glEnd();
    }

    glDisable( GL_BLEND );
    glDisable( GL_TEXTURE_3D );

    this->paintObjects3D();
}


void MyRenderer::renderDepth()
{
}



// ----------------------------------------------------------------------------------
// MyVisualization
// ----------------------------------------------------------------------------------

Carna::base::view::Renderer* MyVisualization::createRenderer( Carna::base::view::SceneProvider& provider )
{
    return new MyRenderer( *this, provider );
}



}  // namespace testing

}  // namespace Carna
