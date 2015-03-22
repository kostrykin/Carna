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
#include <Carna/base/Spatial.h>
#include <memory>

/** \file   Geometry.h
  * \brief  Defines \ref Carna::base::Geometry.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Geometry
// ----------------------------------------------------------------------------------

/** \brief
  * Defines \ref SceneGraph "scene graph" leafs. Instances of this class represent
  * visible geometry that can be rendered. The way of rendering is defined by the
  * \ref GeometryStage "geometry rendering stage" with matching \c %geometryType.
  *
  * Usually the corresponding rendering stage will query particular \em features from
  * this geometry object: Features are like components that make up the geometry
  * object in it's entirety, but the geometry object \em aggregates them, i.e. does
  * not take their possession. Rendering stages identify features through the
  * \em roles they take when associated with a geometry object.
  *
  * \note
  * Conventionally, the default role for \ref Texture3D "3D textures" that represent
  * HU data, such as \ref presets::DRRStage::ROLE_HU_VOLUME or
  * \ref presets::CuttingPlanesStage::ROLE_HU_VOLUME, is 0.
  * 
  * What features a geometry object aggregates depends on the \c %geometryType and
  * what the corresponding rendering stage expects. Typical examples are
  * \ref Mesh "meshes", \ref Material "materials" and
  * \ref Texture3D "volume textures".
  *
  * \author Leonid Kostrykin
  * \date   21.2.15 - 6.3.15
  */
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

    void setBoundingVolume( BoundingVolume* boundingVolume );

    bool hasBoundingVolume() const;

    BoundingVolume& boundingVolume();

    const BoundingVolume& boundingVolume() const;

}; // Geometry



}  // namespace Carna :: base

}  // namespace Carna

#endif // GEOMETRY_H_6014714286
