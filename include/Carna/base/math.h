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
#include <type_traits>
#include <cmath>
#include <Eigen/Dense>

#ifdef min
#error MIN macro defined, define NOMINMAX first!
#endif

#ifdef max
#error MAX macro defined, define NOMINMAX first!
#endif

/** \cond false
  */
#ifndef NOMINMAX
#define NOMINMAX
#endif
/** \endcond
  */

/** \file   math.h
  * \brief  Defines \ref Carna::base::math namespace and \ref CARNA_FOR_VECTOR3UI.
  *
  * \attention
  * If you include `windows.h` before including this file, make sure you have
  * `NOMINMAX` defined before including the `windows.h` header file.
  */

namespace Carna
{

namespace base
{

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
    T clamp( T val, T my_min, T my_max )
    {
        return std::min( std::max( val, my_min ), my_max );
    }

    /** \brief
      * Computes and returns \f$ x^2 \f$
      */
    template< typename T >
    T sq( T x )
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
    T epsilon()
    {
        return static_cast< T >( 0 );
    }

    /** \brief
      * Defines the maximum difference of two single-precision floating point objects
      * treated as equal.
      */
    template< >
    inline float epsilon< float >()
    {
        return 1e-4f;
    }

    /** \brief
      * Defines the maximum difference of two double-precision floating point objects
      * treated as equal.
      */
    template< >
    inline double epsilon< double >()
    {
        return 1e-6;
    }

    /** \brief
      * Retrieves element types of vectors and scalars. General case assumes a scalar
      * type.
      */
    template< typename T >
    struct element_type_of
    {
        /** \brief
          * Since \a T is assumed to be scalar type, it's element type is also \a T.
          */
        typedef T type;
    };

    /** \brief
      * Retrieves element types of vectors and scalars. This is the specialization
      * for vector and matrix types.
      */
    template< typename VectorElementType, int rows, int cols >
    struct element_type_of< Eigen::Matrix< VectorElementType, rows, cols > >
    {
        /** \brief
          * The vector element type is known implicitly for each vector type.
          */
        typedef VectorElementType type;

    };

    /** \brief
      * Retrieves the squared length of vector and scalar types. General case assumes
      * scalar type.
      */
    template< typename T >
    T length2( const T& x )
    {
        return x * x;
    }

    /** \brief
      * Retrieves the squared length of vector and scalar types. This is the
      * specialization for vector types.
      */
    template< typename VectorElementType, int dimension >
    VectorElementType length2( const Eigen::Matrix< VectorElementType, dimension, 1 >& x )
    {
        return x.squaredNorm();
    }

    /** \brief
      * Tells whether two objects are equal respectively to \ref epsilon.
      */
    template< typename InputType >
    bool isEqual( const InputType& x, const InputType& y )
    {
        typedef typename element_type_of< InputType >::type ScalarType;
        const InputType difference = x - y;
        const ScalarType distance2 = length2( InputType( difference ) );
        const ScalarType _epsilon  = epsilon< ScalarType >();
        return distance2 <= _epsilon;
    }
    
    /** \brief
      * Tells whether two objects are equal. This is the specialization for `bool`
      * types.
      */
    template< >
    inline bool isEqual( const bool& x, const bool& y )
    {
        return x == y;
    }

    typedef Eigen::Matrix< float, 4, 4, Eigen::ColMajor > Matrix4f; ///< Defines \f$\mathbb R^{4 \times 4}\f$ matrix.
    typedef Eigen::Matrix< float, 3, 3, Eigen::ColMajor > Matrix3f; ///< Defines \f$\mathbb R^{3 \times 3}\f$ matrix.
    typedef Eigen::Matrix< float, 4, 1 > Vector4f;                  ///< Defines \f$\mathbb R^{4 \times 1}\f$ vector.
    typedef Eigen::Matrix< float, 3, 1 > Vector3f;                  ///< Defines \f$\mathbb R^{3 \times 1}\f$ vector.
    typedef Eigen::Matrix< float, 2, 1 > Vector2f;                  ///< Defines \f$\mathbb R^{2 \times 1}\f$ vector.
    typedef Eigen::Matrix< signed int, 3, 1 > Vector3i;             ///< Defines \f$\mathbb Z^{3 \times 1}\f$ vector.
    typedef Eigen::Matrix< unsigned int, 3, 1 > Vector3ui;          ///< Defines \f$\mathbb Z^{3 \times 1}_{\geq 0}\f$ vector.
    typedef Eigen::Matrix< unsigned int, 2, 1 > Vector2ui;          ///< Defines \f$\mathbb Z^{2 \times 1}_{\geq 0}\f$ vector.

