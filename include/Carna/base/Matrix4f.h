/*
 *  Copyright (C) 2010 - 2015 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#ifndef MATRIX4F_H_6014714286
#define MATRIX4F_H_6014714286

#include <cmath>
#include <Eigen/Dense>


namespace Carna
{

namespace base
{



typedef Eigen::Matrix< float, 4, 4, Eigen::ColMajor > Matrix4f;
typedef Eigen::Matrix< float, 4, 1 > Vector4f;


inline
Matrix4f identity4f()
{
    Matrix4f result;
    result.setIdentity();
    return result;
}


inline
Matrix4f basis4f( const Vector4f& x, const Vector4f& y, const Vector4f& z, const Vector4f& t = Vector4f( 0, 0, 0, 0 ) )
{
    Matrix4f result;
    result.col( 0 ) = x;
    result.col( 1 ) = y;
    result.col( 2 ) = z;
    result.col( 3 ) = t;
    for( unsigned int i = 0; i < 3; ++i )
    {
        result( 3, i ) = 0;
    }
    result( 3, 3 ) = 1;
    return result;
}


inline
Matrix4f translation4f( float x, float y, float z )
{
    Matrix4f result;
    result.setIdentity();
    result( 0, 3 ) = x;
    result( 1, 3 ) = y;
    result( 2, 3 ) = z;
    return result;
}


template< typename Vector >
Matrix4f translation4f( const Vector& v )
{
    return translation4f( v.x(), v.y(), v.z() );
}


inline
Matrix4f scaling4f( float x, float y, float z )
{
    Matrix4f result;
    result.setIdentity();
    result( 0, 0 ) = x;
    result( 1, 1 ) = y;
    result( 2, 2 ) = z;
    return result;
}


inline
Matrix4f scaling4f( float uniform_scale_factor )
{
    return scaling4f( uniform_scale_factor, uniform_scale_factor, uniform_scale_factor );
}


inline
Matrix4f rotation4f( float x, float y, float z, float radians )
{
    const float c = std::cos( radians );
    const float s = std::sin( radians );

    Matrix4f result;
    result.setIdentity();

    result( 0, 0 ) = x * x * ( 1 - c ) + c;
    result( 1, 0 ) = y * x * ( 1 - c ) + z * s;
    result( 2, 0 ) = x * z * ( 1 - c ) - y * s;

    result( 0, 1 ) = x * y * ( 1 - c ) - z * s;
    result( 1, 1 ) = y * y * ( 1 - c ) + c;
    result( 2, 1 ) = y * z * ( 1 - c ) + x * s;

    result( 0, 2 ) = x * z * ( 1 - c ) + y * s;
    result( 1, 2 ) = y * z * ( 1 - c ) - x * s;
    result( 2, 2 ) = z * z * ( 1 - c ) + c;

    return result;
}


inline
float translationDistanceSq( const Matrix4f& m )
{
    return m( 1, 3 ) * m( 1, 3 ) + m( 2, 3 ) * m( 2, 3 ) + m( 3, 3 ) * m( 3, 3 );
}


template< typename Vector >
Vector& normalized( Vector& v )
{
    v.normalize();
    return v;
}


template< typename Matrix >
float maxAbsElement( const Matrix& m )
{
    const std::size_t length = m.rows() * m.cols();
    float maxAbs = 0;
    for( std::size_t i = 0; i < length; ++i )
    {
        maxAbs = std::max( maxAbs, abs( m.data()[ i ] ) );
    }
    return maxAbs;
}



}  // namespace base

}  // namespace Carna



#endif // MATRIX4F_H_6014714286
