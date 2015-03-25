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

#include <Carna/base/Color.h>
#include <Carna/base/math.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Color
// ----------------------------------------------------------------------------------

const Color Color::BLACK         (   0,   0,   0, 255 );
const Color Color::BLACK_NO_ALPHA(   0,   0,   0,   0 );
const Color Color::WHITE         ( 255, 255, 255, 255 );
const Color Color::WHITE_NO_ALPHA( 255, 255, 255,   0 );
const Color Color::RED           ( 255,   0,   0, 255 );
const Color Color::RED_NO_ALPHA  ( 255,   0,   0,   0 );
const Color Color::GREEN         (   0, 255,   0, 255 );
const Color Color::GREEN_NO_ALPHA(   0, 255,   0,   0 );
const Color Color::BLUE          (   0,   0, 255, 255 );
const Color Color::BLUE_NO_ALPHA (   0,   0, 255,   0 );


Color::Color( unsigned char r, unsigned char g, unsigned char b, unsigned char a )
    : r( r )
    , g( g )
    , b( b )
    , a( a )
{
}


Color::Color( const math::Vector4f& fp )
    : r( static_cast< unsigned char >( math::round_ui( math::clamp< float >( fp.x(), 0, 1 ) / 255 ) ) )
    , g( static_cast< unsigned char >( math::round_ui( math::clamp< float >( fp.y(), 0, 1 ) / 255 ) ) )
    , b( static_cast< unsigned char >( math::round_ui( math::clamp< float >( fp.z(), 0, 1 ) / 255 ) ) )
{
}


Color::Color()
    : r( Color::BLACK_NO_ALPHA.r )
    , g( Color::BLACK_NO_ALPHA.g )
    , b( Color::BLACK_NO_ALPHA.b )
    , a( Color::BLACK_NO_ALPHA.a )
{
}


Color::operator math::Vector4f() const
{
    return math::Vector4f( r / 255.f, g / 255.f, b / 255.f, a / 255.f );
}


bool Color::operator==( const Color& c2 ) const
{
    const Color& c1 = *this;
    return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a;
}



}  // namespace Carna :: base

}  // namespace Carna