    /** \brief
      * Returns \f$\mathbb R^{4 \times 4}\f$ identity matrix.
      */
    inline Matrix4f identity4f()
    {
        Matrix4f result;
        result.setIdentity();
        return result;
    }

    /** \brief
      * Returns \f$\mathbb R^{3 \times 3}\f$ identity matrix.
      */
    inline Matrix3f identity3f()
    {
        Matrix3f result;
        result.setIdentity();
        return result;
    }
    
    /** \brief
      * Returns matrix with zeros in all components.
      */
    template< typename MatrixType >
    MatrixType zeros()
    {
        MatrixType result;
        result.setZero();
        return result;
    }

    /** \brief
      * Creates \f$\mathbb R^{3 \times 3}\f$ basis embedded into a
      * \f$\mathbb R^{4 \times 4}\f$ homogenous coordinates matrix.
      *
      * \param x the \a x basis vector
      * \param y the \a y basis vector
      * \param z the \a z basis vector
      * \param t the translation component
      */
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

    /** \overload
      */
    inline Matrix4f basis4f( const Vector3f& x, const Vector3f& y, const Vector3f& z, const Vector3f& t = Vector3f( 0, 0, 0 ) )
    {
        const Vector4f x4( x.x(), x.y(), x.z(), 0 );
        const Vector4f y4( y.x(), y.y(), y.z(), 0 );
        const Vector4f z4( z.x(), z.y(), z.z(), 0 );
        const Vector4f t4( t.x(), t.y(), t.z(), 0 );
        return basis4f( x4, y4, z4, t4 );
    }

    /** \brief
      * Returns matrix that translates homogeneous coordinates.
      */
    inline Matrix4f translation4f( float x, float y, float z )
    {
        Matrix4f result;
        result.setIdentity();
        result( 0, 3 ) = x;
        result( 1, 3 ) = y;
        result( 2, 3 ) = z;
        return result;
    }

    /** \overload
      */
    template< typename Vector >
    Matrix4f translation4f( const Vector& v )
    {
        return translation4f( v.x(), v.y(), v.z() );
    }

    /** \brief
      * Creates scaling matrix for homogeneous coordinates.
      */
    inline Matrix4f scaling4f( float x, float y, float z )
    {
        Matrix4f result;
        result.setIdentity();
        result( 0, 0 ) = x;
        result( 1, 1 ) = y;
        result( 2, 2 ) = z;
        return result;
    }

    /** \overload
      */
    template< typename VectorElementType >
    inline Matrix4f scaling4f( const Eigen::Matrix< VectorElementType, 3, 1 >& v )
    {
        return scaling4f( v.x(), v.y(), v.z() );
    }
    
    /** \overload
      */
    inline Matrix4f scaling4f( float uniformScaleFactor )
    {
        return scaling4f( uniformScaleFactor, uniformScaleFactor, uniformScaleFactor );
    }

    /** \brief
      * Creates rotation matrix for homogeneous coordinates. The rotation is
      * performed around the axis that is specified by the vector with the components
      * \a x, \a y and \a z.
      */
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

    /** \overload
      */
    template< typename Vector >
    Matrix4f rotation4f( const Vector& v, float radians )
    {
        return rotation4f( v.x(), v.y(), v.z(), radians );
    }

    /** \brief
      * Creates \ref rotation4f "rotation matrix for homogeneous coordinates", but
      * returns only the upper left \f$3 \times 3\f$ sub-matrix.
      */
    inline Matrix3f rotation3f( float x, float y, float z, float radians )
    {
        return rotation4f( x, y, z, radians ).block< 3, 3 >( 0, 0 );
    }

