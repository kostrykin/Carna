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

#include <Carna/VolumeRenderings/DVR/ColorMap.h>
#include <Carna/VolumeRenderings/DVR/ColorMapDetails.h>
#include <Carna/base/Transformation.h>
#include <Carna/base/CarnaException.h>
#include <QColor>

namespace Carna
{

namespace VolumeRenderings
{

namespace DVR
{



// ----------------------------------------------------------------------------------
// DVR :: QColorToVector
// ----------------------------------------------------------------------------------

static inline base::Vector QColorToVector( const QColor& c )
{
    return base::Vector( c.redF(), c.greenF(), c.blueF(), c.alphaF() );
}


// ----------------------------------------------------------------------------------
// DVR :: ColorMap :: Details
// ----------------------------------------------------------------------------------

ColorMap::Details::Details( ColorMap& self )
    : self( self )
{
}


void ColorMap::Details::clear( ColorBuffer& out )
{
    out.resize( 4 * 4096 );

    for( unsigned int i = 0; i < out.size(); ++i )
    {
        out[ i ] = 0;
    }
}


void ColorMap::Details::mask( ColorBuffer& out, ColorMap::Details::HUV huv0, ColorMap::Details::HUV huv1, const QColor& firstColor, const QColor& lastColor )
{
    CARNA_ASSERT( huv0 >= -1024 && huv1 >= -1024 );
    CARNA_ASSERT( huv0 <= 3071 && huv1 <= 3071 );

    const base::Vector color0 = QColorToVector( firstColor );
    const base::Vector color1 = QColorToVector( lastColor );

    for( unsigned int i = unsigned( huv0 + 1024 ); i < unsigned( huv1 + 1024 ); ++i )
    {
        const double lambda = static_cast< double >( i - 1024 - huv0 ) / ( huv1 - huv0 - 1 );
        const base::Vector color = color0 + lambda * ( color1 - color0 );

        for( unsigned int j = 0; j < 4; ++j )
        {
            out[ 4 * i + j ] = static_cast< char >( color[ j ] * 0xFF );
        }
    }
}



// ----------------------------------------------------------------------------------
// DVR :: ColorMap
// ----------------------------------------------------------------------------------

ColorMap::ColorMap( QObject* parent )
    : QObject( parent )
    , pimpl( new Details( *this ) )
{
}


ColorMap::~ColorMap()
{
}


void ColorMap::writeToBuffer( ColorBuffer& out ) const
{
    pimpl->clear( out );

    for( auto it = pimpl->spans.begin(); it != pimpl->spans.end(); ++it )
    {
        const HUSpan& huRange = it->first;
        const ColorSpan& colorGradient = it->second;

        pimpl->mask
            ( out
            , huRange.first
            , huRange.last
            , colorGradient.first
            , colorGradient.last );
    }

    CARNA_ASSERT( out.size() == 4 * 4096 );
}


void ColorMap::clear()
{
    pimpl->spans.clear();

    emit changed();
}


void ColorMap::put( const HUSpan& huRange, const ColorMap::ColorSpan& colorGradient )
{
    CARNA_ASSERT( huRange.first != huRange.last );

    for( auto it = pimpl->spans.begin(); it != pimpl->spans.end(); ++it )
    {
        const HUSpan& huRange2 = it->first;

        CARNA_ASSERT( huRange.first < huRange2.first || huRange.first > huRange2.last );
        CARNA_ASSERT( huRange.last < huRange2.first || huRange.last > huRange2.last );
    }

    pimpl->spans[ huRange ] = colorGradient;

    emit changed();
}


const ColorMap::SpanMap& ColorMap::data() const
{
    return pimpl->spans;
}



}  // namespace Carna :: VolumeRenderings :: DVR

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna
