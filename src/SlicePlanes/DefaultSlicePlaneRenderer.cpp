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

#include <Carna/base/model/Scene.h>
#include <Carna/base/model/Volume.h>
#include <Carna/base/model/Object3D.h>
#include <Carna/base/view/glew.h>
#include <Carna/base/view/SceneProvider.h>
#include <Carna/base/view/ShaderProgram.h>
#include <Carna/base/view/Object3DPainter.h>
#include <Carna/base/view/ShaderBundle.h>
#include <Carna/base/CarnaException.h>
#include <Carna/base/Math.h>
#include <Carna/base/Visualization.h>
#include <Carna/base/VisualizationEnvironment.h>
#include <Carna/base/Transformation.h>
#include <Carna/SlicePlanes/DefaultSlicePlaneRenderer.h>
#include <Carna/SlicePlanes/DefaultSlicePlaneRendererDetails.h>
#include <Carna/SlicePlanes/SlicePlane.h>
#include <QDebug>

// uncomment this in order to enable extra debugging visualizations
// #define SLICE_PLANE_VIEW_DEBUG

namespace Carna
{

namespace SlicePlanes
{



// ----------------------------------------------------------------------------------
// Helpers
// ----------------------------------------------------------------------------------

template< typename T >
static TRTK::Coordinate< T > operator-( const TRTK::Coordinate< T >& c )
{
    return c * -1;
}



// ----------------------------------------------------------------------------------
// computeProportions
// ----------------------------------------------------------------------------------

static base::Vector computeProportions( const base::model::Scene& model )
{
    typedef base::Vector Vector;

    Vector proportions = Vector( model.volume().size.x
                               , model.volume().size.y
                               , model.volume().size.z )
                       * Vector( model.spacingX()
                               , model.spacingY()
                               , model.spacingZ() )
                       / std::max
                            ( model.volume().size.x
                            , std::max
                                ( model.volume().size.y
                                , model.volume().size.z ) );

    return proportions / std::max
                            ( proportions[ 0 ]
                            , std::max
                                ( proportions[ 1 ]
                                , proportions[ 2 ] ) );
}



// ----------------------------------------------------------------------------------
// computeScale
// ----------------------------------------------------------------------------------

static base::Vector computeScale( const base::model::Scene& model )
{
    const unsigned int max_component = std::max( model.volume().size.x
        , std::max( model.volume().size.y
        , model.volume().size.z ) );

    const base::Vector scale
        ( model.spacingX() * model.volume().size.x / max_component
        , model.spacingY() * model.volume().size.y / max_component
        , model.spacingZ() * model.volume().size.z / max_component );

    return scale;
}



// ----------------------------------------------------------------------------------
// DefaultSlicePlaneRenderer :: Details
// ----------------------------------------------------------------------------------

DefaultSlicePlaneRenderer::Details::Details( DefaultSlicePlaneRenderer& self )
    : self( self )
    , normal( 0, 0, 1 )
    , distance( 0.5 )
    , plane( nullptr )
    , border( DEFAULT_BORDER_WIDTH )
    , proportions( computeProportions( self.provider.scene ) )
    , scale( computeScale( self.provider.scene ) )
{
}


DefaultSlicePlaneRenderer::Details::~Details()
{
}


void DefaultSlicePlaneRenderer::Details::rebuildTexture()
{
    CARNA_ASSERT_EX
        ( plane->getResultTextureID() != 0
        , "GL has not been initialized yet" );

    const int windowingLevel = self.provider.scene.recommendedWindowingLevel();
    const unsigned int windowingWidth = self.provider.scene.recommendedWindowingWidth();

    plane->compute
        ( normal
        , distance
        , proportions
        , windowingLevel
        , windowingWidth
        , border );
}


void DefaultSlicePlaneRenderer::Details::rebuildTextureAndUpdate()
{
    rebuildTexture();
    self.invalidate();
}



// ----------------------------------------------------------------------------------
// DefaultSlicePlaneRenderer
// ----------------------------------------------------------------------------------

DefaultSlicePlaneRenderer::DefaultSlicePlaneRenderer
    ( base::Visualization& module
    , base::view::SceneProvider& provider
    , QObject* parent )

    : SlicePlaneRenderer( module, provider, parent )
    , pimpl( new Details( *this ) )
{
    connect( &provider.scene, SIGNAL( recommendedWindowingChanged() ), &*pimpl, SLOT( rebuildTextureAndUpdate() ) );

    pimpl->plane.reset( new SlicePlane( provider, module.environment() ) );

    pimpl->shader.reset( new base::view::ShaderBundle( "sliceplane-view" ) );

    pimpl->rebuildTexture();
}


DefaultSlicePlaneRenderer::~DefaultSlicePlaneRenderer()
{
    module().environment().activateContext();
}


bool DefaultSlicePlaneRenderer::setPlane( const base::Vector& normal, const double& distance, float border )
{
    CARNA_ASSERT( !base::Math::isEqual( normal.norm(), 0. ) );

    pimpl->normal   = normal;
    pimpl->distance = distance;
    pimpl->border   = border;

 // rebuild texture

    pimpl->rebuildTexture();

    if( !pimpl->plane->isValid() )
    {
        qDebug() << "Slice plane computation failed.";

        return false;
    }

 // post repaint event

    this->invalidate();
    return true;
}


void DefaultSlicePlaneRenderer::fetchSceneMatrix
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


void DefaultSlicePlaneRenderer::renderColor()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glScalef( 1 / pimpl->scale.x(), 1 / pimpl->scale.y(), 1 / pimpl->scale.z() );