    /** \brief
      * Constructs \f$\mathbb R^3\f$ vector that is orthogonal to \a in. The result
      * is undefined if the \ref length2 "squared length" of \a in
      * \ref isEqual "equals" zero.
      */
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

        /** Obviously the vector was too short. Check this. If it was not, than
          * something went wrong.
          */
        CARNA_ASSERT( isEqual< float >( in.dot( out ), 0 ) );
        return out;
    }

    /** \brief
      * Creates 4-dimensional vector from 3-dimensional (or higher) with same
      * component type, and a scalar that is appended as the fourth component.
      */
    template< typename VectorElementType, int rows, typename WType >
    Eigen::Matrix< VectorElementType, 4, 1 > vector4( const Eigen::Matrix< VectorElementType, rows, 1 >& v, WType w )
    {
        static_assert( rows >= 3, "math::vector4 requires input vector with 3 rows or more." );
        return Eigen::Matrix< VectorElementType, 4, 1 >( v.x(), v.y(), v.z(), static_cast< VectorElementType >( w ) );
    }

    /** \brief
      * Creates 3-dimensional vector from 4-dimensional (or higher) with same
      * component type by dropping the fourth component.
      */
    template< typename VectorElementType, int rows >
    Eigen::Matrix< VectorElementType, 3, 1 > vector3( const Eigen::Matrix< VectorElementType, rows, 1 >& v )
    {
        static_assert( rows >= 3, "math::vector3 requires input vector with 3 rows or more." );
        return Eigen::Matrix< VectorElementType, 3, 1 >( v.x(), v.y(), v.z() );
    }

    /** \brief
      * Creates matrix that transforms from the tangent space of a plane with
      * particular \a normal vector and origin \a distance to the space that
      * \a normal and \a distance are given within.
      */
    inline Matrix4f plane4f( const Vector3f& normal, float distance )
    {
        CARNA_ASSERT( isEqual< float >( normal.norm(), 1 ) );
        const Vector3f tangent  ( orthogonal3f(  normal ).normalized() );
        const Vector3f bitangent( normal.cross( tangent ).normalized() );
        const Vector3f translation( normal * distance );
        return basis4f
            ( vector4(   bitangent, 0 )
            , vector4(     tangent, 0 )
            , vector4(      normal, 0 )
            , vector4( translation, 1 ) );
    }

    /** \overload
      */
    inline Matrix4f plane4f( const Vector3f& normal, const Vector3f& support )
    {
        CARNA_ASSERT( isEqual< float >( normal.norm(), 1 ) );
        const float distance = normal.dot( support );
        return plane4f( normal, distance );
    }

    /** \brief
      * Returns the squared length of the translation component of the homogeneous
      * coordinates transformation matrix \a m.
      */
    inline float translationDistance2( const Matrix4f& m )
    {
        return m( 0, 3 ) * m( 0, 3 ) + m( 1, 3 ) * m( 1, 3 ) + m( 2, 3 ) * m( 2, 3 );
    }

    /** \brief
      * Returns \f$\max_{i,j} \left|m_{i,j}\right|\f$ where $m$ is \a m.
      */
    template< typename Matrix >
    typename Matrix::Scalar maxAbsElement( const Matrix& m )
    {
        const std::size_t length = m.rows() * m.cols();
        typename Matrix::Scalar maxAbs = 0;
        for( std::size_t i = 0; i < length; ++i )
        {
            maxAbs = std::max( maxAbs, std::abs( m.data()[ i ] ) );
        }
        return maxAbs;
    }

    /** \brief
      * Returns the projection matrix that is described by the specified frustum.
      *
      * The frustum consists of six clipping planes. The near clipping plane is also
      * the plane that the 3D scene is projected upon. The planes are specified as
      * follows:
      *
      *   - \a left and \a right specify the horizontal coordinates where the left
      *     and right frustum planes intersect the near clipping plane.
      *   - \a top and \a bottom specify the vertical coordinates where the top and
      *     bottom frustum planes intersect the near clipping plane.
      *   - \a zNear and \a zFar specify the distance of the near and the far frustum
      *     planes to the eye.
      */
    inline Matrix4f frustum4f( float left, float right, float bottom, float top, float zNear, float zFar )
    {
        Matrix4f result;
        result.setZero();

        result( 0, 0 ) = +2 * zNear / ( right - left );
        result( 1, 1 ) = +2 * zNear / ( top - bottom );
        result( 0, 2 ) =  ( right + left ) / ( right - left );
        result( 1, 2 ) =  ( top + bottom ) / ( top - bottom );
        result( 2, 2 ) = -( zFar + zNear ) / ( zFar - zNear );
        result( 3, 2 ) = -1;
        result( 2, 3 ) = -2 * zFar * zNear / ( zFar - zNear );

        return result;
    }

    /** \overload
      *
      * \param fovRadiansHorizontal
      *     Specifies the angle between the left and the right frustum planes.
      *
      * \param heightOverWidth
      *     Specifies the ratio \f$\frac{\text{height}}{\text{width}}\f$ for the
      *     projection plane.
      *
      * \param zNear
      *     Specifies the distance of the projection plane to the eye.
      *
      * \param zFar
      *     Specifies the distance of the far clipping plane to the eye.
      */
    inline Matrix4f frustum4f( float fovRadiansHorizontal, float heightOverWidth, float zNear, float zFar )
    {
        const float halfProjPlaneWidth  = zNear * std::tan( fovRadiansHorizontal / 2 );
        const float halfProjPlaneHeight = halfProjPlaneWidth * heightOverWidth;
        return frustum4f( -halfProjPlaneWidth, +halfProjPlaneWidth, -halfProjPlaneHeight, +halfProjPlaneHeight, zNear, zFar );
    }
    
    /** \brief
      * Returns the projection matrix that is described by the specified box.
      */
    inline Matrix4f ortho4f( float left, float right, float bottom, float top, float zNear, float zFar )
    {
        Matrix4f result;
        result.setZero();

        result( 0, 0 ) =  2 / ( right - left );
        result( 1, 1 ) =  2 / ( top - bottom );
        result( 2, 2 ) = -2 / ( zFar - zNear );
        result( 0, 3 ) = -( right + left ) / ( right - left );
        result( 1, 3 ) = -( top + bottom ) / ( bottom - top );
        result( 2, 3 ) = -( zFar + zNear ) / ( zFar - zNear );
        result( 3, 3 ) = +1;

        return result;
    }

    /** \brief
      * Rounds \a x to the closest \f$x' \in \mathbb Z_{\geq 0}\f$. Either the
      * data type of \f$x\f$ must be unsigned or \f$x \geq 0\f$.
      */
    template< typename ScalarType >
    unsigned int round_ui( ScalarType x )
    {
        CARNA_ASSERT( !std::numeric_limits< ScalarType >::is_signed || x >= 0 );
        return static_cast< unsigned int >( x + static_cast< ScalarType >( 0.5 ) );
    }

    /** \brief
      * Rounds matrix \a m to such \f$m'\f$ that every element in \f$m'\f$ is
      * close-most to its corresponding element in \f$m\f$. Either the element data
      * type of \f$m\f$ must be unsigned or \f$m_{i,j} \geq 0 \forall i,j\f$.
      */
    template< typename MatrixElementType, int cols, int rows >
    Eigen::Matrix< unsigned int, cols, rows > round_ui( const Eigen::Matrix< MatrixElementType, cols, rows >& m )
    {
        Eigen::Matrix< unsigned int, cols, rows > result;
        for( int i = 0; i < cols; ++i )
        for( int j = 0; j < rows; ++j )
        {
            result( i, j ) = round_ui( m( i, j ) );
        }
        return result;
    }

    /** \brief
      * Returns \f$x\f$ if \f$x\f$ is even and \f$x + s\f$ if \f$x\f$ is odd, where
      * \f$s \in \left\{-1, +1\right\}\f$. The data type of \f$x\f$ must be integral.
      */
    template< typename ScalarType >
    ScalarType makeEven( ScalarType x, int s )
    {
        CARNA_ASSERT( s == +1 || s == -1 );
        static_assert( std::is_integral< ScalarType >::value, "Only integral data types allowed." );
        return x + s * ( x % 2 );
    }

