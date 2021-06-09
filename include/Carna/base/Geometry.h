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
  * \ref GeometryStage "geometry rendering stage" with matching `%geometryType`.
  *
  * Usually the corresponding rendering stage will query particular *features* from
  * this geometry object: Features are like components that make up the geometry
  * object in it's entirety, but the geometry object *aggregates* them, i.e. does
  * not take their possession. Rendering stages identify features through the
  * *roles* they take when associated with a geometry object.
  *
  * \note
  * Conventionally, the default role for 3D textures that represent HU data, such as
  * \ref presets::DRRStage::ROLE_INTENSITY_VOLUME or
  * \ref presets::CuttingPlanesStage::ROLE_INTENSITY_VOLUME, is 0.
  * 
  * What features a geometry object aggregates depends on the \c %geometryType and
  * what the corresponding rendering stage expects. Typical examples are
  * \ref ManagedMesh "meshes", \ref Material "materials" and
  * \ref ManagedTexture3D "volume textures".
  *
  * \author Leonid Kostrykin
  * \date   21.2.15 - 6.3.15
  */
class CARNA_LIB Geometry : public Spatial
{

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    /** \brief
      * Holds the \ref GeometryTypes "geometry type" of this geometry node.
      */
    const unsigned int geometryType;

    /** \brief
      * Instantiates.
      *
      * \param geometryType sets the \ref GeometryTypes "geometry type" of this
      *     geometry node.
      *
      * \param tag is an arbitrary string that may be used to identify this node.
      */
    explicit Geometry( unsigned int geometryType, const std::string& tag = "" );

    /** \brief
      * Deletes.
      */
    virtual ~Geometry();

    /** \brief
      * Adds the \a feature to this geometry node using \a role in
      * \f$\mathcal O\left(\log n\right)\f$. The concept of geometry features and
      * roles is explained \ref GeometryFeatures "here".
      *
      * \post `hasFeature(feature) == true`
      * \post `hasFeature(role) == true`
      * \post `&feature(role) == &feature`
      *
      * If \a feature was already added to this node, than its \a role is updated.
      * If \a role is already taken by another feature, then the older one is
      * overridden.
      *
      * \ref Spatial::invalidate "Invalidates" all parent subtrees.
      */
    void putFeature( unsigned int role, GeometryFeature& feature );

    /** \brief
      * Removes \a feature from this geometry node in
      * \f$\mathcal O\left(\log n\right)\f$. The concept of geometry features and
      * roles is explained \ref GeometryFeatures "here".
      *
      * Nothing happens if \a feature was not added to this geometry node previously.
      * \ref Spatial::invalidate "Invalidates" all parent subtrees otherwise.
      */
    void removeFeature( GeometryFeature& feature );

    /** \brief
      * Removes the \ref GeometryFeatures "geometry feature" from this node that is
      * associated with \a role in \f$\mathcal O\left(\log n\right)\f$.
      *
      * Nothing happens if no feature is associated with \a role at the moment.
      * \ref Spatial::invalidate "Invalidates" all parent subtrees otherwise.
      */
    void removeFeature( unsigned int role );

    /** \brief
      * Removes all \ref GeometryFeatures "geometry feature" from this node.
      *
      * \ref Spatial::invalidate "Invalidates" all parent subtrees if the features
      * were not an empty set.
      */
    void clearFeatures();

    /** \brief
      * Tells whether \a feature is attached to this node in
      * \f$\mathcal O\left(\log n\right)\f$.
      */
    bool hasFeature( const GeometryFeature& feature ) const;

    /** \brief
      * Tells whether this node has a feature with \a role attached in
      * \f$\mathcal O\left(\log n\right)\f$.
      */
    bool hasFeature( unsigned int role ) const;

    /** \brief
      * References the feature with \a role attached to this node in
      * \f$\mathcal O\left(\log n\right)\f$.
      *
      * \pre `hasFeature(role) == true`
      */
    GeometryFeature& feature( unsigned int role ) const;

    /** \brief
      * Tells number of \ref GeometryFeatures "geometry features" attached.
      */
    std::size_t featuresCount() const;

    /** \brief
      * Invokes \a visit once on each attached
      * \ref GeometryFeatures "geometry feature".
      */
    void visitFeatures( const std::function< void( GeometryFeature& gf, unsigned int role ) >& visit ) const;

    /** \brief
      * Sets \a boundingVolume as the minimum boundary volume of this node. Supply
      * `nullptr` for \a boundingVolume to remove it.
      */
    void setBoundingVolume( BoundingVolume* boundingVolume );

    /** \brief
      * Tells whether a minimum boundary volume is set on this node.
      */
    bool hasBoundingVolume() const;

    /** \brief
      * References the \ref setBoundingVolume "previously set" minimum boundary
      * volume.
      *
      * \pre `hasBoundingVolume() == true`
      */
    BoundingVolume& boundingVolume();

    /** \overload
      */
    const BoundingVolume& boundingVolume() const;

}; // Geometry



}  // namespace Carna :: base

}  // namespace Carna

#endif // GEOMETRY_H_6014714286
