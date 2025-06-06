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

#ifndef SPATIALMOVEMENT_H_6014714286
#define SPATIALMOVEMENT_H_6014714286

#include <LibCarna/LibCarna.hpp>
#include <LibCarna/base/noncopyable.hpp>
#include <LibCarna/base/math.hpp>
#include <memory>

/** \file
  * \brief
  * Defines \ref LibCarna::base::SpatialMovement.
  */

namespace LibCarna
{

namespace base
{



// ----------------------------------------------------------------------------------
// SpatialMovement
// ----------------------------------------------------------------------------------

/** \brief
  * Handles the displacement of a \ref Spatial object in a drag-&-drop like manner.
  *
  * If the object that is to be handled by an instance of this class is not
  * \ref Spatial::isMovable "movable", than instead it's first movable parent will be
  * moved. If no movable parent exists on the path from the object to the scene root,
  * nothing will be moved. Objects are always moved within the plane that is parallel
  * to the projection plane and intersects the object's origin.
  *
  * \attention
  * The class expects that neither the camera object, nor the projection, nor the
  * frame viewport do change for the duration of the movement.
  *
  * \author Leonid Kostrykin
  */
class LIBCARNA SpatialMovement
{

    NON_COPYABLE

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    /** \brief
      * Starts the spatial movement. This corresponds to "picking up" the spatial.
      *
      * \param movedSpatial
      *     references the spatial object to move.
      *
      * \param frameX
      *     is the \ref FrameCoordinates "first frame coordinate", where the spatial
      *     object has been picked up.
      *
      * \param frameY
      *     is the \ref FrameCoordinates "second frame coordinate", where the spatial
      *     object has been picked up.
      *
      * \param vp
      *     references the \ref FrameRenderer::viewport "root viewport" the frame was
      *     rendered with.
      *
      * \param cam
      *     references the camera the frame was rendered with.
      */
    SpatialMovement
        ( Spatial& movedSpatial
        , unsigned int frameX, unsigned int frameY
        , const Viewport& vp
        , const Camera& cam );

    /** \brief
      * Deletes.
      */
    ~SpatialMovement();

    /** \brief
      * References the \ref FrameRenderer::viewport "root viewport" that the frame
      * was rendered with.
      */
    const Viewport& viewport;

    /** \brief
      * References the camera that the frame was rendered with.
      */
    const Camera& cam;

    /** \brief
      * Holds the inverse of the \ref Camera::projection "projection matrix" that the
      * frame was rendered with.
      */
    const math::Matrix4f inverseProjection;

    /** \brief
      * Updates the moved spatial object's location. Returns \c true if and only if
      * the object's location changed.
      *
      * Invalidates the subtrees the moved spatial is within.
      *
      * \param frameX
      *     is the \ref FrameCoordinates "first frame coordinate", where the spatial
      *     object is to be moved to.
      *
      * \param frameY
      *     is the \ref FrameCoordinates "second frame coordinate", where the spatial
      *     object is to be moved to.
      */
    bool update( unsigned int frameX, unsigned int frameY );

    /** \brief
      * Tells `true` if the spatial given to the constructor is
      * \ref Spatial::isMovable "movable". Tells also `true` if it is not movable,
      * but another movable object exists on it's path to the root. Tells `false`
      * otherwise.
      */
    bool hasMovedSpatial() const;

    /** \brief
      * References the actually moved spatial.
      *
      * \pre `hasMovedSpatial() == true`
      */
    Spatial& movedSpatial() const;

}; // SpatialMovement



}  // namespace LibCarna :: base

}  // namespace LibCarna

#endif // SPATIALMOVEMENT_H_6014714286
