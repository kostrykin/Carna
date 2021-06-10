/*
 *  Copyright (C) 2021 Leonid Kostrykin
 *
 */

#ifndef HUV_H_6014714286
#define HUV_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/CarnaException.h>

/** \file   HUV.h
  * \brief  Defines \ref Carna::base::HUV.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// HUV
// ----------------------------------------------------------------------------------

/** \brief
  * Represents values in \f$\left[-1024, +3071\right]\f$.
  *
  * \author Leonid Kostrykin
  * \date   June 2021
  */
struct HUV
{

    /** \brief
      * Holds the HU value.
      */
    signed short value;

    /** \brief
      * Implicitly casts into a <tt>signed short</tt> and yields \ref value.
      */
    operator signed short() const;

    /** \brief
      * Constructor.
      */
    HUV() = default;

    /** \brief
      * Creates HUV with \a value. Clamps to \f$\left[-1024, 3071\right]\f$ if
      * \a absolute is <tt>true</tt>.
      */
    explicit HUV( signed int value, bool absolute=false );

    /** \brief
      * Creates HUV corresponding to \a intensity. Clamps to
      * \f$\left[-1024, 3071\right]\f$ if \a absolute is <tt>true</tt>.
      */
    explicit HUV( float intensity, bool absolute=false );

    /** \brief
      * Wraps an absolute HU value.
      */
    template< typename T >
    static HUV abs( T value );

    /** \brief
      * Wraps a relative HU value.
      */
    template< typename T >
    static HUV rel( T value );

    /** \brief
      * Returns the corresponding absolute intensity.
      */
    float absIntensity() const;

    /** \brief
      * Returns the corresponding relative intensity.
      */
    float relIntensity() const;

}; // HUV


inline HUV::operator signed short() const
{
    return value;
}


inline HUV::HUV( signed int value, bool absolute )
    : value( value )
{
    if( absolute )
    {
        if( this->value < -1024 ) this->value = -1024;
        if( this->value >  3071 ) this->value =  3071;
    }
}


inline HUV::HUV( float intensity, bool absolute )
{
    const static float huvMax = 3071;
    if( absolute )
    {
        if( intensity < 0 ) intensity = 0;
        if( intensity > 1 ) intensity = 1;
    }
    float huvFloat = intensity * 4095.f + 0.5;
    if( absolute ) huvFloat -= 1024;
    if( huvFloat > huvMax ) huvFloat = huvMax;
    value = static_cast< signed short >( huvFloat );
}


inline float HUV::absIntensity() const
{
    return ( value + 1024 ) / 4095.f;
}


inline float HUV::relIntensity() const
{
    return value / 4095.f;
}


template< typename T >
HUV HUV::abs( T value )
{
    return HUV( value, true );
}


template< typename T >
HUV HUV::rel( T value )
{
    return HUV( value, false );
}



}  // namespace Carna :: base

}  // namespace Carna

#endif // HUV_H_6014714286
