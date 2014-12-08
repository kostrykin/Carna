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

#include <Carna/base/qt/RotationView.h>
#include <Carna/base/Transformation.h>
#include <Carna/base/Math.h>
#include <TRTK/Coordinate.hpp>
#include <QPaintEvent>
#include <QPainter>

namespace Carna
{

namespace base
{

namespace qt
{



// ----------------------------------------------------------------------------------
// WorldToScreen
// ----------------------------------------------------------------------------------

Vector WorldToScreen( const Vector& v )
{
    Transformation tilt;
    tilt.rotateX( -45, Transformation::DEGREES );

    /*
    CMatrix tilt = RotationMatrix( -45.f, CVector( 1, 0, 0 ) );
    tilt.SetSize( 3, 3 );
    */

    Vector out = tilt * v;

    return out;
}



// ----------------------------------------------------------------------------------
// RotationView
// ----------------------------------------------------------------------------------

RotationView::RotationView( QWidget* parent )
    : QWidget( parent )
    /*
    , rotation( 3, 3 )
    */
    , showHorizontalPlane( true )
{
    /*
    rotation.SetUnit( 3 );
    */
}


void RotationView::paintEvent( QPaintEvent* )
{
    const float d = std::min( this->width(), this->height() ) * 0.37f;

    QPainter p( this );

 // unit vector

    Vector v( 1, 0, 0 );

 // apply rotation to unit vector

    v = rotation * v;

 // split: v = v_xz + v_y

    Vector v_xz( v[ 0 ], 0, v[ 2 ] );
    Vector v_y ( 0, v[ 1 ], 0 );

 // tilt & map

    v_xz = WorldToScreen( v_xz );
    v_y  = WorldToScreen( v_y  );

    const float a = std::sqrt( 1 - Math::sq( v[ 1 ] ) );
    Vector plane_h_dx = WorldToScreen( Vector( -a, 0, 0 ) );
    Vector plane_h_dz = WorldToScreen( Vector(  0, 0, a ) );

 // draw

    v_xz *=  d;
    v_y  *= -d;
    v = v_xz + v_y;

    plane_h_dx *= d;
    plane_h_dz *= d;

    p.setPen( Qt::black );
    p.drawLine( 0, height() / 2, width(), height() / 2 );
    p.drawLine( width() / 2, 0, width() / 2, height() );

    p.translate( width() / 2, height() / 2 );

    if( showHorizontalPlane )
    {
        const unsigned int w = 2 * std::abs( plane_h_dx[ 0 ] );
        const unsigned int h = 2 * std::abs( plane_h_dz[ 1 ] );
        p.drawEllipse( plane_h_dx[ 0 ], plane_h_dz[ 1 ] + v_y[ 1 ], w, -signed( h ) );
    }

    p.setPen( Qt::red );
    p.drawLine( 0, 0, v_xz[ 0 ], v_xz[ 1 ] );
    p.drawLine( v_xz[ 0 ], v_xz[ 1 ], v[ 0 ], v[ 1 ] );

    p.setPen( QPen( Qt::black, 3.f ) );
    p.drawPoint( v[ 0 ], v[ 1 ] );
}



}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna
