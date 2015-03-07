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

#ifndef MATH_H_6014714286
#define MATH_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/CarnaException.h>
#include <algorithm>
#include <cmath>
#include <Eigen/Dense>

#ifdef min
#error MIN macro defined, define NOMINMAX first!
#endif

#ifdef max
#error MAX macro defined, define NOMINMAX first!
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

/** \file   math.h
  * \brief  Defines \ref Carna::base::math.
  *
  * \attention
  * If you include \c windows.h before including this file, make sure to have \c NOMINMAX
  * defined before including the \c windows.h header file.
  */

namespace Carna
{

namespace base
{

/** \brief  Provides set of math-related classes and functions.
  *
  * \date   26.1.14 - 26.2.15
  * \author Leonid Kostrykin
  */
namespace math
{

    /** \brief
      * Returns \f$ \min( \max( \f$ \a val \f$, \f$ \a my_min \f$ ), \f$ \a my_max \f$ ) \f$
      *
      * Returns \f$ \left\{ \begin{array}{ll}
      *     \mathrm{max} & \mbox{if $\mathrm{val} > \mathrm{max}$} \\
      *     \mathrm{min} & \mbox{if $\mathrm{val} < \mathrm{min}$} \\
      *     \mathrm{val} & \mbox{else}.\end{array} \right. \f$
      */
    template< typename T >
    inline T clamp( T val, T my_min, T my_max )
    {
        return std::min( std::max( val, my_min ), my_max );
    }

    /** \brief
      * Computes and returns \f$ x^2 \f$
      */
    template< typename T >
    inline T sq( T x )
    {
        return x * x;
    }
    
    /** \brief
      * Converts degrees to radians.
      */
    inline float deg2rad( float deg )
    {
        return deg * 3.1415926f / 180.f;
    }

    /** \brief
      * Converts radians to degrees.
      */
    inline float rad2deg( float rad )
    {
        return 180.f * rad / 3.1415926f;
    }

    /** \brief
      * Defines the maximum difference of two objects treated as equal.
      */
    template< typename T >
    inline T epsilon()
    {
        return static_cast< T >( 0 );
    }

    /** \brief
      * Defines the maximum difference of two single-precision floating point objects treated as equal.
      */
    template< >
    inline float epsilon< float >()
    {
        return 1e-4f;
    }

    /** \brief
      * Defines the maximum difference of two double-precision floating point objects treated as equal.
      */
    template< >
    inline double epsilon< double >()
    {
        return 1e-6;
    }

    /** \brief
      * Retrieves element types of vectors and scalars. General case assumes a scalar type.
      */
    template< typename T >
    struct element_type_of
    {

        /** \brief
          * Since \a T is assumed to be scalar type, it's element type is also \a T.
          */
        typedef typename T type;

    };

    /** \brief
      * Retrieves element types of vectors and scalars. Specialization for \ref Vector class.
      */
    template< typename VectorElementType, unsigned int dimension >
    struct element_type_of< Eigen::Matrix< VectorElementType, dimension, 1 > >
    {
    
        /** \brief
          * The vector element type is known implicitly for each vector type.
          */
        typedef typename VectorElementType type;

    };

#pragma warning( push )
#pragma warning( disable:4146 )

    /** \brief
      * Retrieves the squared length of vector and scalar types. General case assumes scalar type.
      */
    template< typename T >
    inline T length2( const T& x )
    {
        return x * x;
    }

#pragma warning( pop )

    /** \brief
      * Retrieves the length of vector and scalar types. Specialization for \ref Vector class.
      */
    template< typename VectorElementType, unsigned int dimension >
    inline VectorElementType length2( const Eigen::Matrix< VectorElementType, dimension, 1 >& x )
    {
        return x.squaredNorm();
    }

    /** \brief
      * Tells whether two objects are equal respectively to \ref epsilon.
      */
    template< typename InputType >
    inline bool isEqual( const InputType& x, const InputType& y )
    {
        typedef element_type_of< InputType >::type ScalarType;
        const InputType difference = x - y;
        const ScalarType distance2 = length2( InputType( difference ) );
        const ScalarType _epsilon  = epsilon< ScalarType >();
        return distance2 <= _epsilon;
    }

    template< >
    inline bool isEqual( const bool& x, const bool& y )
    {
        return x == y;
    }

    typedef Eigen::Matrix< float, 4, 4, Eigen::ColMajor > Matrix4f;
    typedef Eigen::Matrix< float, 4, 1 > Vector4f;
    typedef Eigen::Matrix< float, 3, 1 > Vector3f;
    typedef Eigen::Matrix< float, 2, 1 > Vector2f;
    typedef Eigen::Matrix< unsigned int, 3, 1 > Vector3ui;
    typedef Eigen::Matrix< unsigned int, 2, 1 > Vector2ui;

    inline Matrix4f identity4f()
    {
        Matrix4f result;
        result.setIdentity();
        return result;
    }

    inline Matrix4f zeros4f()
    {
        Matrix4f result;
        result.setZero();
        return result;
    }

    inline Matrix4f basis4f( const Vector4f& x, const Vector4f& y, const Vector4f& z, const Vector4f& t = Vector4f( 0, 0, 0, 0 ) )
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

    inline Matrix4f basis4f( const Vector3f& x, const Vector3f& y, const Vector3f& z, const Vector3f& t = Vector3f( 0, 0, 0 ) )
    {
        const Vector4f x4( x.x(), x.y(), x.z(), 0 );
        const Vector4f y4( y.x(), y.y(), y.z(), 0 );
        const Vector4f z4( z.x(), z.y(), z.z(), 0 );
        const Vector4f t4( t.x(), t.y(), t.z(), 0 );
        return basis4f( x4, y4, z4, t4 );
    }