    /** \brief
      * Returns matrix \f$m'\f$ of the same size like \f$m\f$ where
      * \f$m'_{i,j} = m_{i,j}\f$ if \f$m_{i,j}\f$ is even and
      * \f$m'_{i,j} = m_{i,j} + s\f$ if \f$m_{i,j}\f$ is odd, where
      * \f$s \in \left\{-1, +1\right\}\f$. The element data type of
      * \f$m\f$ must be integral.
      */
    template< typename MatrixElementType, int cols, int rows >
    Eigen::Matrix< MatrixElementType, cols, rows > makeEven( const Eigen::Matrix< MatrixElementType, cols, rows >& m, int s )
    {
        Eigen::Matrix< unsigned int, cols, rows > result;
        for( int i = 0; i < cols; ++i )
        for( int j = 0; j < rows; ++j )
        {
            result( i, j ) = makeEven( m( i, j ), s );
        }
        return result;
    }
    
    /** \brief
      * Holds mean and variance of an characteristic.
      */
    template< typename T >
    struct Statistics
    {
        T mean;     ///< Holds the mean.
        T variance; ///< Holds the variance.
        
        /** \brief
          * Initializes with \a mean and \a variance.
          */
        Statistics( T mean, T variance ) : mean( mean ), variance( variance )
        {
        }
        
        /** \brief
          * Computes statistics from \a size samples queried from \a values.
          */
        Statistics( std::size_t size, const std::function< T( std::size_t ) > values )
        {
            if( size == 0 )
            {
                mean = variance = 0;
            }
            else
            {
                /* Compute mean.
                 */
                T sum = 0;
                for( std::size_t idx = 0; idx < size; ++idx )
                {
                    sum += values( idx );
                }
                mean = sum / size;
                
                /* Compute variance.
                 */
                sum = 0;
                for( std::size_t idx = 0; idx < size; ++idx )
                {
                    sum += sq( mean - values( idx ) );
                }
                variance = sum / ( size - 1 );
            }
        }
        
        /** \brief
          * Copies from \a other.
          */
        Statistics< T >& operator=( const Statistics< T >& other )
        {
            mean = other.mean;
            variance = other.variance;
            return *this;
        }
        
        /** \brief
          * Computes the standard deviation.
          */
        T standardDeviation() const
        {
            return std::sqrt( variance );
        }
    };
    
    /** \brief
      * Interpolates between \a and \b by \a t \f$\in\left[0, 1\right]\f$ linearly.
      */
    template< typename ResultType, typename SupportType >
    ResultType mix( const SupportType& a, const SupportType& b, float t )
    {
        return a * ( 1 - t ) + b * t;
    }



/** \brief
  * Loops \a vecName over all
  * \f$\vec x \in \mathbb Z^3_{\geq 0} : x_i < n_i \wedge x_i \geq s_i \forall i\f$
  * where \f$\vec n\f$ is \a vecLimit and \f$\vec s\f$ is \a vecStart.
  */
#define CARNA_FOR_VECTOR3UI_EX( vecName, vecLimit, vecStart ) \
    Carna::base::math::Vector3ui vecName; \
    for( vecName.z() = vecStart.x(); vecName.z() < vecLimit.z(); ++vecName.z() ) \
    for( vecName.y() = vecStart.y(); vecName.y() < vecLimit.y(); ++vecName.y() ) \
    for( vecName.x() = vecStart.z(); vecName.x() < vecLimit.x(); ++vecName.x() )



/** \brief
  * Loops \a vecName over all \f$\vec x \in \mathbb Z^3_{\geq 0} : x_i < n_i \forall i\f$
  * where \f$\vec n\f$ is \a vecLimit.
  *
  * Example: \snippet UnitTests/mathTest.cpp example_CARNA_FOR_VECTOR3UI
  */
#define CARNA_FOR_VECTOR3UI( vecName, vecLimit ) \
    CARNA_FOR_VECTOR3UI_EX( vecName, vecLimit, Carna::base::math::Vector3ui( 0, 0, 0 ) )



}  // namespace Carna :: base :: math

}  // namespace Carna :: base

}  // namespace Carna



#endif // MATH_H_6014714286
