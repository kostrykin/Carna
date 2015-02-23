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

#ifndef GEOMETRYDEFINITION_H_6014714286
#define GEOMETRYDEFINITION_H_6014714286

/** \file   GeometryDefinition.h
  * \brief  Defines \ref Carna::base::view::GeometryDefinition.
  */

#include <Carna/base/noncopyable.h>
#include <set>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// GeometryDefinition
// ----------------------------------------------------------------------------------

/** \brief
  * Instances of this class do manage the lifetimes of OpenGL objects that reside in video memory.
  *
  * The management of their lifetimes require two basic operations, namely \em uploading content
  * from system to video memory and \em deleting them from video memory at a later point. Both
  * of these operations must be implemented by an instance of \ref VideoResourcesManager, that
  * is taken possession of by each instance of this class.
  *
  * This class uses a greedy strategy for the management: It orders the upload the moment
  * \ref acquireVideoResources "it is first told that the video resources are required" and it
  * deletes them as soon as \ref releaseVideoResources "it is told that they are no longer required".
  *
  * \author Leonid Kostrykin
  * \date   23.2.2015
  */
class CARNA_LIB GeometryDefinition
{

    NON_COPYABLE

    unsigned int videoResourcesAcquisitions;

    std::set< Geometry* > referencingSceneGraphNodes;

    const std::unique_ptr< VideoResourcesManager > vrm;

    bool released;

    GeometryDefinition( VideoResourcesManager* vrm );

    ~GeometryDefinition();

    bool deleteIfAllowed();

public:

    /** \brief
      * Instantiates. Call \ref release when you do not need the object any longer.
      */
    static GeometryDefinition* create( VideoResourcesManager* vrm );

    /** \brief
      * Increments video resource acquisition counter.
      * Uploads resources to video memory if not happened yet.
      *
      * \important
      * The caller must ensure that this method is called while proper OpenGL context is active.
      */
    void acquireVideoResources();
    
    /** \brief
      * Decrements video resource acquisition counter.
      * Frees acquired video memory if counter reaches 0.
      *
      * \important
      * The caller must ensure that this method is called while proper OpenGL context is active.
      */
    void releaseVideoResources();

    const VideoResourcesManager& videoResources() const;

    /** \brief
      * Denotes that this object is no longer required and may be deleted as soon as it is valid to delete it.
      *
      * It will be deleted as soon as the following conditions become true, which may also be immediately:
      * - The video resources are not loaded, i.e. the last acquisition has been released.
      * - This definition is no longer referenced by a scene graph.
      *
      * If no video resources are acquired currently, this object is deleted immediately.
      */
    void release();

    /** \brief
      * Puts this geometry definition on the \a sceneGraphNode.
      *
      * This is equivalent to \ref Geometry::setDefinition.
      */
    void applyTo( Geometry& sceneGraphNode );

    /** \brief
    * Removes this geometry definition from the \a sceneGraphNode.
    *
    * This is equivalent to \ref Geometry::removeDefinition.
      */
    void removeFrom( Geometry& sceneGraphNode );

}; // GeometryDefinition



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // GEOMETRYDEFINITION_H_6014714286
