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

#include <Carna/base/view/glu.h>
#include <Carna/base/view/DefaultCamera.h>
#include <Carna/base/view/DefaultCameraDetails.h>
#include <Carna/base/view/Renderer.h>
#include <Carna/base/model/Scene.h>
#include <Carna/base/model/Volume.h>
#include <Carna/base/CarnaException.h>
#include <Carna/base/Transformation.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// DefaultCamera
// ----------------------------------------------------------------------------------

DefaultCamera::Details::Details()
    : zoom( 1 )
{
}



// ----------------------------------------------------------------------------------
// DefaultCamera
// ----------------------------------------------------------------------------------

const static float focus = 2.f; ///< \brief Distance of scene center from lens


DefaultCamera::DefaultCamera
    ( const model::Scene& model
    , bool perspectiveProjection )

    : pimpl( new Details() )
{
    pimpl->perspectiveProjection = perspectiveProjection;

 // compute normalization matrix

    Vector dimensions
        ( model.volume().size.x * model.spacingX()
        , model.volume().size.y * model.spacingY()
        , model.volume().size.z * model.spacingZ() );

    const double longest_dimension = std::max( dimensions[ 0 ], std::max( dimensions[ 1 ], dimensions[ 2 ] ) );
    const double scale_factor = 1. / longest_dimension;

    pimpl->normalization = Transformation().scale( scale_factor, scale_factor, scale_factor );
}


DefaultCamera::~DefaultCamera()
{
}


void DefaultCamera::fetchCameraMatrix( Renderer& renderer, Transformation& out )
{
    out = Transformation().translate( 0, 0, -focus ) * pimpl->rotation * pimpl->normalization;
}


void DefaultCamera::fetchProjectionMatrix( Renderer& renderer, Transformation& out )
{
    GLint viewport[ 4 ];
    glGetIntegerv( GL_VIEWPORT, viewport );

    int matrixMode;
    glGetIntegerv( GL_MATRIX_MODE, &matrixMode );
    
    glMatrixMode( GL_PROJECTION );
    
    const float zField = std::sqrt( 3.f );
    const float zNear  = focus - zField / 2;
    const float zFar   = focus + zField / 2;

    glPushMatrix();
    glLoadIdentity();
    if( pimpl->perspectiveProjection )
    {
        gluPerspective( 45.f, 1.f, zNear, zFar );
    }
    else
    {
        glOrtho( -1, 1, -1, 1, zNear, zFar );
    }
    gluPickMatrix( renderer.shortestTargetSide() / 2 + pimpl->zoomPoint.x()
                 , renderer.shortestTargetSide() / 2 + pimpl->zoomPoint.y()
                 , viewport[ 2 ] / pimpl->zoom
                 , viewport[ 3 ] / pimpl->zoom
                 , viewport );
    base::fetchProjectionMatrix( out );
    glPopMatrix();

    glMatrixMode( matrixMode );
}


bool DefaultCamera::isPerspectiveProjection() const
{
    return pimpl->perspectiveProjection;
}


void DefaultCamera::setPerspectiveProjection( bool enabled )
{
    if( enabled != pimpl->perspectiveProjection )
    {
        pimpl->perspectiveProjection = enabled;
        emit viewChanged();
    }
}


void DefaultCamera::shift( float pixelsOffsetX, float pixelsOffsetY )
{
    pimpl->zoomPoint.rx() -= pixelsOffsetX / pimpl->zoom;
    pimpl->zoomPoint.ry() += pixelsOffsetY / pimpl->zoom;

    emit viewChanged();
}


void DefaultCamera::rotate( const Transformation& rotation )
{
    pimpl->rotation = rotation * pimpl->rotation;

    emit viewChanged();
}


void DefaultCamera::setRotation( const Transformation& rotation )
{
    pimpl->rotation = rotation;

    emit viewChanged();
}


void DefaultCamera::zoom( float zoomDelta, float zoomCenterX, float zoomCenterY )
{
    pimpl->zoom *= ( 1 + zoomDelta );  //< update zoom factor

    if( zoomDelta > 0 )
    {
        const int dx = zoomCenterX / pimpl->zoom;
        const int dy = zoomCenterY / pimpl->zoom;

        pimpl->zoomPoint.rx() += dx * zoomDelta;
        pimpl->zoomPoint.ry() -= dy * zoomDelta;
    }

    emit viewChanged();
}


float DefaultCamera::zoom() const
{
    return pimpl->zoom;
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
