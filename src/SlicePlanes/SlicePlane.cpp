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

#include <Carna/SlicePlanes/SlicePlane.h>
#include <Carna/SlicePlanes/SlicePlaneDetails.h>
#include <Carna/base/Math.h>
#include <Carna/base/view/ShaderProgram.h>
#include <Carna/base/view/ShaderBundle.h>
#include <Carna/base/view/glew.h>
#include <Carna/base/view/FramebufferObject.h>
#include <Carna/base/model/Volume.h>
#include <Carna/base/model/Scene.h>
#include <Carna/base/VisualizationEnvironment.h>
#include <Carna/base/CarnaException.h>
#include <limits>
#include <QGLContext>
#include <QImage>
#include <QDebug>
#include <QGLFramebufferObject>

namespace Carna
{

namespace SlicePlanes
{



// ----------------------------------------------------------------------------------
// computeIntersectionPoints
// ----------------------------------------------------------------------------------

static void computeIntersectionPoints
    ( const base::Vector& n
    , const double d
    , std::vector< base::Vector >& set )
{
    using base::Vector;

    const double n1 = n[ 0 ];
    const double n2 = n[ 1 ];
    const double n3 = n[ 2 ];

    double k;

 // ------------------ left to right intersections ------------------

 // (0,0,0) to (1,0,0)

    k = d / n1;
    if( 0 <= k && k <= 1 ) set.push_back( Vector( k, 0, 0 ) );

 // (0,1,0) to (1,1,0)

    k = (d - n2) / n1;
    if( 0 <= k && k <= 1 ) set.push_back( Vector( k, 1, 0 ) );

 // (0,0,1) to (1,0,1)

    k = (d - n3) / n1;
    if( 0 <= k && k <= 1 ) set.push_back( Vector( k, 0, 1 ) );

 // (0,1,1) to (1,1,1)

    k = (d - n2 - n3) / n1;
    if( 0 <= k && k <= 1 ) set.push_back( Vector( k, 1, 1 ) );

 // ------------------ front to back intersections ------------------

 // (0,0,0) to (0,0,1)

    k = d / n3;
    if( 0 <= k && k <= 1 ) set.push_back( Vector( 0, 0, k ) );

 // (0,1,0) to (0,1,1)

    k = (d - n2) / n3;
    if( 0 <= k && k <= 1 ) set.push_back( Vector( 0, 1, k ) );

 // (1,0,0) to (1,0,1)

    k = (d - n1) / n3;
    if( 0 <= k && k <= 1 ) set.push_back( Vector( 1, 0, k ) );

 // (1,1,0) to (1,1,1)

    k = (d - n1 - n2) / n3;
    if( 0 <= k && k <= 1 ) set.push_back( Vector( 1, 1, k ) );

 // ------------------ bottom to top intersections ------------------

 // (0,0,0) to (0,1,0)

    k = d / n2;
    if( 0 <= k && k <= 1 ) set.push_back( Vector( 0, k, 0 ) );

 // (1,0,0) to (1,1,0)

    k = (d - n1) / n2;
    if( 0 <= k && k <= 1 ) set.push_back( Vector( 1, k, 0 ) );

 // (0,0,1) to (0,1,1)

    k = (d - n3) / n2;
    if( 0 <= k && k <= 1 ) set.push_back( Vector( 0, k, 1 ) );

 // (1,0,1) to (1,1,1)

    k = (d - n1 - n3) / n2;
    if( 0 <= k && k <= 1 ) set.push_back( Vector( 1, k, 1 ) );
}



// ----------------------------------------------------------------------------------
// VEC_Cross
// ----------------------------------------------------------------------------------

template< typename T >
static TRTK::Coordinate< T > VEC_Cross( const TRTK::Coordinate< T >& a
                                      , const TRTK::Coordinate< T >& b )
{
    const T& a1 = a[ 0 ];
    const T& a2 = a[ 1 ];
    const T& a3 = a[ 2 ];

    const T& b1 = b[ 0 ];
    const T& b2 = b[ 1 ];
    const T& b3 = b[ 2 ];

    return TRTK::Coordinate< T >( a2 * b3 - a3 * b2
                                , a3 * b1 - a1 * b3
                                , a1 * b2 - a2 * b1 );
}



// ----------------------------------------------------------------------------------
// dotProduct
// ----------------------------------------------------------------------------------

template< typename T >
static T dotProduct
    ( const TRTK::Coordinate< T >& a
    , const TRTK::Coordinate< T >& b )
{
    T sum = 0;

    for( unsigned int i = 0; i < std::min( a.size(), b.size() ); ++i )
    {
        sum += a[ i ] * b[ i ];
    }

    return sum;
}



// ----------------------------------------------------------------------------------
// normalized
// ----------------------------------------------------------------------------------

template< typename T >
static TRTK::Coordinate< T > normalized( const TRTK::Coordinate< T >& a )
{
    return a / a.norm();
}



// ----------------------------------------------------------------------------------
// SlicePlane
// ----------------------------------------------------------------------------------

SlicePlane::SlicePlane
    ( base::view::SceneProvider& provider
    , base::VisualizationEnvironment& environment )

