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

#ifndef VECTOR3_H_6014714286
#define VECTOR3_H_6014714286

/** \file   Vector3.h
  * \brief  Defines Carna::base::Vector3, Carna::base::Vector3ui and operators.
  */

#include <Carna/base/Math.h>
#include <cmath>
#include <ostream>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Vector3
// ----------------------------------------------------------------------------------

/** \brief  Simple representation of 3D vectors
  *
  * \ingroup MathTools
  * \author Leonid Kostrykin
  * \date   2010 - 2012
  */
template< typename T >
class Vector3
{

public:

    /** \brief  Thrown when \f$ \vec a - \vec b \f$ with \f$ b > a \f$ is performed.
      */
    struct subtrahend_larger_than_minued {};

    /** \brief  Holds the underlying element data type.
      */
    typedef T ElementType;

    /** \brief  Instantiates.
      */
    explicit Vector3( const T& x = 0, const T& y = 0, const T& z = 0 )
        : x( x )
        , y( y )
        , z( z )
    {
    }


    /** \brief  Copy constructor
      */
    template<typename U>
    Vector3( const Vector3<U>& v )
        : x( v.x )
        , y( v.y )
        , z( v.z )
    {
    }


    union
    {
        T elements[ 3 ];
        struct
        {
            T x;
            T y;
            T z;
        };
    };


    /** \brief  Accesses the elements by index.
      */
    T& operator[]( unsigned int i )
    {
        return elements[ i ];
    }

    /** \brief  Accesses the elements by index.
      */
    const T& operator[]( unsigned int i ) const
    {
        return elements[ i ];
    }

}; // Vector3


/** \brief  Defines a vector in \f$ \mathbb{N}_0^3 \f$
  *
  * \relates Vector3
  */
typedef Vector3< unsigned int > Vector3ui;



}  // namespace Carna :: base

}  // namespace Carna



// ----------------------------------------------------------------------------------
// Vector3 Operators
// ----------------------------------------------------------------------------------

/** \brief  Defines addition for \ref Carna::base::Vector3.
  *
  * \relates Carna::base::Vector3
  */
template< typename T >
inline Carna::base::Vector3< T > operator+( const Carna::base::Vector3< T >& v1, const Carna::base::Vector3< T >& v2 )
{
    return Carna::base::Vector3< T >( v1.x + v2.x
                                    , v1.y + v2.y
                                    , v1.z + v2.z );
}


/** \brief  Defines subtraction for \ref Carna::base::Vector3.
  *
  * \relates Carna::base::Vector3
  */
template< typename T >
inline Carna::base::Vector3< T > operator-( const Carna::base::Vector3< T >& v1, const Carna::base::Vector3< T >& v2 )
{
    return Carna::base::Vector3< T >( v1.x - v2.x
                                    , v1.y - v2.y
                                    , v1.z - v2.z );
}


/** \brief  Specializes subtraction for \ref Vector3ui.
  *
  * \relates Carna::base::Vector3
  *
  * \throws Carna::base::Vector3::subtrahend_larger_than_minued
  *         when when subtrahend has at least one component larger than the corresponding minuends component.
  */
template< >
inline Carna::base::Vector3ui operator-( const Carna::base::Vector3< unsigned int >& v1, const Carna::base::Vector3< unsigned int >& v2 )
{
    if( v2.x > v1.x
     || v2.y > v1.y
     || v2.z > v1.z )
    {
        throw Carna::base::Vector3ui::subtrahend_larger_than_minued();
    }
    return Carna::base::Vector3ui( v1.x - v2.x
                                 , v1.y - v2.y
                                 , v1.z - v2.z );
}


/** \brief  Defines equality for \ref Carna::base::Vector3.
  *
  * \relates Carna::base::Vector3
  */
template< typename T >
inline bool operator==( const Carna::base::Vector3< T >& v1, const Carna::base::Vector3< T >& v2 )
{
    return Carna::base::Math::isEqual( v1.x, v2.x ) && Carna::base::Math::isEqual( v1.y, v2.y ) && Carna::base::Math::isEqual( v1.z, v2.z );
}


/** \brief  Defines non-equality for \ref Carna::base::Vector3.
  *
  * \relates Carna::base::Vector3
  */
template< typename T >
inline bool operator!=( const Carna::base::Vector3< T >& v1, const Carna::base::Vector3< T >& v2 )
{
    return !Carna::base::Math::isEqual( v1.x, v2.x ) || !Carna::base::Math::isEqual( v1.y, v2.y ) || !Carna::base::Math::isEqual( v1.z, v2.z );
}


/** \brief  Defines division for two \ref Carna::base::Vector3 objects.
  *
  * \relates Carna::base::Vector3
  *
  * The division is performed elementwise.
  */
template< typename T >
inline Carna::base::Vector3< T > operator/( const Carna::base::Vector3< T >& v1, const Carna::base::Vector3< T >& v2 )
{
    return Carna::base::Vector3< T >( v1.x / v2.x
                                    , v1.y / v2.y
                                    , v1.z / v2.z );
}


/** \brief  Defines division for \ref Carna::base::Vector3 and scalars.
  *
  * \relates    Carna::base::Vector3
  */
template< typename T >
inline Carna::base::Vector3< T > operator/( const Carna::base::Vector3< T >& v1, const T& scalar )
{
    return Carna::base::Vector3< T >( v1.x / scalar
                                    , v1.y / scalar
                                    , v1.z / scalar );
}


/** \brief  Defines multiplication for two \ref Carna::base::Vector3 objects.
  *
  * \relates Carna::base::Vector3
  *
  * The multiplication is performed elementwise.
  */
template< typename T >
inline Carna::base::Vector3< T > operator*( const Carna::base::Vector3< T >& v1, const Carna::base::Vector3< T >& v2 )
{
    return Carna::base::Vector3< T >( v1.x * v2.x
                                    , v1.y * v2.y
                                    , v1.z * v2.z );
}


/** \brief  Defines multiplication for \ref Carna::base::Vector3 and scalars.
  *
  * \relates Carna::base::Vector3
  */
template< typename T >
inline Carna::base::Vector3< T > operator*( const Carna::base::Vector3< T >& v1, const T& scalar )
{
    return Carna::base::Vector3< T >( v1.x * scalar
                                    , v1.y * scalar
                                    , v1.z * scalar );
}



#endif // VECTOR3_H_6014714286
