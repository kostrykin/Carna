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

#include <Carna/base/view/Geometry.h>
#include <Carna/base/view/GeometryAggregate.h>
#include <Carna/base/CarnaException.h>
#include <vector>
#include <map>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// Geometry :: Details
// ----------------------------------------------------------------------------------

struct Geometry::Details
{
    std::map< unsigned int, GeometryAggregate* > aggregateByRole;
    std::map< GeometryAggregate*, unsigned int > roleByAggregate;
};



// ----------------------------------------------------------------------------------
// Geometry
// ----------------------------------------------------------------------------------

Geometry::Geometry( int geometryType )
    : pimpl( new Details() )
    , geometryType( geometryType )
{
}


Geometry::~Geometry()
{
    clearAggregates();
}


void Geometry::clearAggregates()
{
    std::vector< GeometryAggregate* > aggregates( pimpl->aggregateByRole.size() );
    unsigned int aggregateIdx = 0;
    for( auto aggregateItr = pimpl->aggregateByRole.begin(); aggregateItr != pimpl->aggregateByRole.end(); ++aggregateItr )
    {
        aggregates[ aggregateIdx++ ] = aggregateItr->second;
    }
    pimpl->aggregateByRole.clear();
    pimpl->roleByAggregate.clear();
    for( auto aggregateItr = aggregates.begin(); aggregateItr != aggregates.end(); ++aggregateItr )
    {
        GeometryAggregate* const ga = *aggregateItr;
        ga->removeFrom( *this );
    }
}


void Geometry::visitAggregates( const std::function< void( GeometryAggregate& ga, unsigned int role ) >& visit ) const
{
    for( auto aggregateItr = pimpl->aggregateByRole.begin(); aggregateItr != pimpl->aggregateByRole.end(); ++aggregateItr )
    {
        visit( *aggregateItr->second, aggregateItr->first );
    }
}


void Geometry::putAggregate( GeometryAggregate& ga, unsigned int role )
{
    const auto aggregateByRoleItr = pimpl->aggregateByRole.find( role );
    if( aggregateByRoleItr != pimpl->aggregateByRole.end() && aggregateByRoleItr->second != &ga )
    {
        /* Given role is already occupied by another aggregate.
         */
        removeAggregate( role );
    }
    const auto roleByAggregateItr = pimpl->roleByAggregate.find( &ga );
    if( roleByAggregateItr != pimpl->roleByAggregate.end() && roleByAggregateItr->second != role )
    {
        /* Given aggregate already occupies another role.
         */
        removeAggregate( ga );
    }
    const std::size_t size0 = pimpl->aggregateByRole.size();
    pimpl->aggregateByRole[ role ] = &ga;
    pimpl->roleByAggregate[ &ga ] = role;
    if( size0 != pimpl->aggregateByRole.size() )
    {
        ga.addTo( *this, role );
    }
}


void Geometry::removeAggregate( GeometryAggregate& ga )
{
    const auto aggregateItr = pimpl->roleByAggregate.find( &ga );
    if( aggregateItr != pimpl->roleByAggregate.end() )
    {
        const unsigned int role = aggregateItr->second;
        pimpl->roleByAggregate.erase( aggregateItr );
        pimpl->aggregateByRole.erase( role );
        ga.removeFrom( *this );
    }
}


void Geometry::removeAggregate( unsigned int role )
{
    const auto aggregateItr = pimpl->aggregateByRole.find( role );
    if( aggregateItr != pimpl->aggregateByRole.end() )
    {
        GeometryAggregate* const ga = aggregateItr->second;
        pimpl->roleByAggregate.erase( ga );
        pimpl->aggregateByRole.erase( aggregateItr );
        ga->removeFrom( *this );
    }
}


bool Geometry::hasAggregate( const GeometryAggregate& ga ) const
{
    return pimpl->roleByAggregate.find( const_cast< GeometryAggregate* >( &ga ) ) != pimpl->roleByAggregate.end();
}


bool Geometry::hasAggregate( unsigned int role ) const
{
    return pimpl->aggregateByRole.find( role ) != pimpl->aggregateByRole.end();
}


GeometryAggregate& Geometry::aggregate( unsigned int role ) const
{
    const auto aggregateItr = pimpl->aggregateByRole.find( role );
    CARNA_ASSERT( aggregateItr != pimpl->aggregateByRole.end() );
    return *aggregateItr->second;
}


std::size_t Geometry::aggregatesCount() const
{
    return pimpl->aggregateByRole.size();
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
