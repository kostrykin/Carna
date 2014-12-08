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

#include <Carna/base/view/gl.h>
#include <Carna/base/Transformation.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// fetchCurrentMatrix
// ----------------------------------------------------------------------------------

static void FetchCurrentMatrix( Transformation& out, GLenum which_matrix )
{
    double matrix[ 16 ];
    
    glGetDoublev( which_matrix, matrix );

    for( int i = 0; i < 16; ++i )
    {
        out.getTransformationMatrix()( i % 4, i / 4 ) = matrix[ i ];
    }
}



// ----------------------------------------------------------------------------------
// fetchModelViewMatrix
// ----------------------------------------------------------------------------------

void fetchModelViewMatrix( Transformation& out )
{
    FetchCurrentMatrix( out, GL_MODELVIEW_MATRIX );
}



// ----------------------------------------------------------------------------------
// fetchProjectionMatrix
// ----------------------------------------------------------------------------------

void fetchProjectionMatrix( Transformation& out )
{
    FetchCurrentMatrix( out, GL_PROJECTION_MATRIX );
}



}  // namespace Carna :: base

}  // namespace Carna



// ----------------------------------------------------------------------------------
// glTranslate
// ----------------------------------------------------------------------------------

void glTranslate( const Carna::base::Vector& t )
{
    glTranslatef( t.x(), t.y(), t.z() );
}



// ----------------------------------------------------------------------------------
// glMultMatrix
// ----------------------------------------------------------------------------------

void glMultMatrix( const Carna::base::Transformation& m )
{
    float a[ 16 ];
    for( int i = 0; i < 16; ++i )
    {
        a[ i ] = m.getTransformationMatrix()( i % 4, i / 4 );
    }
    glMultMatrixf( a );
}



// ----------------------------------------------------------------------------------
// glLoadMatrix
// ----------------------------------------------------------------------------------

void glLoadMatrix( const Carna::base::Transformation& m )
{
    glLoadIdentity();
    glMultMatrix( m );
}
