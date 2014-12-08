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

#include <Carna/MPR/DefaultAxialPlaneRenderer.h>
#include <Carna/MPR/DefaultAxialPlaneRendererDetails.h>
#include <Carna/MPR/SliderPainter.h>
#include <Carna/MPR/SliderScreenSpaceBuffer.h>
#include <Carna/MPR/AxialPlaneVisualization.h>
#include <Carna/base/model/Volume.h>
#include <Carna/base/model/Object3DEvent.h>
#include <Carna/base/model/Position.h>
#include <Carna/base/model/Object3D.h>
#include <Carna/base/view/glew.h>
#include <Carna/base/view/ShaderProgram.h>
#include <Carna/base/view/SceneProvider.h>
#include <Carna/base/view/ShaderBundle.h>
#include <Carna/base/CarnaException.h>
#include <Carna/base/Math.h>
#include <Carna/base/Visualization.h>
#include <Carna/base/VisualizationEnvironment.h>
#include <QMouseEvent>
#include <QMenu>
#include <QDebug>
#include <sstream>

namespace Carna
{

namespace MPR
{



// ----------------------------------------------------------------------------------
// MPR :: DefaultAxialPlaneRenderer
// ----------------------------------------------------------------------------------

DefaultAxialPlaneRenderer::DefaultAxialPlaneRenderer
    ( base::Visualization& module
    , base::view::SceneProvider& provider
    , QWidget* parent )

    : AxialPlaneRenderer
        ( module
        , provider
        , parent )
    , pimpl( new Details() )
{
    connect( pimpl->sliderPainter.get(), SIGNAL( changed() ), this, SLOT( invalidate() ) );

    glDepthFunc( GL_LEQUAL );
}


DefaultAxialPlaneRenderer::~DefaultAxialPlaneRenderer()
{
    module().environment().activateContext(); //< TODO: check whether this is obsolete
}


void DefaultAxialPlaneRenderer::setSliderScreenSpaceBuffer( SliderScreenSpaceBuffer* sliderScreenSpaceBuffer )
{
    CARNA_ASSERT( sliderScreenSpaceBuffer != nullptr );

    pimpl->sliderScreenSpaceBuffer.reset( sliderScreenSpaceBuffer );

    this->invalidate();
}


void DefaultAxialPlaneRenderer::setSliderPainter( SliderPainter* painter )
{
    CARNA_ASSERT( painter != nullptr );

    pimpl->sliderPainter.reset( painter );
    connect( pimpl->sliderPainter.get(), SIGNAL( changed() ), this, SLOT( invalidate() ) );

    this->invalidate();
}


void DefaultAxialPlaneRenderer::setIntersection( const base::Vector& insection )
{
    base::Vector newInsection( 0, 0, 0 );

    for( int i = 0; i < 3; ++i )
    {
        newInsection[ i ] = base::Math::clamp< double >( insection[ i ], 0, 1 );
    }

    if( newInsection == pimpl->intersection )
    {
        return;
    }

    pimpl->intersection = newInsection;

    emit intersectionChanged( pimpl->intersection );

    this->invalidate();
}


void DefaultAxialPlaneRenderer::setMaskMinHuv( int huv )
{
    pimpl->huv0 = huv;

    this->invalidate();
}


void DefaultAxialPlaneRenderer::setMaskMaxHuv( int huv )
{
    pimpl->huv1 = huv;

    this->invalidate();
}


void DefaultAxialPlaneRenderer::setHuvMaskOpacity( float huvMaskOpacity )
{
    pimpl->huvMaskOpacity = huvMaskOpacity;

    this->invalidate();
}


void DefaultAxialPlaneRenderer::fetchSceneMatrix
    ( base::Transformation& sceneMatrix
    , const base::Transformation& cameraMatrix
    , const base::Transformation& projectionMatrix ) const
{
    using base::Transformation;

 // scene space

    Renderer::fetchSceneMatrix( sceneMatrix, cameraMatrix, projectionMatrix );

 // volume space

    sceneMatrix = sceneMatrix * Transformation().translate( -0.5, -0.5, -0.5 );
}


void DefaultAxialPlaneRenderer::renderColor()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // bind windowing shader