    glPushMatrix();  //< scene origin

    glScalef( pimpl->scale.x(), pimpl->scale.y(), pimpl->scale.z() );

 // draw plane

    glPushMatrix();  //< volume space origin

    glMultMatrix( pimpl->plane->getTangentToVolume() );  //< tangent space

    glEnable( GL_TEXTURE_2D );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glBindTexture( GL_TEXTURE_2D, pimpl->plane->getResultTextureID() );

    {
        base::view::ShaderProgram::Binding shaderBinding( pimpl->shader->program() );
        shaderBinding.putUniform1i( "planeTexture", 0 );

        glBegin( GL_TRIANGLE_FAN );

            glTexCoord2f( 0, 0 );   glVertex3f( 0, 0, 0 );
            glTexCoord2f( 1, 0 );   glVertex3f( 1, 0, 0 );
            glTexCoord2f( 1, 1 );   glVertex3f( 1, 1, 0 );
            glTexCoord2f( 0, 1 );   glVertex3f( 0, 1, 0 );

        glEnd();
    }

    glDisable( GL_TEXTURE_2D );
    
 // draw plane frame

#ifdef SLICE_PLANE_VIEW_DEBUG

    {
        glLineWidth( 1.f );
        glColor4f( 0, 0.3f, 0, 1 );
        glBegin( GL_LINE_LOOP );
            glVertex3f( 0, 0, 0 );
            glVertex3f( 1, 0, 0 );
            glVertex3f( 1, 1, 0 );
            glVertex3f( 0, 1, 0 );
        glEnd();
    }

#endif

 // draw tangent space origin

#ifdef SLICE_PLANE_VIEW_DEBUG

    {
        const float l = 0.04f;
        glColor4f( 0.3f, 1, 0.3f, 1 );
        glBegin( GL_TRIANGLE_FAN );
            glVertex3f( 0, 0, 0 );
            glVertex3f( 0, l, 0 );
            glVertex3f( l, 0, 0 );
        glEnd();
    }

#endif

 // draw volume bounding box

    glPopMatrix();  //< volume space origin

#ifdef SLICE_PLANE_VIEW_DEBUG

    glLineWidth( 1.f );
    glColor4f( 0.7f, 0.7f, 0, 1 );
    glBegin( GL_LINE_LOOP );
        glVertex3f( 0, 0, 0 );
        glVertex3f( 1, 0, 0 );
        glVertex3f( 1, 1, 0 );
        glVertex3f( 0, 1, 0 );
    glEnd();
    glBegin( GL_LINE_LOOP );
        glVertex3f( 0, 0, 1 );
        glVertex3f( 1, 0, 1 );
        glVertex3f( 1, 1, 1 );
        glVertex3f( 0, 1, 1 );
    glEnd();
    glBegin( GL_LINES );
        glVertex3f( 0, 0, 0 );
        glVertex3f( 0, 0, 1 );
        glVertex3f( 1, 0, 0 );
        glVertex3f( 1, 0, 1 );
        glVertex3f( 1, 1, 0 );
        glVertex3f( 1, 1, 1 );
        glVertex3f( 0, 1, 0 );
        glVertex3f( 0, 1, 1 );
    glEnd();

 // draw volume space origin

    glPointSize( 5.f );
    glColor4f( 1, 1, 0, 1 );
    glBegin( GL_POINTS );
        glVertex3f( 0, 0, 0 );
    glEnd();

#endif

 // draw objects

    this->paintObjects3D();

 // draw axis

    glPopMatrix();  //< scene origin

    const float l = 0.07f;
    glLineWidth( 3.f );
    glBegin( GL_LINES );

        glColor4f( 1, 0, 0, 1 );
        glVertex3f( 0, 0, 0 );
        glVertex3f( l, 0, 0 );

        glColor4f( 0, 0.8f, 0, 1 );
        glVertex3f( 0, 0, 0 );
        glVertex3f( 0, l, 0 );

        glColor4f( 0.3f, 0.3f, 1, 1 );
        glVertex3f( 0, 0, 0 );
        glVertex3f( 0, 0, l );

    glEnd();
}


void DefaultSlicePlaneRenderer::renderDepth()
{
}


const base::Vector& DefaultSlicePlaneRenderer::getNormal() const
{
    return pimpl->normal;
}


double DefaultSlicePlaneRenderer::getDistance() const
{
    return pimpl->distance;
}


float DefaultSlicePlaneRenderer::getBorderWidth() const
{
    return pimpl->border;
}


const SlicePlane& DefaultSlicePlaneRenderer::getPlane() const
{
    return *pimpl->plane;
}



}  // namespace Carna :: SlicePlanes

}  // namespace Carna
