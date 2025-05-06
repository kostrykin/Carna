/*
 *  Copyright (C) 2010 - 2016 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 * 
 * 
 *  Copyright (C) 2021 - 2025 Leonid Kostrykin
 * 
 */

#ifndef HUV_H_6014714286
#define HUV_H_6014714286

#include <cmath>
#include <LibCarna/LibCarna.hpp>
#include <LibCarna/base/LibCarnaException.hpp>

/** \file
  * \brief
  * Defines \ref LibCarna::base::HUV.
  */

namespace LibCarna
{

namespace base
{



// ----------------------------------------------------------------------------------
// HUV
// ----------------------------------------------------------------------------------

/** \brief
  * Represents Hounsfield Units Values in \f$\left[-1024, +3071\right]\f$, usually used in CT imaging.
  *
  * \author Leonid Kostrykin
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
      * Creates HUV with \a value. Clamps to \f$\left[-1024, 3071\right]\f$.
      */
    explicit HUV( signed int value );

    /** \brief
      * Creates HUV corresponding to \a intensity. Clamps to \f$\left[-1024, 3071\right]\f$.
      */
    explicit HUV( float intensity );

    /** \brief
      * Returns the corresponding intensity.
      */
    float intensity() const;

}; // HUV


inline HUV::operator signed short() const
{
    return value;
}


inline HUV::HUV( signed int value )
    : value( value )
{
    if( this->value < -1024 ) this->value = -1024;
    if( this->value >  3071 ) this->value =  3071;
}


inline HUV::HUV( float intensity )
{
    const static float huvMax = 3071;
    if( intensity < 0 ) intensity = 0;
    if( intensity > 1 ) intensity = 1;
    const float huvFloat = intensity * 4095.f - 1024;
    value = static_cast< signed short >( std::lround( huvFloat ) );
}


inline float HUV::intensity() const
{
    return ( value + 1024 ) / 4095.f;
}



// ----------------------------------------------------------------------------------
// HUVOffset
// ----------------------------------------------------------------------------------

/** \brief
  * Represents offsets to \ref HUV "Hounsfield Units Values".
  *
  * \author Leonid Kostrykin
  */
struct HUVOffset
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
    HUVOffset() = default;

    /** \brief
     * Creates a HUV offset with \a value. Clamps to \f$\left[-4095, +4095\right]\f$.
     */
    explicit HUVOffset( signed int value );

    /** \brief
     * Creates a HUV offset corresponding to \a intensity. Clamps to \f$\left[-4095, +4095\right]\f$.
     */
    explicit HUVOffset( float intensity );

    /** \brief
     * Returns the corresponding intensity offset.
     */
    float intensity() const;

}; // HUVOffset


inline float HUVOffset::intensity() const
{
    return value / 4095.f;
}


inline HUVOffset::HUVOffset( signed int value )
    : value( value )
{
    if( this->value < -4095 ) this->value = -4095;
    if( this->value > +4095 ) this->value = +4095;
}


inline HUVOffset::HUVOffset( float intensity )
{
    if( intensity < -1 ) intensity = -1;
    if( intensity > +1 ) intensity = +1;
    const float huvFloat = intensity * 4095.f;
    value = static_cast< signed short >( std::lround( huvFloat ) );
}



}  // namespace LibCarna :: base

}  // namespace LibCarna

#endif // HUV_H_6014714286
