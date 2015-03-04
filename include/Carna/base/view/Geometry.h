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

    const unsigned int geometryType;

    Geometry( int unsigned geometryType );

    virtual ~Geometry();

    void putFeature( unsigned int role, GeometryFeature& );

    void removeFeature( GeometryFeature& );

    void removeFeature( unsigned int role );

    void clearFeatures();

    bool hasFeature( const GeometryFeature& ) const;

    bool hasFeature( unsigned int role ) const;

    GeometryFeature& feature( unsigned int role ) const;

    std::size_t featuresCount() const;

    void visitFeatures( const std::function< void( GeometryFeature& gf, unsigned int role ) >& ) const;

}; // Geometry



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // GEOMETRY_H_6014714286
