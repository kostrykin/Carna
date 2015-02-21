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
Matrix4f translation4f( float x, float y, float z )
{
	Matrix4f result;
	result.setIdentity();
	result( 0, 3 ) = x;
	result( 1, 3 ) = y;
	result( 2, 3 ) = z;
	return result;
}


inline
Matrix4f scaling( float x, float y, float z )
{
	Matrix4f result;
	result.setIdentity();
	result( 0, 0 ) = x;
	result( 1, 1 ) = y;
	result( 2, 2 ) = z;
	return result;
}


inline
Matrix4f scaling( float uniform_scale_factor )
{
	return scaling( uniform_scale_factor, uniform_scale_factor, uniform_scale_factor );
}


inline
Matrix4f rotation( float x, float y, float z, float radians )
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



}  // namespace base

}  // namespace Carna



#endif // MATRIX4F_H_6014714286
