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

#ifndef MATH_H_6014714286
#define MATH_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/Transformation.h>
#include <algorithm>

#ifdef min
#error MIN macro defined, define NOMINMAX first!
#endif

#ifdef max
#error MAX macro defined, define NOMINMAX first!
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

/** \file   Math.h
  * \brief  Defines \ref Carna::base::Math.
  *
  * \attention
  * If you include \c windows.h before including this file, make sure to have \c NOMINMAX
  * defined before including the \c windows.h header file.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Math
// ----------------------------------------------------------------------------------
    
/** \brief  Provides set of math-related helper functions.
  *
  * \since  \ref v_2_4
  * \date   26.1.14
  * \author Leonid Kostrykin
  */
class Math
{

    Math()
    {
    }

public:

    /** \brief  Returns \f$ \min( \max( \f$ \a val \f$, \f$ \a my_min \f$ ), \f$ \a my_max \f$ ) \f$
      *
      * \ingroup MathTools
      *
      * Returns \f$ \left\{ \begin{array}{ll}
      *     \mathrm{max} & \mbox{if $\mathrm{val} > \mathrm{max}$} \\
      *     \mathrm{min} & \mbox{if $\mathrm{val} < \mathrm{min}$} \\
      *     \mathrm{val} & \mbox{else}.\end{array} \right. \f$
      */
    template< typename T >
    inline static T clamp( T val, T my_min, T my_max )
    {
        return std::min( std::max( val, my_min ), my_max );
    }

    /** \brief  Computes and returns \f$ x^2 \f$
      *
      * \ingroup MathTools
      */
    template<typename T>
    inline static T sq( T x )
    {
        return x * x;
    }
    
    /** \brief      Converts degrees to radians.
      *
      * \ingroup MathTools
      */
    inline static float deg2rad( float deg )
    {
        return deg * 3.1415926f / 180.f;
    }

    /** \brief      Converts radians to degrees.
      *
      * \ingroup MathTools
      */
    inline static float rad2deg( float rad )
    {
        return 180.f * rad / 3.1415926f;
    }

    /** \brief  Defines the maximum difference of two objects treated as equal.
      *
      * \ingroup MathTools
      */
    template< typename T >
    inline static T epsilon()
    {
        return static_cast< T >( 0 );
    }

    /** \brief  Defines the maximum difference of two single-precision floating point objects treated as equal.
      *
      * \ingroup MathTools
      */
    template< >
    inline static float epsilon< float >()
    {
        return 1e-4f;
    }

    /** \brief  Defines the maximum difference of two double-precision floating point objects treated as equal.
      *
      * \ingroup MathTools
      */
    template< >
    inline static double epsilon< double >()
    {
        return 1e-6;
    }

    /** \brief  Retrieves element types of vectors and scalars. General case assumes a scalar type.
      *
      * \ingroup MathTools
      */
    template< typename T >
    struct element_type_of
    {

        /** \brief  Since \a T is assumed to be scalar type, it's element type is also \a T.
          */
        typedef typename T type;

    };

    /** \brief  Retrieves element types of vectors and scalars. Specialization for \ref Vector class.
      *
      * \ingroup MathTools
      */
    template< >
    struct element_type_of< Vector >
    {
    
        /** \brief  The element type of \ref Vector is obtained via \c Vector::value_type.
          */
        typedef Vector::value_type type;

    };

#pragma warning( push )
#pragma warning( disable:4146 )

    /** \brief  Retrieves the length of vector and scalar types. General case assumes scalar type.
      *
      * \ingroup MathTools
      */
    template< typename LengthType, typename InputType >
    inline static LengthType length( const InputType& x )
    {
        return x >= 0 ? x : -x;
    }

#pragma warning( pop )

    /** \brief  Retrieves the length of vector and scalar types. Specialization for \ref Vector class.
      *
      * \ingroup MathTools
      */
    template< >
    inline static double length< double, Vector >( const Vector& x )
    {
        return x.norm();
    }

    /** \brief  Tells whether two objects are equal respectively to \ref epsilon.
      *
      * \ingroup MathTools
      */
    template< typename InputType >
    inline static bool isEqual( const InputType& x, const InputType& y )
    {
        typedef element_type_of< InputType >::type ScalarType;
        const InputType difference = x - y;
        const ScalarType distance = length< ScalarType, InputType >( difference );
        const ScalarType _epsilon = epsilon< ScalarType >();
        return distance <= _epsilon;
    }

}; // Math



}  // namespace Carna :: base

}  // namespace Carna

#endif // MATH_H_6014714286