    : pimpl( new SharedModeDetails( *this, environment, provider ) )
{
    pimpl->initialize();
}


SlicePlane::SlicePlane( const base::model::Scene& model )
    : pimpl( new ExclusiveModeDetails( *this, model ) )
{
    pimpl->initialize();
}


SlicePlane::~SlicePlane()
{
    pimpl->activateContext();

    /*
 // release shaders

    pimpl->shader.reset( nullptr );
    */
}


bool SlicePlane::compute
    ( const base::Vector& normal
    , const double& distance
    , const base::Vector& proportions
    , int windowingLevel
    , unsigned int windowingWidth
    , float border )
{
    CARNA_ASSERT( !base::Math::isEqual( normal.norm(), 0. ) );

    struct too_few_intersection_points{};
    pimpl->activateContext();
    
    glClear( GL_COLOR_BUFFER_BIT );

 // compute slice plane with 3D volume points of intersection

    typedef base::Vector CVector;

    std::vector< CVector > planeCorners( 4 );
    const static double epsilon = 1e-8;

    std::vector< CVector > intersection_points;
    computeIntersectionPoints( normal, distance, intersection_points );

    try
    {
        if( intersection_points.size() < 3 )
        {
            throw too_few_intersection_points();
        }
        else
        {

         // compute center

            CVector point_sum( 0, 0, 0 );
            for( unsigned int i = 0; i < intersection_points.size(); ++i )
            {
                point_sum += intersection_points[ i ];
            }
            const CVector center = point_sum / intersection_points.size();

         // select most far intersection point

            double max_distance = std::numeric_limits< double >::min();
            unsigned int max_distanced_point_index;

            for( unsigned int i = 0; i < intersection_points.size(); ++i )
            {
                const double distance = ( intersection_points[ i ] - center ).norm();
                if( distance > max_distance )
                {
                    max_distanced_point_index = i;
                    max_distance = distance;
                }
            }

            if( base::Math::isEqual( max_distance, 0.0 ) )
            {
                /* Intersection points are too close to each other. Usually this happens
                 * when the plane does not intersect the 3D volume but is tangent to some
                 * corner of the volume, for instance with n = ( 1, 1, 1 ) and d = 0.
                 *
                 * In this case the result is a null set, meaning a all black image, and
                 * the transformation matrix is undefined.
                 */

                throw too_few_intersection_points();
            }

         // build plane

            const CVector a = intersection_points[ max_distanced_point_index ] - center;
            const CVector b = a * Details::WINDOW_SIZE / ( 2 * max_distance );
            planeCorners[ 0 ] = center + b;
            planeCorners[ 2 ] = center - b;

            const CVector c = normalized( VEC_Cross( a, normal ) ) * Details::WINDOW_SIZE / 2;
            planeCorners[ 1 ] = center + c;
            planeCorners[ 3 ] = center - c;
        }
    }
    catch( const too_few_intersection_points& )
    {
        pimpl->valid = false;

        return false;
    }

 // bind framebuffer

    pimpl->fbo->bind();

 // configure GL states

    glPushAttrib( GL_ALL_ATTRIB_BITS );
    
    glDisable( GL_CULL_FACE  );
    glEnable ( GL_TEXTURE_3D );
    glDisable( GL_DEPTH_TEST );

 // setup projection

    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();
    glOrtho( 0, 1, 0, 1, 0.f, 1.f );

    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();

 // setup viewport
    
    GLint viewport[ 4 ];
    glGetIntegerv( GL_VIEWPORT, viewport );

    glViewport( 0, 0, pimpl->fboSizeXY, pimpl->fboSizeXY );

 // bind volume texture

    glBindTexture( GL_TEXTURE_3D, pimpl->getVolumeTextureID() );

 // setup windowing shader

    const float z = -0.5f;

    {
        base::view::ShaderProgram::Binding shaderBinding( pimpl->shader->program() );
        shaderBinding.putUniform1i( "intensityTexture", 0 );
        shaderBinding.putUniform1i( "windowingLevel", windowingLevel );
        shaderBinding.putUniform1i( "windowingWidth", windowingWidth );
        shaderBinding.putUniform1f( "border", border );
        shaderBinding.putUniform1i( "huv0", 5000 );
        shaderBinding.putUniform1i( "huv1", 5000 );
        shaderBinding.putUniform1f( "maskOpacity", 0.8f );

     // paint slice plane

        glColor4f( 1, 1, 1, 1 );
        glBegin( GL_TRIANGLE_FAN );

         // --- A ---

            glTexCoord3f( planeCorners[ 0 ][ 0 ],
                          planeCorners[ 0 ][ 1 ],
                          planeCorners[ 0 ][ 2 ] );

            glVertex3f( 0, 1, z );

         // --- B ---

            glTexCoord3f( planeCorners[ 1 ][ 0 ],
                          planeCorners[ 1 ][ 1 ],
                          planeCorners[ 1 ][ 2 ] );

            glVertex3f( 1, 1, z );

         // --- C ---

            glTexCoord3f( planeCorners[ 2 ][ 0 ],
                          planeCorners[ 2 ][ 1 ],
                          planeCorners[ 2 ][ 2 ] );

            glVertex3f( 1, 0, z );

         // --- D ---

            glTexCoord3f( planeCorners[ 3 ][ 0 ],
                          planeCorners[ 3 ][ 1 ],
                          planeCorners[ 3 ][ 2 ] );

            glVertex3f( 0, 0, z );

        glEnd();
    }

 // restore viewport

    glViewport( viewport[ 0 ], viewport[ 1 ], viewport [ 2 ], viewport[ 3 ] );

 // restore matrices

    glMatrixMode( GL_PROJECTION );
    glPopMatrix();

    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();

 // restore GL states

    glPopAttrib();

 // release framebuffer

    pimpl->fbo->release();

 // compute transformation matrix

    {
        const CVector tangent   = planeCorners[ 2 ] - planeCorners[ 3 ];
        const CVector cotangent = planeCorners[ 0 ] - planeCorners[ 3 ];
        const CVector normal    = VEC_Cross( tangent, cotangent );

        if( dotProduct( tangent, cotangent ) / ( tangent.norm() * cotangent.norm() ) > epsilon )
        {
            pimpl->valid = false;

            return false; // Transformation matrix computation failed.
        }

        pimpl->tangent_to_volume = base::Transformation
            ( tangent.x(), cotangent.x(), normal.x(), planeCorners[ 3 ].x()
            , tangent.y(), cotangent.y(), normal.y(), planeCorners[ 3 ].y()
            , tangent.z(), cotangent.z(), normal.z(), planeCorners[ 3 ].z()
            , 0          , 0            , 0         , 1 );

        std::stringstream ss;
        ss
            << "Tangent Space to Volume Space transformation matrix:\n"
            << pimpl->tangent_to_volume.getTransformationMatrix();

        qDebug() << QString::fromStdString( ss.str() );

        if( ( pimpl->tangent_to_volume * base::Vector( 0, 0, 0 ) - planeCorners[ 3 ] ).norm() > epsilon
         || ( pimpl->tangent_to_volume * base::Vector( 0, 1, 0 ) - planeCorners[ 0 ] ).norm() > epsilon
         || ( pimpl->tangent_to_volume * base::Vector( 1, 0, 0 ) - planeCorners[ 2 ] ).norm() > epsilon
         || ( pimpl->tangent_to_volume * base::Vector( 1, 1, 0 ) - planeCorners[ 1 ] ).norm() > epsilon )
        {
            pimpl->valid = false;
            return false; // Transformation matrix computation failed.
        }
    }

    pimpl->valid = true;
    return true;
}


const base::Transformation& SlicePlane::getTangentToVolume() const
{
    return pimpl->tangent_to_volume;
}


bool SlicePlane::isValid() const
{
    return pimpl->valid;
}


unsigned int SlicePlane::getResultTextureID() const
{
    return pimpl->fbo->texture();
}


void SlicePlane::fetchResult( QImage& out ) const
{
    if( !pimpl->valid )
    {
        QPixmap pixmap( 8, 8 );
        pixmap.fill( Qt::black );
        out = pixmap.toImage();
    }
    else
    {
        out = pimpl->fbo->toImage();
    }
}



}  // namespace Carna :: SlicePlanes

}  // namespace Carna
