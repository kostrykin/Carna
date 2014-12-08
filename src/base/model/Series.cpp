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

#include <Carna/base/model/Series.h>
#include <Carna/base/model/SeriesElement.h>
#include <Carna/base/CarnaException.h>
#include <climits>

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// Types & Globals
// ----------------------------------------------------------------------------------

const static double SPACING_OUT_OF_DATE = -std::numeric_limits< double >::infinity();



// ----------------------------------------------------------------------------------
// Series
// ----------------------------------------------------------------------------------

Series::Series( const std::string& name )
    : name( name )
    , zSpacing( SPACING_OUT_OF_DATE )
{
}


Series::~Series()
{
    std::for_each( elements.begin(), elements.end(), std::default_delete< SeriesElement >() );
}


bool Series::IsCloser::operator()( SeriesElement* e1, SeriesElement* e2 ) const
{
    return e1->zPosition != e2->zPosition ? e1->zPosition < e2->zPosition : e1->fileName < e2->fileName;
}


const Series::OrderedElements& Series::getElements() const
{
    return elements;
}


void Series::put( SeriesElement* element )
{
    elements.insert( element );
    element->setSeries( *this );

    zSpacing = SPACING_OUT_OF_DATE;
}


double Series::getSpacingZ() const
{
    CARNA_ASSERT( elements.size() >= MIN_ELEMENTS_COUNT );

    if( zSpacing == SPACING_OUT_OF_DATE )
    {
        auto it = elements.begin();
        zSpacing  = ( **it ).zPosition;
        zSpacing -= ( **( ++it ) ).zPosition;
        zSpacing *= -1;
    }

    return zSpacing;
}


const SeriesElement& Series::getCentralElement() const
{
    CARNA_ASSERT( elements.size() >= MIN_ELEMENTS_COUNT );

    const auto& last = **( --elements.end() );
    const auto& first = **( elements.begin() );

    const double z_max = last.zPosition;
    const double z_min = first.zPosition;

    CARNA_ASSERT( z_max >= z_min );

    const double z_mid = z_min + ( z_max - z_min ) / 2;

    double min_z_mid_distance = std::numeric_limits< double >::max();
    SeriesElement* mid_closest_element = nullptr;
    for( auto current_itr = elements.begin(); current_itr != elements.end(); ++current_itr )
    {
        const double z_mid_distance = std::abs( ( **current_itr ).zPosition - z_mid );
        if( z_mid_distance > min_z_mid_distance )
        {
            break;
        }
        else
        {
            mid_closest_element = *current_itr;
            min_z_mid_distance = z_mid_distance;
        }
    }

    return *mid_closest_element;
}


bool Series::contains( const std::string& fileName ) const
{
    for( auto current_itr = elements.begin(); current_itr != elements.end(); ++current_itr )
    {
        if( ( **current_itr ).fileName == fileName )
        {
            return true;
        }
    }
    return false;
}



}  // namespace Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna
