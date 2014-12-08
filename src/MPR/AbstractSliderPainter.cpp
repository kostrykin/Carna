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
#include <Carna/MPR/AbstractSliderPainter.h>

namespace Carna
{

namespace MPR
{



// ----------------------------------------------------------------------------------
// MPR :: AbstractSliderPainter
// ----------------------------------------------------------------------------------

void AbstractSliderPainter::paintXSlider( float x )
{
    glLineWidth( width );

    glColor4f( 1, 0, 0, 1 );
    glBegin( GL_LINE_LOOP );

        glVertex3f( x, 0, 0 );
        glVertex3f( x, 0, 1 );
        glVertex3f( x, 1, 1 );
        glVertex3f( x, 1, 0 );

    glEnd();
}


void AbstractSliderPainter::paintYSlider( float y )
{
    glLineWidth( width );

    glColor4f( 0, 1, 0, 1 );
    glBegin( GL_LINE_LOOP );

        glVertex3f( 0, y, 0 );
        glVertex3f( 0, y, 1 );
        glVertex3f( 1, y, 1 );
        glVertex3f( 1, y, 0 );

    glEnd();
}


void AbstractSliderPainter::paintZSlider( float z )
{
    glLineWidth( width );

    glColor4f( 0, 0, 1, 1 );
    glBegin( GL_LINE_LOOP );

        glVertex3f( 0, 0, z );
        glVertex3f( 0, 1, z );
        glVertex3f( 1, 1, z );
        glVertex3f( 1, 0, z );

    glEnd();
}



}  // namespace MPR

}  // namespace Carna