    {
        base::view::ShaderProgram::Binding shaderBinding( shader( "sliceplane-windowing" ) );

        shaderBinding.putUniform1i( "intensityTexture", 0 );
        shaderBinding.putUniform1i( "windowingLevel", provider.scene.recommendedWindowingLevel() );
        shaderBinding.putUniform1i( "windowingWidth", provider.scene.recommendedWindowingWidth() );
        shaderBinding.putUniform1f( "border", 0 );
        shaderBinding.putUniform1i( "huv0", pimpl->huv0 );
        shaderBinding.putUniform1i( "huv1", pimpl->huv1 );
        shaderBinding.putUniform1f( "maskOpacity", pimpl->huvMaskOpacity );

        // draw slices

        glBindTexture( GL_TEXTURE_3D, volumeTextureID() );

        const base::Vector& intersection = pimpl->intersection;

        // z-slice

        glBegin( GL_TRIANGLE_FAN );

        glTexCoord3f( 0, 0, intersection[ 2 ] );
        glVertex3f( 0, 0, intersection[ 2 ] );

        glTexCoord3f( 0, 1, intersection[ 2 ] );
        glVertex3f( 0, 1, intersection[ 2 ] );

        glTexCoord3f( 1, 1, intersection[ 2 ] );
        glVertex3f( 1, 1, intersection[ 2 ] );

        glTexCoord3f( 1, 0, intersection[ 2 ] );
        glVertex3f( 1, 0, intersection[ 2 ] );

        glEnd();

        // x-slice

        glBegin( GL_TRIANGLE_FAN );

        glTexCoord3f( intersection[ 0 ], 0, 0 );
        glVertex3f( intersection[ 0 ], 0, 0 );

        glTexCoord3f( intersection[ 0 ], 0, 1 );
        glVertex3f( intersection[ 0 ], 0, 1 );

        glTexCoord3f( intersection[ 0 ], 1, 1 );
        glVertex3f( intersection[ 0 ], 1, 1 );

        glTexCoord3f( intersection[ 0 ], 1, 0 );
        glVertex3f( intersection[ 0 ], 1, 0 );

        glEnd();

        // y-slice

        glBegin( GL_TRIANGLE_FAN );

        glTexCoord3f( 0, intersection[ 1 ], 0 );
        glVertex3f( 0, intersection[ 1 ], 0 );

        glTexCoord3f( 0, intersection[ 1 ], 1 );
        glVertex3f( 0, intersection[ 1 ], 1 );

        glTexCoord3f( 1, intersection[ 1 ], 1 );
        glVertex3f( 1, intersection[ 1 ], 1 );

        glTexCoord3f( 1, intersection[ 1 ], 0 );
        glVertex3f( 1, intersection[ 1 ], 0 );

        glEnd();

    }

    pimpl->sliderPainter->paint( pimpl->intersection );
    pimpl->sliderScreenSpaceBuffer->update( pimpl->intersection, *this );

    // prepare painting 3D-objects

    glBlendFunc( GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA );
    glBlendColor( 1, 1, 1, 0.5f );

    // draw objects

    glEnable( GL_BLEND );
    glDisable( GL_DEPTH_TEST );

    this->paintObjects3D();

    glDisable( GL_BLEND );
    glEnable( GL_DEPTH_TEST );

    this->paintObjects3D();

    glDepthMask( GL_TRUE );
}


void DefaultAxialPlaneRenderer::renderDepth()
{
}


const base::Vector& DefaultAxialPlaneRenderer::intersection() const
{
    return pimpl->intersection;
}


int DefaultAxialPlaneRenderer::minimalMaskHuv() const
{
    return pimpl->huv0;
}


int DefaultAxialPlaneRenderer::maximalMaskHuv() const
{
    return pimpl->huv1;
}


SliderPainter& DefaultAxialPlaneRenderer::sliderPainter()
{
    return *pimpl->sliderPainter;
}


const SliderPainter& DefaultAxialPlaneRenderer::sliderPainter() const
{
    return *pimpl->sliderPainter;
}


const SliderScreenSpaceBuffer& DefaultAxialPlaneRenderer::sliderScreenSpaceBuffer() const
{
    return *pimpl->sliderScreenSpaceBuffer;
}



}  // namespace MPR

}  // namespace Carna