    inline Matrix4f translation4f( float x, float y, float z )
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

    inline Matrix4f scaling4f( float x, float y, float z )
    {
        Matrix4f result;
        result.setIdentity();
        result( 0, 0 ) = x;
        result( 1, 1 ) = y;
        result( 2, 2 ) = z;
        return result;
    }

    template< typename VectorElementType >
    inline Matrix4f scaling4f( const Eigen::Matrix< VectorElementType, 3, 1 >& v )
    {
        return scaling4f( v.x(), v.y(), v.z() );
    }

    inline Matrix4f scaling4f( float uniform_scale_factor )
    {
        return scaling4f( uniform_scale_factor, uniform_scale_factor, uniform_scale_factor );
    }

    inline Matrix4f rotation4f( float x, float y, float z, float radians )
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

    template< typename Vector >
    Matrix4f rotation4f( const Vector& v, float radians )
    {
        return rotation4f( v.x(), v.y(), v.z(), radians );
    }

    inline Vector3f orthogonal3f( const Vector3f& in )
    {
        Vector3f out;
        if( std::abs( in.x() - in.y() ) > 1e-4f )
        {
            out.x() = in.y();
            out.y() = in.x();
            out.z() = 0;

            if( std::abs( out.x() ) > std::abs( out.y() ) )
            {
                out.x() = -out.x();
            }
            else
            {
                out.y() = -out.y();
            }
        }
        else
        if( std::abs( in.x() - in.z() ) > 1e-4f )
        {
            out.x() = in.z();
            out.y() = 0;
            out.z() = in.x();

            if( std::abs( out.x() ) > std::abs( out.z() ) )
            {
                out.x() = -out.x();
            }
            else
            {
                out.z() = -out.z();
            }
        }
        else
        if( std::abs( in.y() - in.z() ) > 1e-4f )
        {
            out.x() = 0;
            out.y() = in.z();
            out.z() = in.x();

            if( std::abs( out.y() ) > std::abs( out.z() ) )
            {
                out.y() = -out.y();
            }
            else
            {
                out.z() = -out.z();
            }
        }
        else // all components are equal
        {
            out = Vector3f( -in.x(), in.y(), 0 );
        }
        CARNA_ASSERT( std::abs( in.dot( out ) ) < 1e-4f );
        return out;
    }

    template< typename Vector3 >
    Vector4f vector4f( const Vector3& v, float w )
    {
        return Vector4f( v.x(), v.y(), v.z(), w );
    }

    template< typename Vector4 >
    Vector3f vector3f( const Vector4& v )
    {
        return Vector3f( v.x(), v.y(), v.z() );
    }

    inline Matrix4f plane4f( const Vector3f& normal, float distance )
    {
        CARNA_ASSERT( isEqual< float >( normal.norm(), 1 ) );
        const Vector3f tangent  ( orthogonal3f(  normal ).normalized() );
        const Vector3f bitangent( normal.cross( tangent ).normalized() );
        return basis4f
            ( vector4f        ( bitangent, 0 )
            , vector4f          ( tangent, 0 )
            , vector4f           ( normal, 0 )
            , vector4f( normal * distance, 1 ) );
    }

    inline Matrix4f plane4f( const Vector3f& normal, const Vector3f& support )
    {
        CARNA_ASSERT( isEqual< float >( normal.norm(), 1 ) );
        const float distance = normal.dot( support );
        return plane4f( normal, distance );
    }

    inline float translationDistanceSq( const Matrix4f& m )
    {
        return m( 1, 3 ) * m( 1, 3 ) + m( 2, 3 ) * m( 2, 3 ) + m( 3, 3 ) * m( 3, 3 );
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

    Matrix4f frustum4f( float left, float right, float bottom, float top, float zNear, float zFar )
    {
        Matrix4f result;
        result.setZero();

        result( 0, 0 ) = 2 * zNear / ( right - left );
        result( 1, 1 ) = 2 * zNear / ( top - bottom );
        result( 0, 2 ) = ( right + left ) / ( right - left );
        result( 1, 2 ) = ( top + bottom ) / ( top - bottom );
        result( 2, 2 ) = -( zFar + zNear ) / ( zFar - zNear );
        result( 3, 2 ) = -1;
        result( 2, 3 ) = -2 * zFar * zNear / ( zFar - zNear );

        return result;
    }

    Matrix4f frustum4f( float fovRadiansHorizontal, float heightOverWidth, float zNear, float zFar )
    {
        const float halfProjPlaneWidth  = zNear * std::tan( fovRadiansHorizontal );
        const float halfProjPlaneHeight = halfProjPlaneWidth * heightOverWidth;
        return frustum4f( -halfProjPlaneWidth, +halfProjPlaneWidth, -halfProjPlaneHeight, +halfProjPlaneHeight, zNear, zFar );
    }

    template< typename ScalarType >
    unsigned int round_ui( ScalarType x )
    {
        CARNA_ASSERT( !std::numeric_limits< ScalarType >::is_signed || x > 0 );
        return static_cast< unsigned int >( x + static_cast< ScalarType >( 0.5 ) );
    }

    template< typename MatrixElementType, unsigned int cols, unsigned int rows >
    Eigen::Matrix< unsigned int, cols, rows > round_ui( const Eigen::Matrix< MatrixElementType, cols, rows >& m )
    {
        Eigen::Matrix< unsigned int, cols, rows > result;
        for( unsigned int i = 0; i < cols; ++i )
        for( unsigned int j = 0; j < rows; ++j )
        {
            result( i, j ) = round_ui( m( i, j ) );
        }
        return result;
    }



}  // namespace Carna :: base :: math

}  // namespace Carna :: base

}  // namespace Carna

#endif // MATH_H_6014714286
