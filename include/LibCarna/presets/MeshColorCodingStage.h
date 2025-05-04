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

#ifndef MESHCOLORCODINGSTAGE_H_6014714286
#define MESHCOLORCODINGSTAGE_H_6014714286

#include <LibCarna/LibCarna.h>
#include <LibCarna/base/GeometryStage.h>
#include <LibCarna/base/Aggregation.h>

/** \file   MeshColorCodingStage.h
  * \brief  Defines \ref LibCarna::presets::MeshColorCodingStage.
  */

namespace LibCarna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// MeshColorCodingStage
// ----------------------------------------------------------------------------------

/** \brief
  * Implements a simple interface for mapping
  * \ref FrameCoordinates "frame coordinates" to \ref base::Mesh objects.
  *
  * Lets assume we have an \ref OpaqueRenderingStage defined, e.g.:
  *
  * \snippet ModuleTests/MeshColorCodingStageTest.cpp MeshColorCodingStage_Opaque_Setup
  *
  * The concept of geometry types is explained \ref GeometryTypes "here".
  *
  * Than we can instantiate the `%MeshColorCodingStage` like this:
  *
  * \snippet ModuleTests/MeshColorCodingStageTest.cpp MeshColorCodingStage_instantiation
  *
  * After rendering the frame, we can query the `%MeshColorCodingStage` object for
  * meshes at particular frame coordinates using its \ref pick method.
  *
  * \note
  * In the \ref RenderingProcess "rendering process" this stage can be inserted
  * *anywhere*.
  *
  * \see
  * The \ref base::SpatialMovement class in combination with `%MeshColorCodingStage`
  * makes the implementation of drag-&-drop like behaviour for \ref base::Spatial
  * objects very easy.
  *
  * \author Leonid Kostrykin
  */
class LIBCARNA MeshColorCodingStage : public base::GeometryStage< void >
{

    struct Details;
    const std::unique_ptr< Details > pimpl;

    struct VideoResources;
    std::unique_ptr< VideoResources > vr;

public:

    /** \brief
      * Instantiates.
      */
    MeshColorCodingStage();

    /** \brief
      * Deletes.
      */
    virtual ~MeshColorCodingStage();

    /** \brief
      * Sets on which pass this stage should do its work.
      *
      * If this stage is processed multiple times per frame, than it will contribute
      * its own processing logic to the rendering process only on the \f$n\f$th pass,
      * where \f$n\f$ is \a activationPassIndex. Default is \f$n = 0\f$.
      */
    void setActivationPassIndex( unsigned int activationPassIndex );

    /** \brief
      * Tells on which pass this stage will do its work.
      *
      * If this stage is processed multiple times per frame, than it will contribute
      * its own processing logic to the rendering process only on the \f$n\f$th pass,
      * where \f$n\f$ is the returned value. Default is \f$n = 0\f$.
      */
    unsigned int activationPassIndex() const;

    /** \brief
      * Activates \a geometryType. Meshes with \a meshRole attached to geometry nodes
      * with \a geometryType will be processed by this stage.
      *
      * If \a geometryType was activated with another \a meshRole previosly, than the
      * old \a meshRole is overridden by the new one.
      */
    void putGeometryType( unsigned int geometryType, unsigned int meshRole );
    
    /** \brief
      * Enables support for materials if \a geometryType has been
      * \ref putGeometryType "activated" or will be activated later. Otherwise this
      * method has no effect.
      *
      * The behaviour that this enables is the following at the moment and may be
      * extended in the future. If the stage encounters a mesh with
      * \ref base::ManagedMeshBase::primitiveType set to
      * \ref base::IndexBufferBase::PRIMITIVE_TYPE_POINTS, it queries its material
      * for a `pointSize` parameter that it than takes account of.
      */
    void enableMaterials( unsigned int geometryType, unsigned int materialRole );

    /** \brief
      * Deactivates \a geometryType. Also disables the occasionally enabled material.
      */
    void removeGeometryType( unsigned int geometryType );

    /** \brief
      * Deactivates all geometry types.
      */
    void clearGeometryTypes();

    virtual void reshape( base::FrameRenderer& fr, unsigned int width, unsigned int height ) override;

    virtual void renderPass( const base::math::Matrix4f& viewTransform, base::RenderTask& rt, const base::Viewport& vp ) override;

    /** \brief
      * Maps the \ref FrameCoordinates "frame coordinates" \a x and \a y to the
      * \ref base::Geometry object that was rendered at the queried location. If no
      * object was rendered there, \ref base::Aggregation::NULL_PTR is returned.
      */
    base::Aggregation< const base::Geometry > pick( unsigned int x, unsigned int y ) const;

    /** \overload
      */
    base::Aggregation< const base::Geometry > pick( const base::math::Vector2ui& ) const;

protected:

    virtual void render( const base::Renderable& renderable ) override;

}; // MeshColorCodingStage



}  // namespace LibCarna :: presets

}  // namespace LibCarna

#endif // MESHCOLORCODINGSTAGE_H_6014714286
