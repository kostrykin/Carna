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

#include <Carna/base/view/Point3D.h>
#include <Carna/base/view/Point3DDetails.h>
#include <Carna/base/view/gl.h>
#include <Carna/base/model/Object3DEvent.h>
#include <Carna/base/CarnaException.h>
#include <QPainter>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// Point3D :: Details
// ----------------------------------------------------------------------------------

qt::RotatingColor Point3D::Details::nextColor;


Point3D::Details::Details( Point3D& self, float size, const QColor& color )
    : self( self )
    , iconColor( 0, 0, 0 )
    , color( color )
    , size( size )
{
}


void Point3D::Details::rotateColor()
{
    self.setColor( nextColor++ );
}



// ----------------------------------------------------------------------------------
// Point3D
// ----------------------------------------------------------------------------------

const float Point3D::defaultSize = 6.f;


Point3D::Point3D( model::Scene& model, bool rotatingColor )
    : model::Object3D( model )
    , pimpl( new Details( *this, defaultSize, QColor( 0, 0, 0 ) ) )
{
    if( rotatingColor )
    {
        pimpl->rotateColor();
    }
}


Point3D::Point3D( model::Scene& model
                , const model::Position& position
                , float size
                , bool rotatingColor )
    : model::Object3D( model, position )
    , pimpl( new Details( *this, size, QColor( 0, 0, 0 ) ) )
{
    if( rotatingColor )
    {
        pimpl->rotateColor();
    }
}


Point3D::Point3D( model::Scene& model
                , const model::Position& position
                , const QColor& color
                , float size )
    : model::Object3D( model, position )
    , pimpl( new Details( *this, size, color ) )
{
}


Point3D::~Point3D()
{
}


void Point3D::setColor( const QColor& newColor )
{
    if( pimpl->color != newColor )
    {
        pimpl->color = newColor;

        invalidateObjects3D( model::Object3DEvent( model::Object3DEvent::shape ) );

        emit colorChanged();
        emit colorChanged( pimpl->color );
    }

    CARNA_ASSERT( color() == newColor );
}


const QColor& Point3D::color() const
{
    return pimpl->color;
}


void Point3D::setSize( float newSize )
{
    if( !Math::isEqual( pimpl->size, newSize ) )
    {
        pimpl->size = newSize;

        invalidateObjects3D( model::Object3DEvent( model::Object3DEvent::shape ) );

        emit sizeChanged();
        emit sizeChanged( float( pimpl->size ) );
        emit sizeChanged( double( pimpl->size ) );
    }

    CARNA_ASSERT( Math::isEqual( size(), newSize ) );
}


void Point3D::setSize( double size )
{
    setSize( static_cast< float >( size ) );
}


float Point3D::size() const
{
    return pimpl->size;
}


void Point3D::paint( const Renderer& renderer ) const
{
    glPointSize( pimpl->size + 2 );
    glColor3f( 1, 1, 1 );
    glBegin( GL_POINTS );
    glVertex3f( position().toVolumeUnits()[ 0 ]
              , position().toVolumeUnits()[ 1 ]
              , position().toVolumeUnits()[ 2 ] );
    glEnd();
    model::Object3D::paintFalseColor( renderer, pimpl->color );
}


void Point3D::paintFalseColor( const Renderer& renderer, const Vector3ui& vColor ) const
{
    glPointSize( pimpl->size );
    glColor3ub( vColor.x, vColor.y, vColor.z );
    glBegin( GL_POINTS );
    glVertex3f( position().toVolumeUnits()[ 0 ]
              , position().toVolumeUnits()[ 1 ]
              , position().toVolumeUnits()[ 2 ] );
    glEnd();
}


const QIcon& Point3D::icon() const
{
    const static double MATH_EPSILON = 1e-5;

    if( pimpl->iconColor != pimpl->color )
    {
        QPixmap pixmap( 10, 10 );

        QPainter painter( &pixmap );

        painter.setPen( Qt::black );
        painter.drawRect( pixmap.rect().adjusted( 0, 0, -1, -1 ) );
        painter.setPen( Qt::white );
        painter.setBrush( QBrush( pimpl->color ) );
        painter.drawRect( pixmap.rect().adjusted( 1, 1, -2, -2 ) );

        pimpl->icon = QIcon( pixmap );

        pimpl->iconColor = pimpl->color;
    }

    return pimpl->icon;
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
