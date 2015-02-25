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

#ifndef GEOMETRY_H_6014714286
#define GEOMETRY_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/view/Spatial.h>

/** \file   Geometry.h
  * \brief  Defines \ref Carna::base::view::Geometry.
  */

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// Geometry
// ----------------------------------------------------------------------------------

class CARNA_LIB Geometry : public Spatial
{

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    const int geometryType;

    Geometry( int geometryType );

    virtual ~Geometry();

    void putAggregate( GeometryAggregate&, unsigned int role );

    void removeAggregate( GeometryAggregate& );

    void removeAggregate( unsigned int role );

    void clearAggregates();

    bool hasAggregate( const GeometryAggregate& ) const;

    bool hasAggregate( unsigned int role ) const;

    GeometryAggregate& aggregate( unsigned int role ) const;

    std::size_t aggregatesCount() const;

    void visitAggregates( const std::function< void( GeometryAggregate& ga, unsigned int role ) >& ) const;

}; // Geometry



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // GEOMETRY_H_6014714286
